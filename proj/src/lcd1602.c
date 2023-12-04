/**
 * 作者：李宗霖 日期：2023/11/16
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * - 此头文件对文件：'__config__.h' 强依赖
 * - 数据端口 控制端口 需要通过'__config__.h'配置
 * - LCD1602_D RS RW EN
 * ----------------------------------------------
 * 写数据和写命令完全不需要在最后加上delay，如果连续执行不正常，就是CheckBusy有问题
 */
#include "__config__.h"
/**
 * - 如果 '__config__.h' 文件丢失 或者出现问题 创建空的 '__config__.h' 文件
 * - 在 '__config__.h' 中 定义 LCD_1602_USE_DEFAULT 以降低对 '__config__.h'
 * 的依赖
 * - 也可以直接在此文件中进行配置:
 *     好处是取消对'__config__.h' 的依赖 降低耦合
 *     代价是没有集中端口配置 让配置、修改、调试、移植 的成本变高
 */
#ifdef LCD1602_USE_DEFAULT
#include <REG52.H>
#undef LCD1602_DEFINE_RS
#undef LCD1602_DEFINE_RW
#undef LCD1602_DEFINE_EN
#undef LCD1602_DATA
#define LCD1602_DEFINE_RS P1 ^ 0 // 寄存器选择
#define LCD1602_DEFINE_RW P1 ^ 1 // 读/写
#define LCD1602_DEFINE_EN P1 ^ 2 // 使能
#define LCD1602_DATA P0          // 数据 to LCD
#endif

sbit RS = LCD1602_DEFINE_RS; // 寄存器选择
sbit RW = LCD1602_DEFINE_RW; // 读/写
sbit EN = LCD1602_DEFINE_EN; // 使能
#define DT LCD1602_DATA

// -------------------------------------

void LCD1602_CheckBusy(void)
{
    unsigned char busy;
    DT = 0xff;
    do
    {
        busy = 0xff;
        EN = 0;
        RS = 0;
        RW = 1;
        EN = 1;
        busy = DT;
    } while (busy & 0x80);
    EN = 0;
}

void LCD1602_WriteByte(bit rs, unsigned char byte);

void LCD1602_WriteCmd(unsigned char cmd)
{
    if (!cmd)
        return;
    LCD1602_WriteByte(0, cmd);
}

void LCD1602_WriteData(unsigned char dat)
{
    LCD1602_WriteByte(1, dat);
}

#ifndef LCD1602_NO_READDATA
unsigned char LCD1602_ReadData(void)
{
    unsigned char dat;
    LCD1602_CheckBusy();
    DT = 0xff;
    RW = 1;
    RS = 1;
    EN = 1;
    dat = DT;
    EN = 0;
    return dat;
}
#endif

// -------------------------------------

extern void _nop_(void);

void LCD1602_WriteByte(bit rs, unsigned char byte)
{
    LCD1602_CheckBusy();
    RW = 0;
    RS = rs; // 0 写命令  1 写数据
    DT = byte;
    EN = 1; // EN: 0 -> 1
    _nop_();
    EN = 0;
}

// -------------------------------------
