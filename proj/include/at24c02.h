/**
 * 作者：李宗霖 日期：2023/12/04
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * - 值得注意的是 写入数据后不可立马读取数据 (24c02)不会应答
 * - 因为写入数据需要一定时间 请注意 例如at24c02进行写字节操作不可跨页读写
 * - 因此特意封装了连续页写入函数 连续页写入函数可以确保数据写入24c02中
 * - 不必自己设置延迟
 */
#ifndef AT24C02_H
#define AT24C02_H

/**
 * @param sla 器件地址码
 * @param suba 器件单元地址
 * @param dat 数据的首地址
 * @param num 数据的个数
 * @return 0: 未完成 中间某个环节未应答  1: 完成
 */
extern bit At24c02_ReadData(
    unsigned char sla, unsigned char suba, unsigned char* dat, unsigned char num
); // 可以读任意范围内长度的数据

extern bit At24c02_WriteByte(
    unsigned char sla, unsigned char suba, unsigned char* dat, unsigned char num
); // 一次最多写入8字节 多了会循环覆盖掉 需要自己添加延迟

extern bit At24c02_WriteData(
    unsigned char sla, unsigned char suba, unsigned char* dat, unsigned char num
); // 可以写入任意多范围内的数据 内置延迟 可以连续操作

#endif // AT24C02_H