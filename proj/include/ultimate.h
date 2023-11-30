#ifndef ULTIMATE_H
#define ULTIMATE_H

#include "utility.h"
#include "lcd1602.h"
#include "ds18b20.h"

extern void Delay1ms(uint t);

extern void LCD1602_ShowString(uchar *s);
extern void LCD1602_Action(void);
extern void ViewPage_1(void);
extern void ViewPage_2(void);
extern void ViewPage_3(void);
extern void ViewPage_4(void);

extern void Settings(uchar opt);

extern void KeysSystem_1(void);
extern void KeysSystem_2(void);

#endif // ULTIMATE_H