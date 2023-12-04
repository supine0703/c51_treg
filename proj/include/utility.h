/**
 * 作者：李宗霖 日期：2023/12/01
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * 这是一个工具类 其中的函数与C51无关 属于通用C函数 
 */
#ifndef UTILITY_H
#define UTILITY_H

extern void Int8ToString(char num, unsigned char* str, unsigned char length);

extern void FloatToString(
    float num, unsigned char* str, unsigned char length, unsigned char decimal
);

#endif