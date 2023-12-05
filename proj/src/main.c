/**
 * main.c
 * 作者：李宗霖 日期：2023/12/01
 * CSDN昵称：Leisure_水中鱼
 * CSDN: https://blog.csdn.net/Supine_0?type=blog
 * ----------------------------------------------
 * 程序的main 主要分为三个部分 初始化部分 主循环部分 中断部分
 */
#include "__config__.h"
#include "at24c02.h"
#include "ds18b20.h"
#include "i2c.h"
#include "lcd1602.h"
#include "ultimate.h"
#include "utility.h"
#include "music.h"

#define uint unsigned int
#define uchar unsigned char

extern bit page_change;
extern bit settings_mode;
extern bit ready_settings;
extern bit convert_finished;
extern bit dc_motor_working;
extern bit above_upper_limit;
extern bit below_lower_limit;
extern bit ringtone_change;
extern bit save_in_24c02;

extern float temperature, highest, lowest;
extern uchar idata settingsSave;
extern uchar page, option;
extern uchar hus, hms, hs, hm, lus, lms, ls, lm;
extern uchar dsr, fanGear, fanGearStep;
extern uchar audio, ringtoneNum;
extern char upperLimit, lowerLimit;

extern uchar numStr[];
extern uint code cttcn[];
extern uint convertCount, dcmCount;
extern uint SHOW_WAIT;

sbit RELAY = DEFINE_RELAY; // 继电器
sbit DCM = DEFINE_DCM;     // 直流电机
sbit Buzzer = DEFINE_BUZZRT;

void init_data(void);          // 初始化数据
void init_program(void);       // 初始化程序
void UpdateTemperature(void);  // 更新温度信息
void UpdateViewPageShow(void); // 刷新视图显示

void main(void)
{
    /**
     * 初始化数据:
     * 1. 初始化 定时/计数器 对应的方式初值 优先级
     * 2. 初始化 外部中断 对应的方式 初值 优先级
     * 3. 从 DS18B20 读取 温度上限 下限 分辨率
     * 4. 从 24LC02  读取 ...
     */
    init_data();

    /**
     * 初始化程序:
     * 1. 开始温度转换
     * 2. LCD1602初始化 显示开机界面
     * 3. 以打字机效果显示主视图
     * 4. 打开 定时/计数器中断 外部中断
     */
    init_program();

    /**
     * 程序的主循环:
     * 1. 判断是哪一种模式并执行模式对应的程序
     * 2. 模式主程序:
     * - 设置模式
     *
     * - 视图模式
     *   1. 如果温度转换完成 更新温度信息
     *   2. 判断当前处于哪一个视图
     *   3. 如果视图发生改变 需要更新整个视图
     *   4. 如果视图没有改变 刷新当前视图的 可变量
     *   5. 以 按键系统1 接收按键操作 并反应
     *
     * 外部中断:
     * X0: 长按切换设置模式和视图模式
     */
    while (1)
    {
        if (settings_mode) // 设置模式
        {
            if (ready_settings)
            {
                ready_settings = 0;
                ShowSettings(0); // 显示设置模式 并指向第一条
            }
            KeysSystem_2(); // 第二套按键事件响应系统
        }
        else // 视图模式
        {
            if (ringtone_change)
            {
                // 从第二字节开始读取铃声 放入 ringtone
                // ringtone[127]
                // ...
            }
            if (save_in_24c02)
            {
                save_in_24c02 = 0;
                At24c02_WriteByte(0xa0, 0x00, &settingsSave, 1);
            }
            if (convert_finished)
            { // 如果温度转换完成 更新温度信息
                UpdateTemperature();
                convert_finished = 0;
            }
            UpdateViewPageShow(); // 刷新视图显示
            KeysSystem_1();       // 第一套按键事件响应系统
        }
    }
    
}

