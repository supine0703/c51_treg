/**
 * 作者：李宗霖 日期：2023/12/04
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * - 此头文件对文件：'__config__.h' 强依赖
 * - 数据端口 控制端口 需要通过'__config__.h'配置 SDA SCL
 * ----------------------------------------------
 */
#include "__config__.h"
#ifdef I2C_USE_DEFAULT
#include <REG52.H>
#undef I2C_DEFINE_SDA
#undef I2C_DEFINE_SCL
#define I2C_DEFINE_SDA P1 ^ 7
#define I2C_DEFINE_SCL P1 ^ 6
#endif

sbit SDA = I2C_DEFINE_SDA;
sbit SCL = I2C_DEFINE_SCL;

#ifndef uchar
#define uchar unsigned char
#endif

extern void _nop_(void);

void I2C_Wait(void) // 4us
{
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

// void I2C_Init(void)
// {
//     SCL = 1;
//     _nop_();
//     SDA = 1;
//     _nop_();
//     I2C_Wait();
//     I2C_Stop();
// }

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
