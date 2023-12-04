/**
 * 作者：李宗霖 日期：2023/12/01
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * - 此文件对DS18B20是不完全封装 并不包含所有操作
 * - DS18B20最基本的三个函数 InitCheck ReadByte WriteByte
 * - 其他函数为二级封装 都是基于三个基本函数的 但是都是跳过选择0xcc
 * - DS18B20对时序敏感 这里封装的是12MHz或者11.0592MHz 提供了6MHz的
 */

#ifndef DS18B20_H
#define DS18B20_H

// 并不需要对外暴露接口 因为这是一个二级封装文件 没有将基本函数作为一级封装
// extern unsigned char DS18B20_InitCheck(void);
// extern unsigned char DS18B20_ReadByte(void);
// extern void DS18B20_WriteByte(unsigned char dat);

// -------------------------------------

extern void DS18B20_Convert (); // 温度转换
extern int  DS18B20_ReadTemp(); // 温度读取

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