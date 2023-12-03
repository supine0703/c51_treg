/**
 * 这是一个工具类 其中的函数与C51无关 属于通用C函数
*/

#ifndef UTILITY_H
#define UTILITY_H

extern void Int8ToString(char num, unsigned char* str, unsigned char length);

extern void FloatToString(
    float num, unsigned char* str, unsigned char length, unsigned char decimal
);

#endif