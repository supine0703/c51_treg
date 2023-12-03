#ifndef I2C_H
#define I2C_H

#include "__config__.h"

extern void I2C_Init(void);
extern bit I2C_WriteData(uchar sla, uchar suba, uchar* dat, uchar num);
extern bit I2C_ReadData(uchar sla, uchar suba, uchar* dat, uchar num);

#endif // I2C_H