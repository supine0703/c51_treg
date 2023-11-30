#define uchar unsigned char
#define uint  unsigned int

// @param num: 数字  str: 存储地址  length: 数据可存储大小  decimal: 小数位数
void FloatToString(float num, uchar* str, uchar length, uchar decimal)
{
    char i, mid;
    uint intn, decn;
    float dec, std = 0.4;

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
    bit neg = num < 0;
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