/**
 * T0 中断函数
 * 设定:
 *     定时器 T0 方式2 初值 0 计数256溢出一次
 *     所以 定时器中断内 指令周期总和需要少于 250 个机器周期
 *     在 定时器中断函数内 获取温度等复杂函数 会严重破坏 T0 产生的时序
 * 思路:
 *     在中断函数内通过设置标志位 让复杂的函数逻辑在主循环中执行
 * 理念:
 *     在 11.0592MHz下  每 1/3.6 ms 溢出一次 即中断36次为 10ms
 *     所以 T0 可以作为 1/3.6 ms 的时序产生器
 *     通过 计数变量 count 即可实现不同周期的定时
 */
void int_T0() interrupt 1
{
    // 根据分辨率调整温度转换需要的时间
    if (!convert_finished && ++convertCount >= cttcn[dsr])
    {
        convertCount = 0;
        convert_finished = 1;
    }
    // 将直流电机的方波分成 3段 根据档位决定某一段 电平高低
    if (dc_motor_working)
        if (++dcmCount >= 1440)       // 0.4s
            if (dcmCount >= 2520)     // 0.7s
                if (dcmCount >= 3600) // 1.0s
                    dcmCount = 0;
                else // 0.7s - 1.0s
                    DCM = fanGear >= 0x03;
            else // 0.4s - 0.7s
                DCM = fanGear >= 0x02;
        else // 0.0s - 0.4s
            DCM = fanGear >= 0x01;
    else
        DCM = 0;
    // 过界定时 需要 32个机器周期
    if (above_upper_limit)
        AboveLimitClock();
    else if (below_lower_limit)
        BelowLimitClock();
}

/**
 * X0 中断函数
 * 设定:
 *     外部中断0 为低优先级 可以被定时器中断 给中断
 *     否则 外部中断会破坏 T0 产生的时序
 *     设置模式下会关闭定时器 因为没有继续定时的必要
 * 思路:
 *     外部中断通过软件延迟 每0.05s判断一次是否仍然处于 按下且仅按下 INT0
 状态
 *     当按下持续时间超过给定阈值将会 切换设置/视图模式 并执行必要的
 初始化/收尾
 * 工作
 */
void int_X0() interrupt 0
{
    uchar ky; // 临时用来接收按键操作
    uchar i = 20;
    KEYS = 0xff;
    do
    {
        ky = 0x03;
        ky |= KEYS;
        if (ky != 0xfb)
        {
            Delay1ms(10);
            ky = 0x03;
            ky |= KEYS;
            if (ky != 0xfb)
                return;
        }
        Delay1ms(50);
    } while (--i);
    if (settings_mode) // 退出设置模式
    {
        // 将设置的内容存储至 DS18B20
        DS18B20_Set(upperLimit, lowerLimit, dsr);
        DS18B20_Save();
        // 将设置的内容存储至 24lc02
        settingsSave = 0xff;
        settingsSave &= (fanGearStep << 5) | (ringtoneNum << 3) | (audio);
        /**
         * @bug 不知道为什么 只要在此放下
         * AT24C02_WriteData(0xa0,0x00,&settingsSave,1) / Byte(...)
         * 就会在触发外部中断时导致死循环/崩溃，哪怕 if(0)
         * 没有调用此函数 仅仅是存在就会导致如此 因此作为妥协
         只好设置一个标志位
         * 在主循环中进行存储
         */
        save_in_24c02 = 1;
        // ... ...
        DS18B20_Convert();
        convertCount = 0;
        convert_finished = 0; // 重新打开温度转换
        TR0 = 1;              // 重启定时器开始测温
        option = 0xff;        // 设置模式 不选择
        page_change = 1;      // 需要刷新整个视图显示
    }
    else // 进入设置模式
    {
        RELAY = 0; // 断开继电器
        DCM = 0;   // 关闭直流电机
        TR0 = 0;   // 关闭定时计器T0
        // 得从主函数中调用 否则可能会发生形参被出错 产生不可预料问题
        // 可以设定一个标志 让主函数调用一次 ready_settings
        ready_settings = 1; // 进入设置模式 刷新设置模式显示
    }
    settings_mode = !settings_mode;
}


