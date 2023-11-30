/**
 * 在此添加共享的全局变量
*/
#define uint  unsigned int
#define uchar unsigned char

// 模式状态 自上往下 优先级递减
bit setting = 0;  // 是否是设置模式
bit simulate = 0; // 是否是虚拟模式

// 存储摄氏度 字符集的 '°' ASCII 和标准集不一样
uchar DC[] = {0xdf, 0x43, 0}; // °C

// 定义不同长度字符串方便打印
uchar str1[] = " ";
uchar str2[] = "  ";
uchar str3[] = "   ";
uchar str5[] = "     ";

// 通过全局设置显示等待(间隔)时间
uint SHOW_WAIT = 0;

// ========= 以下为视图模式下需要使用的变量 ===========

// 第一页变量
char upperLimit = 128; // 温度上限
char lowerLimit = -55; // 温度下限
float temp = 80.5; // 温度
uchar fanGear = 0; // 风扇档位

// 第二页变量
float highest = -55; // 开机后最高温
float lowest = 128; // 开机后最低温

// 第三页变量
uchar hms = 0, hs = 0, hm = 0; // 开机后 超过温度上限 时间
uchar lms = 0, ls = 0, lm = 0; // 开机后 低于温度下限 时间

uchar fanGearStep = 0;

// ===============================================

// 视图模式下按键、翻页操作
uchar key = 0xf;
uchar pressKey = 0xff;
uchar page = 0x7f;
bit pageChange = 0;

// 定时器 3.6次/ms 分辨率对应需要的转换时间: 0:37.5 1:675 2:1350 3:2700
uchar dsr = 0x03; // ds18b20 resolution (11.0592MHz)
uint ctc[] = {337, 675, 1350, 2700}; // convert timer count

uchar ringtone = 1;
uchar volume = 7;

uchar option = 0xff;
bit readySetting = 0;
uchar changeCount = 0;
