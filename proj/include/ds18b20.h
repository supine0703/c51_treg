/**
 *
 */

#ifndef DS18B20_H
#define DS18B20_H

#include "__config__.h"

extern unsigned char DS18B20_InitCheck(void);
extern unsigned char DS18B20_ReadByte(void);
extern void DS18B20_WriteByte(unsigned char dat);

// -------------------------------------

extern void DS18B20_Convert(); // 温度转换
extern int DS18B20_ReadTemp(); // 温度读取

extern void DS18B20_Set(
    unsigned char upperLimit, unsigned char lowerLimit, unsigned char resolution
);
extern void DS18B20_Get(
    unsigned char* upperLimit,
    unsigned char* lowerLimit,
    unsigned char* resolution
);
extern void DS18B20_Save(void);

// extern void DS18B20_Update(void);

// extern char DS18B20_Mode(void);

#endif // DS18B20_H