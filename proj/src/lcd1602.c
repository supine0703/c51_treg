#include "lcd1602.h"

// #define WAIT 1 // 12MHz 72能正常运行 400可以让33MHz正常运行
/**
 * 写数据和写命令完全不需要在最后加上delay，如果连续执行不正常，就是CheckBusy有问题。
 */

// -------------------------------------

void LCD1602_CheckBusy(void)
{
	unsigned char busy;
	LCD1602_DATA = 0xff;
	do
	{	busy = 0xff;
		LCD1602_EN = 0;
		LCD1602_RS = 0;
		LCD1602_RW = 1;
		LCD1602_EN = 1;
		busy = LCD1602_DATA;
	} while (busy & 0x80);
	LCD1602_EN = 0;
}

void LCD1602_Write(bit rs, unsigned char byte);

void LCD1602_WriteCmd(uchar cmd)
{ if (!cmd) return; LCD1602_Write(0, cmd); }

void LCD1602_WriteData(unsigned char dat)
{ LCD1602_Write(1, dat); }

#ifndef LCD1602_NOREADDATA
unsigned char LCD1602_ReadData(void)
{
	unsigned char dat;
	LCD1602_CheckBusy();
	LCD1602_DATA = 0xff;
	LCD1602_RW   = 1;
	LCD1602_RS   = 1;
	LCD1602_EN   = 1;
	dat = LCD1602_DATA;
	LCD1602_EN   = 0;
	return dat;
}
#endif

// -------------------------------------

extern void _nop_(void);

//void LCD1602_Wait(unsigned int t)
//{ do { } while (t--); }

void LCD1602_Write(bit rs, unsigned char byte)
{
	LCD1602_CheckBusy();
	LCD1602_RW   = 0;
	LCD1602_RS   = rs; 	// 0 写命令  1 写数据
	LCD1602_DATA = byte;
	LCD1602_EN   = 1; 	// LCD1602_EN: 0 -> 1
	_nop_();
	LCD1602_EN   = 0;
	//LCD1602_Wait(WAIT);
}

// -------------------------------------
