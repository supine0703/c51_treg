/**
 * 作者：李宗霖 日期：2023/12/04
 * CSDN 昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * - 此文件对I2C操作的封装 初始化可有可无 预留了写数据和读数据的接口
 * - 此封装是基于12MHz或者11.0592MHz 对于其他的需要求改其中的延迟
 */
#ifndef I2C_H
#define I2C_H

extern void          I2C_Start    (void);
extern void          I2C_Stop     (void);
// extern void          I2C_Init     (void); // 可有可无
extern void          I2C_Ack      (void);
extern void          I2C_NoAck    (void);
extern void          I2C_SendByte (unsigned char dat);
extern unsigned char I2C_RecByte  (void);
extern bit           I2C_CheckAck (void);

#endif // I2C_H