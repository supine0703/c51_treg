#define uchar unsigned char

#include "ds18b20.h"

extern void _nop_(void);

/*
void DS18B20_Delay10us(uchar t) // 延迟t * 10us
{
    if (t == 1) return;
    t -= 2;
    _nop_();
    while (t--) { }
}*/
// 6MHz 延迟t * 10us

// 12MHz 延迟t * 10us
void DS18B20_Delay10us(uchar t)
{ // lcall mov ret : 5us
    uchar i;
    _nop_(); // nop : 1us
    if (!--t)
        return; // dec mov jz : 4us
    do          // 1+1+6+2 = 10us
    {
        _nop_(); // nop : 1us
        i = 3;   // mov : 1us
        do
        {
        } while (--i); // djnz : 2us
    } while (--t);     // djnz : 2us
} // (t-1)*10us + (5+1+4)us

// -------------------------------------

char DS18B20_InitCheck(void)
{
    char x = 0;
    DQ = 1; // 复位DQ
    DQ = 0;
    DS18B20_Delay10us(60); // 拉低 480~960us
    DQ = 1;
    DS18B20_Delay10us(12); // 等待 15~60us 240us之内
    x = DQ;                // 总线60~240us低电平
    DQ = 1;                // 释放总线
    DS18B20_Delay10us(24); // 保证时序完整
    return x;
} // 约 970

void DS18B20_WriteByte(uchar dat)
{
    uchar i = 8;
    do // 串行写8位数据，先写低位后写高位
    {
        DQ = 0;
        DS18B20_Delay10us(1); // 至少间隔1us 低于15us
        DQ = dat & 0x01;      // 写"1" 在15μs内拉高
        DS18B20_Delay10us(5); // 写"0" 拉低60μs 10+50
        DQ = 1;
        dat >>= 1;
    } while (--i);
} // 约 550 (551)

uchar DS18B20_ReadByte(void)
{
    uchar i = 8, dat = 0;
    do // 串行读8位数据，先读低位后读高位
    {
        DQ = 0; // 拉低
        DS18B20_Delay10us(1);
        DQ = 1; // 15μs内拉高释放总线
        dat >>= 1;
        if (DQ)
            dat |= 0x80;
        DS18B20_Delay10us(5); // 最少60us
    } while (--i);
    return dat;
} // 约 560 (559)

// -------------------------------------

void DS18B20_Convert() // 温度转换
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0x44);
} // 约 2080 (2076)

int DS18B20_ReadTemp() // 温度读取
{
    uchar low = 0, high = 0;
    int temp = 0;
    if (DS18B20_InitCheck())
        return 0;
    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0xbe);
    low = DS18B20_ReadByte();
    high = DS18B20_ReadByte();
    temp = (high << 8) | low;
    return temp;
} // 约 3210 (3211)

// -------------------------------------

void DS18B20_Set(uchar upperLimit, uchar lowerLimit, uchar resolution)
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0x4e);       // 写暂存器指令4E
    DS18B20_WriteByte(upperLimit); // 写高速缓存器TH高温限值
    DS18B20_WriteByte(lowerLimit); // 写高速缓存器TL低温限值
    DS18B20_WriteByte((resolution << 5) | 0x1f); // 精度设置
}

void DS18B20_Get(uchar* upperLimit, uchar* lowerLimit, uchar* resolution)
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0xbe);
    DS18B20_ReadByte();
    DS18B20_ReadByte();
    *upperLimit = DS18B20_ReadByte();
    *lowerLimit = DS18B20_ReadByte();
    *resolution = DS18B20_ReadByte();
    *resolution >>= 5;
}

void DS18B20_Save(void)
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0x48);
}
