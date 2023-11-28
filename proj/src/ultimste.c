#include "ultimste.h"

// ============== LCD1602 ==============

void LCD1602_Initial(void)
{
	LCD1602_WriteCmd (Set_8bit_2line_5x7);   // 命令6
	LCD1602_WriteCmd (Show_CursorOff);       // 命令4
	LCD1602_WriteCmd (Mode_CursorRightMove); // 命令3
	LCD1602_WriteCmd (Clear_Screen);         // 命令1			
}

void LCD1602_ShowString(uchar* s, uint t)
{
	while (*s)
	{
		LCD1602_WriteData(*s++);
		Delay(t);
	}
}

void LCD1602_ScreenFlicker(uchar num, uint t)
{
	t >>= 1;
	while (num--)
	{
		LCD1602_WriteCmd (Show_ScreenOff); // 命令4
		Delay(t);
		LCD1602_WriteCmd (Show_CursorOff); // 命令4
		Delay(t);
	}
}

void LCD1602_ScreenShiftRight(uchar num, uint t)
{
	while (num--)
	{
		LCD1602_WriteCmd (Shift_ScreenRight); // 命令5
		Delay(t);
	}
}

// -------------------------------------

void NumberToString(float num, uchar* str, uchar numLen, uchar decimal)
{
	char i, mid;
	uint intn, decn;
	float dec, std = 0.4;
	
	// 确定小数点的位置
	if (decimal != 0) 
    {
        if (decimal + (num<0) + 1 > numLen)
        {
            mid = num < 0;
            decimal = numLen - 1 - (num < 0);
        }
        else mid = numLen - decimal - 1;
		str[mid] = '.';
	}
	else mid = numLen;
	
	// 五舍六入
	i = decimal;
	while (i--) std *= 0.1;
	
	// 分离整数和小数部分
	if (num < 0) 
    {
		num -= std;
		intn = -(int)num;
		dec = (int)num - num;
	}
	else 
	{
		num += std;
		intn = (int)num;
		dec = num - intn;
	}
	
	// 处理小数部分
	for (i = mid + 1; i < numLen; ++i)
    {
		dec *= 10;
		decn = (uchar)dec;
        str[i] = decn + '0';
		dec -= decn;
	}
	// 处理整数部分
	for (i = mid - 1; i > 0; --i)
    {
        str[i] = intn % 10 + '0';
        intn /= 10;
		if (intn == 0)
        {
            --i;
            break;
        }
    }
	str[i] = num < 0 ? '-' : (intn == 0 && mid != 1 ? ' ' : intn % 10 + '0');
    while (i-- > 0) str[i] = ' ';
}
