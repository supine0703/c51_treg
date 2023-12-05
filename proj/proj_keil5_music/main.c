#include "music.h"
#include "at24c02.h"

extern unsigned int Speed;
extern unsigned int FreqSelect, MusicSelect;
extern unsigned char idata Music_arry[128]; 
extern unsigned int code Music[];
extern unsigned int code FreqTable[];

void main()
{
	unsigned char music_num, startAddr, musicLen;
	Music_score();

	music_num = 1;
	At24c02_ReadData(0xa0, music_num + 0x03, &startAddr, 1);
	At24c02_ReadData(0xa0, music_num + 0x04, &musicLen, 1);
	musicLen -= startAddr;

	MusicSelect = 0;
    At24c02_ReadData(0xa0, startAddr, Music_arry, musicLen);
	Timer0Init();
	while(1)
	{

		if(Music_arry[MusicSelect]!=0xff)	//如果不是停止标志位
		{
			FreqSelect=Music_arry[MusicSelect];	//选择音符对应的频率
			MusicSelect++;
 
			Delay_music(Speed/4*Music_arry[MusicSelect]);	//选择音符对应的时长
			MusicSelect++;
 
			TR0=0;//不同音符间短暂停顿，利用延时开关定时器实现
			Delay_music(24);	
			TR0=1;
		}
		else	//如果是停止标志位
		{
			TR0=0;  //关闭定时器
			MusicSelect = 0;
		}
	}
}
 
void Timer0_Routine() interrupt 1
{
	if(FreqTable[FreqSelect])	//如果是休止符(0)，那么不播放声音，只进行延时
	{
		//取对应频率值的重装载值到定时器(确认音高)FreqSelect=Music[MusicSelect]
		TL0 = FreqTable[FreqSelect]&0xff;		//设置低位定时初值
		TH0 = FreqTable[FreqSelect]>>8;		//设置高位定时初值
		Buzzer=!Buzzer;	//翻转蜂鸣器IO口(注意这里的重装值是周期的一半，故仅进行一次蜂鸣器的翻转)
	}
}
