/**
 * 作者：李宗霖 日期：2023/12/01
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * 此处为针对与本项目 基于ds18b20 lcd1602 i2c基本函数的复杂函数封装
 */
#ifndef ULTIMATE_H
#define ULTIMATE_H

extern void Delay1ms(unsigned int t);

extern void LCD1602_ShowString(unsigned char* s); // 显示字符串
extern void LCD1602_Action(void); // 开机 并显示开机画面

extern void ShowViewPage_1(void); // 主视图 (温度信息查询视图)
extern void ShowViewPage_2(void); // 最高/最低温(温度极值)查询视图
extern void ShowViewPage_3(void); // 温度越界计时视图
extern void ShowViewPage_4(void); // 设置查询视图
extern void ShowSettings(unsigned char opt); // 设置模式显示

extern void KeysSystem_1(void);
extern void KeysSystem_2(void);

extern void AboveLimitClock(void); // 上越界计时
extern void BelowLimitClock(void); // 下越界计时

extern void UpdateOverLimitTimer(bit which); // 更新越界的定时值
extern void UpdateExtremes(bit which); // 更新最高/最低温度值(极值)

extern void ReadMusic(void); // 读取音乐

#endif // ULTIMATE_H
/**
 * 三套按键系统
 * 第一套: 视图模式下 全部上升沿(松开按键时)触发
 *   S1: 主视图 (温度信息查询视图)
 *   S2: 最高/最低温(温度极值)查询视图
 *   S3: 温度越界计时视图
 *   S4: 设置查询视图
 *
 * 第二套: 设置模式下 全部上升沿(松开按键时)触发
 *   S1: 修改当前选项值
 *   S2: 下一项
 *   S3: 上一项
 *   S4: 回到第一项
 *
 * 第三套: 修改选项时
 *   S1: 取消 上升沿触发
 *   S2: 加   低电平触发 按下触发一次 长按超过阈值 每隔一段时间间隔触发
 *   S3: 减   低电平触发 按下触发一次 长按超过阈值 每隔一段时间间隔触发
 *   S4: 确认 上升沿触发
 */
