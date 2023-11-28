#include "ultimste.h"

#define WELCOME "Welcome to AAUCS"
#define GROUP   "NO.01"
#define GROUP_RIGHT 5
#define GROUP_LEFT  6

#define THIGH "H:"
#define TLOW  "L:"
#define TEMP  "T:"
#define FAN   "FAN:"

uchar fanGear[]    = "0";
uchar tempLow[]    = "   ";
uchar tempHigh[]   = "   ";
uchar temperture[] = "     ";


uint count = 0;

void LCD1602Action(void);
void ShowTemp(uint t);
void ShowTemper(uint t);

void timer0() interrupt 1
{
	++count;
}


void main(void)
{
	//uchar key, sure = 0, enter = 0; // 一些标志
	//DS_Retemp();
	uchar i, t=3;
	while (--t)
	{
		for (i = 4; i; i--);
	}
	DS18B20_Convert();
	
	TMOD = 0x02;
	TH0 = 0x00;
	TL0 = 0x00;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	
	LCD1602Action();	// lcd1602 初始化（开机）
	Delay(1);
	//DS18B20_Save();
	while (1)
	{
		ShowTemper(0);
		//ShowTemp(0);	// 显示温度
		Delay(1);
	} 
	
	/*
	LCD_SetCursorFirstLine(7);
	LCD_ShowString("a:", 100);
	LCD_WriteData('0');
	// LCD_ShowString(number, 0);
	LCD_CursorShiftLeft();
	LCD_CursorFlicher(); // 打开光标闪烁
	while (1)
	{
		key = P3 >> 4;
		switch (key)
		{
			case 0x0e: 				// s4
				Delay(10);			// 去抖
				if (!enter && key == P3 >> 4)
				{	// 锁定当前数字 关闭光标闪烁
					enter = 1;
					sure = 1;
					LCD_CursorOFF();
				}
				break;
			case 0x0d: 				// s3
				Delay(10);
				if (!enter && !sure && key == P3 >> 4)
				{	// 数字0-9循环减1
					enter = 1;
					num = LCD_ReadData();
					LCD_CursorShiftLeft();
					num += num == 48 ? 9 : -1;
					LCD_WriteData(num);
					// number[0] += number[0] == 48 ? 9 : -1;
					//LCD_ShowString(number, 0);
					LCD_CursorShiftLeft();
				}
				break;
			case 0x0b: 				// s2
				Delay(10);
				if (!enter && !sure && key == P3 >> 4)
				{	// 数字0-9循环加1
					enter = 1;
					num = LCD_ReadData();
					LCD_CursorShiftLeft();
					num += num == 57 ? -9 : 1;
					LCD_WriteData(num);
					//number[0] += number[0] == 57 ? -9 : 1;
					//LCD_ShowString(number, 0);
					LCD_CursorShiftLeft();
				}
				break;
			case 0x07: 				// s1
				Delay(100);
				if (!enter && key == P3 >> 4)
				{	// 将a的值清零
					enter = 1;
					sure = 0;
					LCD_CursorFlicher();
					number[0] = 48;
					LCD_ShowString(number, 0);
					LCD_CursorShiftLeft();
				}
				break;
			case 0x0F:
				// 解除按键锁定 每次按下将按键锁定，防止频繁触发按键效果
				enter = 0;
		}
	}
	*/
}

void LCD1602Action(void)
{
	uchar signal[] = " C";
	uchar high, low, i;
	signal[0] = 0xdf;
	LCD1602_Initial(); // 初始化
	// 开机界面
	LCD1602_WriteCmd (Move_Cursor_Row1_Col(16)); // 命令8
	//LCD1602_SetCursorFirstLine(16);		// 设置光标在显示屏之外
	LCD1602_ShowString(WELCOME, 0);
	i = 16 + GROUP_LEFT;
	LCD1602_WriteCmd (Move_Cursor_Row2_Col(i)); // 命令8
	//LCD1602_SetCursorSecondLine(16 + GROUP_LEFT);
	LCD1602_WriteCmd (Mode_ScreenRightMove); // 命令3
	//LCD1602_Mode_ScreenRightMove();		// 一边输出第二行子一边移动屏幕
	LCD1602_ShowString(GROUP, 100);
	LCD1602_WriteCmd (Mode_CursorRightMove); // 命令3
	//LCD1602_Mode_CursorRightMove();		// 恢复光标自增
	LCD1602_ScreenShiftRight(16-GROUP_RIGHT, 100);// 字体移动居中
	LCD1602_ScreenFlicker(3, 500);		// 闪烁三次
	LCD1602_ScreenShiftRight(16, 100);
	LCD1602_WriteCmd (Clear_Screen); 	// 命令1
	Delay(100);
	
	//DS18B20_Set(0x4b, 0x40, 0x8f);
	//DS18B20_Update();
	DS18B20_Get(&high, &low);
	
	// 初始化显示
	LCD1602_WriteCmd (Show_CursorOn); // 命令4
	//LCD1602_CursorON();					// 设置光标
	LCD1602_ShowString(THIGH, 100);
	NumberToString(high, tempHigh, 3, 0);
	LCD1602_ShowString(tempHigh, 100);
	LCD1602_ShowString(signal, 100);
	LCD1602_ShowString("  ", 100);
	LCD1602_ShowString(TLOW, 100);
	NumberToString(low, tempLow, 3, 0);
	LCD1602_ShowString(tempLow, 100);
	LCD1602_ShowString(signal, 100);
	LCD1602_WriteCmd (Move_Cursor_Row2_Col(0)); // 命令8
	//LCD1602_SetCursorSecondLine(0);	
	LCD1602_ShowString(TEMP, 100);
	ShowTemper(100);
	LCD1602_ShowString(signal, 100);
	LCD1602_ShowString("  ", 100);
	LCD1602_ShowString(FAN, 100);
	LCD1602_ShowString(fanGear, 100);
	LCD1602_WriteCmd (Show_CursorOff); // 命令4
	//LCD1602_CursorOFF();
}

//#include <stdio.h>
void ShowTemper(uint t)
{
	int temp;
	while (count < 2700) { } // 27 * 256 = 6912 (7500us)
	LCD1602_WriteCmd (Move_Cursor_Row2_Col(2)); // 命令8
	//LCD1602_SetCursorSecondLine(2);
	temp = DS18B20_ReadTemp();
	DS18B20_Convert();
	count = 0;
	//sprintf(main2, "%.1f", temper * 0.0625);
	NumberToString(temp * 0.0625, temperture, 5, 1);
	LCD1602_ShowString(temperture, t);
}
