/**
 * 作者：李宗霖 日期：2023/12/04
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * 注意: 24c02的操作是基于I2C操作的 所以对其为强依赖 'i2c.h'
 */
#include "i2c.h"

#ifndef uchar
#define uchar unsigned char
#endif

#define PAGE_BYTE 0x10 // 也缓冲器字节数

// 连续读时序
bit At24c02_ReadData(uchar sla, uchar suba, uchar* dat, uchar num)
{
    bit flg;
    uchar i;
    if (!num)
        return 1;
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

// 页写入时序 最多为 页写缓冲器的大小 字节 超出会被循环覆盖 且此函数不带延迟
bit At24c02_WriteByte(uchar sla, uchar suba, uchar* dat, uchar num)
{
    bit flg;
    uchar i;
    if (!num)
        return 1;
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

void At24c02_Wait(void) // 12MHz 1000us
{
    uchar t = 164;
    while (t--)
        ;
    for (t = 2; t; --t)
        ;
}

bit At24c02_Check(uchar sla) // 检查是否写入完成
{
    uchar i = 16;
    do // 每毫秒检测一次如果 16 次未应答则是为错误
    {
        At24c02_Wait();
        I2C_Start();
        I2C_SendByte(sla); // 发送器件地址码
    } while (!I2C_CheckAck() && --i);
    I2C_Stop();
    return (i != 0);
}

// 连续页写入 带延迟(确保写入完成)
bit At24c02_WriteData(uchar sla, uchar suba, uchar* dat, uchar num)
{
    while (num > PAGE_BYTE) // 按页写入数据 并且等待写入成功开始下一页写入
    {
        if (!At24c02_WriteByte(sla, suba, dat, PAGE_BYTE))
            return 0;
        suba += PAGE_BYTE;
        dat += PAGE_BYTE;
        num -= PAGE_BYTE;
        if (!At24c02_Check(sla))
            return 0;
    }
    if (At24c02_WriteByte(sla, suba, dat, num))
        return At24c02_Check(sla);
    else
        return 0;
}

/**
 * 2023/12/04
 * 最开始学24c02我以为页写入操作是存储空间被例如 划分为了 每页16byte 共16page
 * 我以为是只能以每页为单位进行存储 例如 0x03 开始存储到0x0f会自动返回到0x00
 * 因为许多人使用 '不会自动翻页' 来形容页写入 甚至有人认为有 '页读取'
 * 这是非常离谱的误会 下面有我实现的 '错误' 的连续页写入方式
实现方法很有参考价值
 * 故此保留 参见24c02的数据手册可知 需要页写入时因为受限于 页写缓冲器的大小

#define PAGE_BYTE 0x10
#define PAGE_BIT 4
#define PAGE_END 0x0f

bit I2C_WriteData(uchar sla, uchar suba, uchar* dat, uchar num)
{
    bit flg;
    uchar i;
    if (!num)
        return 1;
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

void At24c02_Write(uchar sla, uchar suba, uchar* dat, uchar num)
{
    uchar begin, end, i;
    if (!num)
        return;
    begin = suba >> PAGE_BIT;
    end = (suba + (num - 1)) >> PAGE_BIT;
    if (begin != end)
    {
        i = PAGE_END - (suba & PAGE_END) + 1;
        I2C_WriteData(sla, suba, dat, i);
        suba += i;
        dat += i;
        ++begin;
        At24c02_Wait();
    }
    while ((begin != end) && begin < PAGE_END)
    {
        I2C_WriteData(sla, suba, dat, PAGE_BYTE);
        suba += PAGE_BYTE;
        dat += PAGE_BYTE;
        ++begin;
        At24c02_Wait();
    }
    I2C_WriteData(
        sla, suba, dat, (begin == end ? (num - i) & PAGE_END : PAGE_END)
    );
    At24c02_Wait();
}

*/
