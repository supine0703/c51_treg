#include "at24c02.h"
#include "music.h"

extern unsigned int Speed;
extern unsigned int MusicSelect, MusicSelect;
extern unsigned char idata Music_arry[128];
extern unsigned int code Music[];
extern unsigned int code FreqTable[];

void TimerInit(void)
{
    TMOD = 0x12; // 设置定时器模式

    TH0 = 0x00; // 自动装填
    TL0 = 0x00; // 记 256 次

    TL1 = 0x18; // 设置定时初值
    TH1 = 0xFC; // 设置定时初值

    PT0 = 0; // 高优先级
    PT1 = 1;

    EA = 1;
    ET0 = 1;
    ET1 = 1;

    TF0 = 0;
    TF1 = 0; // 清除TF1标志
    TR0 = 1;
    TR1 = 1; // 定时器0开始计时
}

unsigned int delay = 0;
unsigned int space = 0;
unsigned char freqH, freqL;

void main()
{
    unsigned char music_num, startAddr, musicLen;
    Music_score();
	
    music_num = 0;
    At24c02_ReadData(0xa0, music_num + 0x03, &startAddr, 1);
    At24c02_ReadData(0xa0, music_num + 0x04, &musicLen, 1);
    musicLen -= startAddr;

    MusicSelect = 0;
    At24c02_ReadData(0xa0, startAddr, Music_arry, musicLen);
    TimerInit();
    while (1)
    {
        // if (Music_arry[MusicSelect] == MUSIC_END)
        //     MusicSelect = 0;
        // MusicSelect = Music_arry[MusicSelect]; // 选择音符对应的频率
        // MusicSelect++;

        // Delay_music((Speed >> 2) * Music_arry[MusicSelect]); //
        // 选择音符对应的时长
        // MusicSelect++;

        // TR1 = 0; // 不同音符间短暂停顿，利用延时开关定时器实现
        // Delay_music(24);
        // TR1 = 1;
        // if (Music_arry[MusicSelect] != MUSIC_END) // 如果不是停止标志位
        // {
        //     MusicSelect = Music_arry[MusicSelect]; // 选择音符对应的频率
        //     MusicSelect++;

        //     Delay_music(
        //         Speed / 4 * Music_arry[MusicSelect]
        //     ); // 选择音符对应的时长
        //     MusicSelect++;

        //     TR1 = 0; // 不同音符间短暂停顿，利用延时开关定时器实现
        //     Delay_music(24);
        //     TR1 = 1;
        // }
        // else // 如果是停止标志位
        // {
        //     TR1 = 0; // 关闭定时器
        //     MusicSelect = 0;
        // }
    }
}
bit play_music = 1;
unsigned int freqDelay = 25;
void int_T0() interrupt 1 using 1
{
    if (play_music)
        if (--freqDelay == 24)
            TR1 = 0;
        else if (!freqDelay)
        {
            TR1 = 1;
            if (Music_arry[MusicSelect] == 0xff)
                MusicSelect = 0;
            freqH = FreqTable[Music_arry[MusicSelect]] >> 8;
            freqL = FreqTable[Music_arry[MusicSelect]] & 0xff;
            ++MusicSelect;
            freqDelay = Speed * Music_arry[MusicSelect]; // 选择音符对应的时长
            ++MusicSelect;
        }
    // if (delay > 1)
    // {
    //     --delay;
    //     return;
    // }
    // else if (delay == 1)
    // {
    //     --delay;
    //     TR1 = 0;
    //     space = 24;
    //     return;
    // }
    // else if (space)
    // {
    //     --space;
    //     return;
    // }

    // if (!TR1) // 不同音符间短暂停顿，利用延时开关定时器实现
    // {
    //     TR1 = 1;
    //     return;
    // }

    // if (Music_arry[MusicSelect] == MUSIC_END)
    // {
    //     MusicSelect = 0;
    // }
    // // MusicSelect = Music_arry[MusicSelect]; // 选择音符对应的频率
    // freqH = FreqTable[Music_arry[MusicSelect]] >> 8;
    // freqL = FreqTable[Music_arry[MusicSelect]] & 0xff;
    // MusicSelect++;

    // delay = (Speed)*Music_arry[MusicSelect]; // 选择音符对应的时长
    // MusicSelect++;
}

void int_T1() interrupt 3 using 2
{
    if (freqH || freqL) // 如果是休止符(0)，那么不播放声音，只进行延时
    {
        // 取对应频率值的重装载值到定时器(确认音高)MusicSelect=Music[MusicSelect]
        TL1 = freqL; // 设置低位定时初值
        TH1 = freqH;   // 设置高位定时初值
        // 翻转蜂鸣器IO口(注意这里的重装值是周期的一半，故仅进行一次蜂鸣器的翻转)
        Buzzer = !Buzzer;
    }
}
