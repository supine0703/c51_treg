/**
 * 作者：李宗霖 日期：2023/11/16
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * - 此头文件对文件：'__config__.h' 强依赖
 * - 数据端口 控制端口 需要通过'__config__.h'配置
 * - LCD1602_D LCD1602_RS LCD1602_RW LCD1602_EN
 * ----------------------------------------------
 * - LCD1602 的所有操作都是基于端口的读写操作的
 * - 本文件定义了 LCD1602 四个最基本操作函数
 * - 基于写命令 通过宏定义封装了 LCD1602的指令表
 * - 包含了 LCD1602 的 所有 可行操作
 * - 所有更复杂的逻辑 都可通过包含此头文件进行二
 * 次封装 eg: 初始化 写入字符串 屏幕闪烁 等
 */
 
#ifndef LCD1602_H
#define LCD1602_H

#include "__config__.h"
/**
 * - 如果 '__config__.h' 文件丢失 或者出现问题 创建空的 '__config__.h' 文件
 * - 在 '__config__.h' 中 定义 LCD_1602_USE_DEFAULT 以降低对 '__config__.h' 的依赖
 * - 也可以直接修改此文件 进行配置:
 *     好处是取消对 '__config__.h' 的依赖 降低耦合
 *     代价是没有集中端口配置 让配置、修改、调试、移植 的成本变高
 */
#ifdef LCD1602_USE_DEFAULT
	#include <reg52.h>
	#define LCD1602_DATA P0	// 数据 to LCD
	sbit LCD1602_RS = P1^0; // 寄存器选择
	sbit LCD1602_RW = P1^1; // 读/写
	sbit LCD1602_EN = P1^2; // 使能
#endif

// -------------------------------------

// 定义LCD1602基本操作函数
extern void          LCD1602_CheckBusy (void);              // 检查忙标志位 含命令9
extern void          LCD1602_WriteCmd  (unsigned char cmd);	// 写入命令 命令1-8
extern void          LCD1602_WriteData (unsigned char dat);	// 写入数据 命令10
extern unsigned char LCD1602_ReadData  (void);              // 读数据 命令11
// 读数据用的很少，定义宏 LCD1602_NOREADDATA 可以不编译 LCD1602_ReadData(void)

// ------------- 命令封装 --------------

#define LCD1602_Clear() 				LCD1602_WriteCmd(0x01)	// 命令1：清屏
#define LCD1602_CursorReturn()			LCD1602_WriteCmd(0x02)	// 命令2：光标返回0位 0x02-0x03

// 命令3：输入字符后 屏幕/光标 移动
#define LCD1602_Mode_ScreenLeftMove()	LCD1602_WriteCmd(0x05) // 屏幕左移
#define LCD1602_Mode_ScreenRightMove()	LCD1602_WriteCmd(0x07) // 屏幕右移
#define LCD1602_Mode_CursorLeftMove()	LCD1602_WriteCmd(0x04) // 光标自减
#define LCD1602_Mode_CursorRightMove()	LCD1602_WriteCmd(0x06) // 光标自增

// 命令4：控制屏幕和光标状态
#define LCD1602_ScreenOFF() 			LCD1602_WriteCmd(0x08) // 屏幕关闭   0x08-0x0b
#define LCD1602_CursorOFF() 			LCD1602_WriteCmd(0x0c) // 亮屏无光标 0x0c 0x0d
#define LCD1602_CursorON()  			LCD1602_WriteCmd(0x0e) // 亮屏有光标
#define LCD1602_CursorFlicher()			LCD1602_WriteCmd(0x0f) // 亮屏光标闪烁

// 命令5：光标/屏幕 左/右移 延迟
#define LCD1602_ScreenShiftLeft()   	LCD1602_WriteCmd(0x1c) // 屏幕左移 0x1c-0x1f
#define LCD1602_ScreenShiftRight()  	LCD1602_WriteCmd(0x18) // 屏幕右移 0x18-0x1b
#define LCD1602_CursorShiftLeft()   	LCD1602_WriteCmd(0x10) // 光标左移 0x10-0x13
#define LCD1602_CursorShiftRight()		LCD1602_WriteCmd(0x14) // 光标右移 0x14-0x17

// 命令6：设置数据位 显示行数 点阵
#define LCD1602_Set_4bit_1line_5x7()	LCD1602_WriteCmd(0x20) // 0x20-0x23
#define LCD1602_Set_4bit_1line_5x10()	LCD1602_WriteCmd(0x24) // 0x24-0x27
#define LCD1602_Set_4bit_2line_5x7()	LCD1602_WriteCmd(0x28) // 0x28-0x2b
#define LCD1602_Set_4bit_2line_5x10()	LCD1602_WriteCmd(0x2c) // 0x2c-0x2f
#define LCD1602_Set_8bit_1line_5x7()	LCD1602_WriteCmd(0x30) // 0x30-0x33
#define LCD1602_Set_8bit_1line_5x10()	LCD1602_WriteCmd(0x34) // 0x34-0x37
#define LCD1602_Set_8bit_2line_5x7()	LCD1602_WriteCmd(0x38) // 0x38-0x3b
#define LCD1602_Set_8bit_2line_5x10()	LCD1602_WriteCmd(0x3c) // 0x3c-0x3f

// 命令7：设置字库地址 0x00-0x1f
#define LCD1602_SetCGROM(X)       if (0x00<=(X) && (X)<=0x1f) LCD1602_WriteCmd(0x40+(X))

// 命令8：设置光标位置 0x80-0xa7 0xc0-0xe7
#define LCD1602_SetCursorFirstLine(X)  if (0<=(X) && (X)<=40) LCD1602_WriteCmd(0x80|(X))
#define LCD1602_SetCursorSecondLine(X) if (0<=(X) && (X)<=40) LCD1602_WriteCmd(0xc0|(X))

// -------------------------------------

#endif