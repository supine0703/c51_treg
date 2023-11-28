#ifndef DS18B20_H
#define DS18B20_H

#include "__config__.h"

#ifndef uchar
#define uchar unsigned char
#endif

bit           DS18B20_InitCheck (void);
void          DS18B20_WriteByte (unsigned char dat);
unsigned char DS18B20_ReadByte  (void);


// -------------------------------------

void DS18B20_Convert(); 	//温度转换
int  DS18B20_ReadTemp();	//温度读取

extern void DS18B20_Set(unsigned char temp_low, unsigned char temp_high, unsigned char accuracy);
extern void DS18B20_Get(unsigned char* temp_low, unsigned char* temp_high);
extern void DS18B20_Save();
extern void DS18B20_Update();

#endif // DS18B20_H