void int_T1() interrupt 3
{
    // ... audio 音频变量
    if(FreqTable[FreqSelect])	//如果是休止符(0)，那么不播放声音，只进行延时
	{
		//取对应频率值的重装载值到定时器(确认音高)FreqSelect=Music[MusicSelect]
		TL0 = FreqTable[FreqSelect]%256;		//设置低位定时初值
		TH0 = FreqTable[FreqSelect]/256;		//设置高位定时初值
		Buzzer=!Buzzer;	//翻转蜂鸣器IO口(注意这里的重装值是周期的一半，故仅进行一次蜂鸣器的翻转)
	}
}

void init_data(void)
{
    DCM = 0;     // 初始化电机不工作
    RELAY = 0;   // 初始化继电器断开
    TMOD = 0x02; // 定时器0 方式2
    TH0 = 0x00;  // 自动装填
    TL0 = 0x00;  // 记 256 次

    IT0 = 1; // 下降沿触发
    PX0 = 0; // 低优先级
    PT0 = 1; // 高优先级

    // 从 DS18B20 读取 温度上下限 分辨率
    // DS18B20_Update();
    DS18B20_Get(&upperLimit, &lowerLimit, &dsr);
    // 从 24c02 读取 风扇档位步长 开机音乐序号 音频(分为0-7)
    I2C_Init();
    At24c02_ReadData(0xa0, 0x00, &settingsSave, 1);
    audio = settingsSave & 0x07;
    ringtoneNum = (settingsSave >> 3) & 0x03;
    fanGearStep = (settingsSave >> 5) & 0x03;
    // 从 24c02 读取 铃声 放入ringtone
    // ...
}

void init_program(void)
{
    DS18B20_Convert();                // 开始温度转换
    LCD1602_Action();                 // lcd1602 初始化（开机）
    UpdateTemperature();              // 更新温度信息
    LCD1602_WriteCmd(Show_CursorOn);  // 打开光标
    SHOW_WAIT = 40;                   // 开机打字机特效
    ShowViewPage_1();                 // 显示首页
    LCD1602_WriteCmd(Show_CursorOff); // 关闭光标
    SHOW_WAIT = 0;
    DS18B20_Convert();    // 开始温度转换
    convert_finished = 0; // 打开温度转换定时
    EA = 1;               // 总中断允许开启
    ET0 = 1;              // 允许定时器中断
    TR0 = 1;              // T0 开始工作
    EX0 = 1;              // 允许外部中断
}

void UpdateTemperature(void)
{
    uchar i;
    EA = 0; // 获取温度转化得关闭中断 否则会破坏 DS18B20 的时序 造成错误
    temperature = DS18B20_ReadTemp(); // 获取温度计转换的温度
    DS18B20_Convert();
    i = 35;
    do
    {
    } while (--i); // 85个机器周期 共约 2+1+5815+70 个机器周期
    EA = 1; // 假设触发定时中断 23 次 可能有一些误差 但非常小
    temperature *= 0.0625; // 转化为可读温度
    // 更新温度最大最小值
    if (temperature > highest)
        highest = temperature;
    if (temperature < lowest)
        lowest = temperature;
    // 比较温度是否越界 并采取措施
    if (temperature > upperLimit) // 高于温度上限
    {
        above_upper_limit = 1; // 设置上越界标志位
        dc_motor_working = 1;  // 直流电机开始工作
        fanGear = (temperature - upperLimit) / fanGearStep + 1;
        if (fanGear > 3)
            fanGear = 3;
        i = 23;
        do
        {
            AboveLimitClock(); // 上越界定时
        } while (--i);
    }
    else // 温度正常
    {
        above_upper_limit = 0; // 上越界标志位清0
        dc_motor_working = 0;  // 直流电机停止工作
        fanGear = 0;           // 直流电机档位置0
    }
    if (temperature < lowerLimit) // 低于温度下限
    {
        below_lower_limit = 1; // 设置下越界标志位
        RELAY = 1;             // 闭合继电器
        i = 21;
        do
        {
            BelowLimitClock(); // 下越界定时
        } while (--i);
    }
    else // 温度正常
    {
        RELAY = 0;             // 断开继电器
        below_lower_limit = 0; // 下越界标志位清0
    }
}

