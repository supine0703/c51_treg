/**
 * 作者：李宗霖 日期：2023/11/16
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * 以下头文件对此文件是强依赖：
 *   lcd1602.*  -- 2
 *   ds18b20.*  -- 3
 * 也可以自己定义配置文件以减少耦合
 * 将所有定义放与config文件方便修改单片机配置
 * ----------------------------------------------
 * 此头文件可以调整对 c5x 单片机数据端口的定义
 * 需要满足以下要求：
 *   1. 对uint uchar定义
 *   2. out LCD_RS LCD_RW LCD_EN 为了lcd1602定义
 *   3. DQ 为了ds18b20定义
 */
 
#ifndef __CONFIG___H
#define __CONFIG___H

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

// -------------------------------------

#include <reg52.h>

void Delay(uint t);	// 延迟tms

// ------- define for lcd1602 ----------

// #define LCD1602_USE_DEFAULT // 使用默认配置 手动配置后不可再这样做
#define LCD1602_NOREADDATA	// 不编译LCD1602_ReadData(void)
#define LCD1602_DATA P0		// 数据 to LCD1602
sbit LCD1602_RS = P1^0; 	// 寄存器选择
sbit LCD1602_RW = P1^1; 	// 读/写
sbit LCD1602_EN = P1^2; 	// 使能

// ------- define for ds18b20 ----------

sbit DQ = P1^5;

// -------------------------------------

#endif // __CONFIG___H