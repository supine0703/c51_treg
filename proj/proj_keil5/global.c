/** 在此添加共享的全局变量 */
#define uint unsigned int
#define uchar unsigned char

// ==================== 标志位 ====================

/** 约定: 标志位采用下划线方式命名 */
// 模式状态标志位 自上往下 优先级递减 (可扩充)
bit settings_mode = 0; // 当前是设置模式
// ... ...

// 行动标志位
bit page_change = 0;       // 视图模式 视图发生改变
bit ready_settings = 0;    // 将会进入设置模式
bit convert_finished = 1;  // 温度传感器完成温度转化
bit dc_motor_working = 0;  // 直流电机需要工作
bit above_upper_limit = 0; // 温度高于上限阈值
bit below_lower_limit = 0; // 温度低于下限阈值

// ==================== ===== ====================

// ==================== 用于计算时间戳的计数变量 ====================

uint dcmCount = 0;     // 用于分割直流电机方波
uint convertCount = 0; // 用于计算温度传感器转化时间
uchar changeCount = 0; // 用于计算长按 持续增加/减少 间隔

// ==================== ==================== ====================

// ==================== 模式需要用到的变量 ====================

uchar page = 0x7f;   // 用于视图模式选择不同视图
uchar option = 0xff; // 用于设置模式选择不同项

// 视图模式 主视图  设置模式 第 1 2 _ _ 项
char upperLimit = 128;    // 温度上限
char lowerLimit = -55;    // 温度下限
float temperature = 80.5; // 温度
uchar fanGear = 0;        // 风扇档位

// 视图模式 温度极值查询视图
float highest = -55; // 开机后最高温
float lowest = 128;  // 开机后最低温

// 视图模式 温度过界计时视图
uchar hus = 0, hms = 0, hs = 0, hm = 0; // 开机后 超过温度上限 时间
uchar lus = 0, lms = 0, ls = 0, lm = 0; // 开机后 低于温度下限 时间

// 设置模式 第 4 项
uchar fanGearStep = 2; // 风扇/直流电机档位步长

// 设置模式 第 3 _ 项 在 11.0592MHz 下 分辨率对应最大转换时间需要 T0 的定时次数
uchar dsr = 0x03; // ds18b20 resolution 温度传感器分辨率
uint code cttcn[] = {337, 675, 1350, 2700}; // convert temperature timer count num

// 设置模式 第 5 6 项
uchar ringtone = 1; // 铃声序号
uchar volume = 7;   // 音量大小

// ==================== =============== ====================

// ==================== 其他的 ====================

// 存储摄氏度 字符集的 '°' ASCII 和标准集不一样
uchar code DC[] = {0xdf, 0x43, 0}; // °C

// 定义一个最大长度的字符串用作打印的空间
uchar numStr[] = "     ";

// 通过全局设置显示等待(间隔)时间
uint SHOW_WAIT = 0;

// 用于接收按键操作的变量
uchar key = 0xf;
uchar pressKey = 0xff;

// ==================== ===== ====================


