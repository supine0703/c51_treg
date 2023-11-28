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
void DS18B20_Delay10us(unsigned char t)
{
	unsigned char i;
	// _nop_();
	if (!--t) return;
	do 
	{	_nop_();
		i = 3;
		do { } while (--i);
	} while (--t);
}

// -------------------------------------

bit DS18B20_InitCheck(void)
{
	bit x = 0;
	DQ = 1; 	// 复位DQ
	DQ = 0; 
    DS18B20_Delay10us(60); // 拉低 480~960us
    DQ = 1;
    DS18B20_Delay10us(12); // 等待 15~60us 240us之内
	x = DQ; 	// 总线60~240us低电平
	DQ = 1; 	// 释放总线
	DS18B20_Delay10us(24);
	return x;
}

void  DS18B20_WriteByte(uchar dat)
{
	unsigned char i = 8;
	do // 串行写8位数据，先写低位后写高位
	{	DQ = 0;
		DS18B20_Delay10us(1);
        DQ = dat & 0x01; 		// 写"1" 在15μs内拉低
        DS18B20_Delay10us(6);   // 写"0" 拉低60μs
        DQ = 1;
        dat >>= 1;
	} while (--i);
}

unsigned char DS18B20_ReadByte(void)
{
	unsigned char i = 8, dat = 0;
	do // 串行读8位数据，先读低位后读高位
	{	DQ = 0; 				// 拉低
        DS18B20_Delay10us(1);
        DQ = 1; 				// 15μs内拉高释放总线
        dat >>= 1;
        if (DQ) dat |= 0x80;
        DS18B20_Delay10us(4);
	} while (--i);
    return dat;
}

// -------------------------------------

void DS18B20_Convert() //温度转换
{
	if (DS18B20_InitCheck()) return;
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0x44);
}

int DS18B20_ReadTemp() //温度读取
{
	unsigned char low = 0, high = 0;
	int temp = 0;
	if (DS18B20_InitCheck()) return 0;
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0xbe);
	low  = DS18B20_ReadByte();
	high = DS18B20_ReadByte();
	temp = (high << 8) | low;
	return temp;
}

// -------------------------------------

void DS18B20_Set(unsigned char temp_low, unsigned char temp_high, unsigned char accuracy)
{
	if(DS18B20_InitCheck()) return;
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0x4e);	//写暂存器指令4E	
	DS18B20_WriteByte(temp_high);		//写高速缓存器TH高温限值 temp_high 度
	DS18B20_WriteByte(temp_low);		//写高速缓存器TL低温限值 temp_low  度
	DS18B20_WriteByte(accuracy);		//精度设置 
}

void DS18B20_Get(unsigned char* temp_low, unsigned char* temp_high)
{
	if(DS18B20_InitCheck()) return;
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0xbe);
	DS18B20_ReadByte();
	DS18B20_ReadByte();
	*temp_high = DS18B20_ReadByte();
	*temp_low  = DS18B20_ReadByte();
}	

void DS18B20_Save()
{
	if(DS18B20_InitCheck()) return;
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0x48);
	DQ = 1;
	DS18B20_Delay10us(200);
	DS18B20_Delay10us(200);
	DS18B20_Delay10us(200);
	DS18B20_Delay10us(200);
	DS18B20_Delay10us(200);
}

void DS18B20_Update()
{
	uchar dat;
	if(DS18B20_InitCheck()) return;
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0xb8);
	dat = DS18B20_ReadByte();
	while (!(dat|0)) { }
}

void DS18B20_CheckAlarm()
{
	uchar dat;
	if(DS18B20_InitCheck()) return;
	DS18B20_WriteByte(0xec);
	DS18B20_WriteByte(0xec);
	dat = DS18B20_ReadByte();
	while (!(dat|0)) { }
}
