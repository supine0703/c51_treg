#include <music.h>


void Timer0Init(void)
{
	TMOD &= 0xF0; // 设置定时器模式
	TMOD |= 0x01; // 设置定时器模式
	TL0 = 0x18;	  // 设置定时初值
	TH0 = 0xFC;	  // 设置定时初值
	TF0 = 0;	  // 清除TF0标志
	TR0 = 1;	  // 定时器0开始计时
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}

void Delay_music(unsigned int t)
{
	unsigned int j;
	while (t--)
	{

		for (j = 125; j > 0; --j)
		{
		}
	}
}