void UpdateViewPageShow(void)
{
    switch (page) // 判断当前处于哪一个视图
    {
    case 0x7f: // 主视图 (温度信息查询视图)
        if (page_change)
        { // 如果视图改变 刷新整个屏幕内容显示
            ShowViewPage_1();
            page_change = 0;
        }
        // 刷新温度值显示
        LCD1602_WriteCmd(Move_Cursor_Row2_Col(2));
        FloatToString(temperature, numStr, 5, 1);
        LCD1602_ShowString(numStr);
        // 刷新风扇档位显示
        LCD1602_WriteCmd(Move_Cursor_Row2_Col(15));
        Int8ToString(fanGear, numStr, 1);
        LCD1602_ShowString(numStr);
        break;
    case 0xbf: // 最高/最低温(温度极值)查询视图
        if (page_change)
        { // 如果视图改变 刷新整个屏幕内容显示
            ShowViewPage_2();
            page_change = 0;
        }
        // 刷新显示温度的极值
        LCD1602_WriteCmd(Move_Cursor_Row1_Col(9));
        UpdateExtremes(1);
        LCD1602_WriteCmd(Move_Cursor_Row2_Col(9));
        UpdateExtremes(0);
        break;
    case 0xdf: // 温度越界计时视图
        if (page_change)
        { // 如果视图改变 刷新整个屏幕内容显示
            ShowViewPage_3();
            page_change = 0;
        }
        // 如果温度高于上限 刷新显示上越界计时
        if (above_upper_limit)
        {
            LCD1602_WriteCmd(Move_Cursor_Row1_Col(8));
            UpdateOverLimitTimer(1);
        }
        // 如果温度低于下限 刷新显示下越界计时
        else if (below_lower_limit)
        {
            LCD1602_WriteCmd(Move_Cursor_Row2_Col(8));
            UpdateOverLimitTimer(0);
        }

    case 0xef: // 设置查询视图
        if (page_change)
        { // 如果视图改变 刷新整个屏幕内容显示
            ShowViewPage_4();
            page_change = 0;
        }
        // 没有需要刷新的可变量
        break;
    }
}

/*
 * 为了测时I2C 24C02
extern void _nop_(void);
uchar code aa[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
uchar idata bb[] = "                                                        ";
extern void Delay1ms(uint t);
extern uint SHOW_WAIT;

void main(void)
{
    uchar i, j;
    LCD1602_WriteCmd(Set_8bit_2line_5x7); // 命令6
    LCD1602_WriteCmd(Show_CursorOn);      // 命令4
    LCD1602_WriteCmd(Mode_CursorRightMove);
    LCD1602_WriteCmd(Clear_Screen); // 命令1
    // I2C_WriteData(0xa0, 0x00, aa, 26);
    I2C_Init();
    // I2C_WriteData(0xa0, 0x80, aa, 53);
    // At24c02_WriteByte(0xa0, 0x00, aa, 8);
    At24c02_WriteData(0xa0, 0xb0, aa, 53);
    SHOW_WAIT = 100;

    // LCD1602_ShowString(aa);
    At24c02_ReadData(0xa0, 0xb0, bb, 53);
    i = 53;
    while (1)
    {
        LCD1602_WriteCmd(Move_Cursor_Row1_Col(0));
        for (j = 16; j && i; --j)
        {
            LCD1602_WriteData(bb[--i]);
            Delay1ms(100);
        }
        if (!i)
            break;
    }
    // // LCD1602_WriteCmd(Return_Cursor);
    // // LCD1602_WriteCmd(Move_Cursor_Row2_Col(15));
    // // LCD1602_ShowString(bb);
    while (1)
        ;
}
*/
