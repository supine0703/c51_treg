#include "i2c.h"

extern void _nop_(void);

void I2C_Wait(void) // 4us
{
}

void I2C_Start(void)
{
    SCL = 1;
    SDA = 1;
    _nop_();
    I2C_Wait(); // 5us
    SDA = 0;
    _nop_();
    I2C_Wait(); // 5us
    SCL = 0;
}

void I2C_Stop(void)
{
    SDA = 0;
    SCL = 1;
    _nop_();
    I2C_Wait(); // 5us
    SDA = 1;
    _nop_();
    I2C_Wait(); // 5us
}

bit I2C_CheckAck(void)
{
    bit flg;
    SDA = 1;
    I2C_Wait();
    SCL = 1;
    I2C_Wait();
    flg = SDA;
    SCL = 0;
    return !flg;
}

void I2C_Ack(void)
{
    SDA = 0;
    I2C_Wait();
    SCL = 1;
    I2C_Wait();
    SCL = 0;
    I2C_Wait();
    SDA = 1;
    _nop_();
}

void I2C_NoAck(void)
{
    SDA = 1;
    I2C_Wait();
    SCL = 1;
    I2C_Wait();
    SCL = 0;
    I2C_Wait();
    SDA = 0;
    _nop_();
}

void I2C_SendByte(uchar dat)
{
    uchar i;
    SCL = 0;
    for (i = 8; i; --i)
    {
        SDA = (bit)(dat & 0x80);
        SCL = 1;
        I2C_Wait();
        SCL = 0;
        dat <<= 1;
    }
    SDA = 1;
}

uchar I2C_RecByte(void)
{
    uchar i, dat;
    SDA = 1;
    for (i = 8; i; --i)
    {
        dat <<= 1;
        SCL = 0;
        I2C_Wait();
        SCL = 1;
        I2C_Wait();
        dat |= SDA;
    }
    SCL = 0;
    I2C_Wait();
    return dat;
}


void I2C_Init(void)
{
    SCL = 1;
    _nop_();
    SDA = 1;
    _nop_();
    I2C_Wait();
    I2C_Stop();
}

bit I2C_WriteData(uchar sla, uchar suba, uchar* dat, uchar num)
{
    bit flg;
    uchar i;
    I2C_Start();       // 启动I2C总线
    I2C_SendByte(sla); // 发送器件地址码
    if (flg = I2C_CheckAck())
    {
        I2C_SendByte(suba); // 发送期间单元地址
        if (flg = I2C_CheckAck())
        {
            for (i = num; i; --i)
            {
                I2C_SendByte(*dat);
                if (!(flg = I2C_CheckAck()))
                    break; // 无应答
                ++dat;
            }
        }
    }
    I2C_Stop(); // 发送结束信号
    return flg;
}

bit I2C_ReadData(uchar sla, uchar suba, uchar* dat, uchar num)
{
    bit flg;
    uchar i;
    I2C_Start();       // 启动I2C总线
    I2C_SendByte(sla); // 发送器件地址码
    if (flg = I2C_CheckAck())
    {
        I2C_SendByte(suba); // 发送期间单元地址
        if (flg = I2C_CheckAck())
        {
            I2C_Start();           // 重新启动I2C总线
            I2C_SendByte(sla + 1); // 发送写器件地址码信号
            if (flg = I2C_CheckAck())
            {
                for (i = num - 1; i; --i)
                {
                    *dat = I2C_RecByte();
                    I2C_Ack();
                    ++dat;
                }
                *dat = I2C_RecByte();
                I2C_NoAck();
            }
        }
    }
    I2C_Stop();
    return flg;
}
