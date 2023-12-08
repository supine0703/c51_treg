/**
 * 作者：李宗霖 日期：2023/12/01
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * 不加入结束符就可以插入别的字符串中间
 * 但是对于单独开空间存储的来说 要么不同长度各自开空间 要么手动加结束符
 * 所以出于妥协 添加一个宏来作为编译的这一份代码是否需要自动加入结束符
 */
#define uchar unsigned char
#define uint unsigned int

#define _ADD_END_

/**
 * @param num 需要转换的数字
 * @param str 存储字符串的首地址
 * @param length 转换后数字可以存放的空间长度
 * @param decimal 小数占多少位
 */
#include <stdio.h>
void FloatToString(float num, uchar* str, uchar length, uchar decimal)
{
    char i, mid;
    uint intn, decn;
    float dec, std;

#ifdef _ADD_END_
    str[length] = 0; // 字符串结束标志位
#endif

    std = 0.4;
    // 确定小数点的位置
    if (decimal != 0)
    {
        if (decimal + (num < 0) + 1 > length)
        {
            mid = num < 0;
            decimal = length - 1 - (num < 0);
        }
        else
            mid = length - decimal - 1;
        str[mid] = '.';
    }
    else
        mid = length;

    // 五舍六入
    i = decimal;
    while (i--)
        std *= 0.1;

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
    for (i = mid + 1; i < length; ++i)
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
    while (i-- > 0)
        str[i] = ' ';
}

void Int8ToString(char num, uchar* str, uchar length)
{
    bit neg;

#ifdef _ADD_END_
    str[length] = 0; // 字符串结束标志位
#endif

    neg = num < 0;
    if (!length)
        return;
    if (neg)
    {
        if (!--length)
            return;
        ++str;
        num = ~num;
        ++num;
    }
    str[--length] = num % 10 + '0';
    while (length)
    {
        num /= 10;
        if (!num)
            break;
        str[--length] = num % 10 + '0';
    }
    if (neg)
    {
        --str;
        if (!length)
            *str = '-';
        str[length] = '-';
    }
    while (length)
        str[--length] = ' ';
}
