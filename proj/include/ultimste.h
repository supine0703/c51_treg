#ifndef ULTIMSTE_H
#define ULTIMSTE_H

#include "lcd1602.h"
#include "ds18b20.h"

void LCD1602_Initial(void);
void LCD1602_ShowString(uchar *s, uint t);
void LCD1602_ScreenFlicker(uchar num, uint t);
void LCD1602_ScreenShiftRight(uchar num, uint t);

void NumberToString(float num, uchar* str, uchar numLen, uchar decimal);

#endif // ULTIMSTE_H