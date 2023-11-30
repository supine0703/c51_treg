#include "ultimate.h"

uint count;

extern bit setting;
extern bit simulate;

extern uchar str5[];
extern float temp;
extern uint SHOW_WAIT;

extern char upperLimit;
extern char lowerLimit;

extern float highest;
extern float lowest;

extern uchar page;
extern bit pageChange;

extern uchar dsr;
extern uint ctc[];

extern uchar option;
extern bit readySetting;

void init_data(void);
void init_program(void);

void main(void)
{
    init_data();
    init_program();

    while (1)
    {
        if (setting)
        {
            if (readySetting)
            {
                readySetting = 0;
                Settings(0);
            }
            KeysSystem_2();
            continue;
        }
        if (pageChange)
        {
            switch (page)
            {
            case 0x7f: // 首页
                ViewPage_1();
                break;
            case 0xbf: // 统计1
                ViewPage_2();
                break;
            case 0xdf: // 统计2
                ViewPage_3();
                break;
            case 0xef: // 设置查询
                ViewPage_4();
                break;
            }
            pageChange = 0;
        }
        switch (page)
        {
        case 0x7f: // 首页
            LCD1602_WriteCmd(Move_Cursor_Row2_Col(2));
            FloatToString(temp, str5, 5, 1);
            LCD1602_ShowString(str5);
            break;
        case 0xbf: // 统计1
            LCD1602_WriteCmd(Move_Cursor_Row1_Col(9));
            FloatToString(highest, str5, 5, 1);
            LCD1602_ShowString(str5);
            LCD1602_WriteCmd(Move_Cursor_Row2_Col(9));
            FloatToString(lowest, str5, 5, 1);
            LCD1602_ShowString(str5);
            break;
        case 0xdf: // 统计2
                   // ViewPage_3();
        case 0xef: // 设置查询
            break;
        }
        KeysSystem_1();
    }
}

void int_T0() interrupt 1
{
    if (++count >= ctc[dsr]) // 2700
    {
        count = 0;
        temp = DS18B20_ReadTemp();
        temp *= 0.0625;
        if (temp > highest)
            highest = temp;
        if (temp < lowest)
            lowest = temp;
        DS18B20_Convert();
    }
}

void int_X0() interrupt 0
{
    uchar ky;
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
    if (setting) // 退出设置
    {
        // 将设置的内容进行存储 DS18B20
        DS18B20_Set(upperLimit, lowerLimit, dsr);
        DS18B20_Save();
        // 24lc02
        TR0 = 1; // 重启定时器开始测温
        count = 0;
        DS18B20_Convert();
        option = 0xff;
        pageChange = 1;
    }
    else // 进入设置
    {
        TR0 = 0; // 关闭定时计器T0
        // 得从主函数中调用 否则可能会发生形参被出错 产生不可预料问题
        // 可以设定一个标志 让主函数调用一次 readySetting
        // Settings(0);
        readySetting = 1;
    }
    setting = !setting;
}

void init_data(void)
{
    TMOD = 0x02; // 定时器0 方式2
    TH0 = 0x00;  // 自动装填
    TL0 = 0x00;  // 记 256 次

    IT0 = 1; // 下降沿触发
    IT1 = 1;
    PX0 = 0; // 低优先级
    PX1 = 0;
    PT0 = 1; // 高优先级

    // 从 DS18B20 读取 温度上下限 分辨率
    DS18B20_Get(&upperLimit, &lowerLimit, &dsr);
    // 从 at24c02/24lc02 读取 风扇档位步长 开机音乐序号 音量(分为0-7) 3+2+3 =
    // 1byte
}

void init_program(void)
{
    DS18B20_Convert();
    LCD1602_Action();                // lcd1602 初始化（开机）
    EA = 1;                          // 总中断允许打开
    ET0 = 1;                         // 允许定时器中断
    TR0 = 1;                         // 开启定时计器T0
    count = ctc[dsr] - 5;            // 尽快进入中断
    LCD1602_WriteCmd(Show_CursorOn); // 打开光标
    SHOW_WAIT = 40;                  // 开机打字机特效
    ViewPage_1();                    // 显示首页
    LCD1602_WriteCmd(Show_CursorOff);
    SHOW_WAIT = 0;
    EX0 = 1; // 允许外部中断

    // EX1 = 1;
}