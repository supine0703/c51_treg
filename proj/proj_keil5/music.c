// #include "music.h"
// #include "at24c02.h"

// void Timer0Init(void)
// {
// 	TMOD &= 0xF0; // 设置定时器模式
// 	TMOD |= 0x01; // 设置定时器模式
// 	TL0 = 0x18;	  // 设置定时初值
// 	TH0 = 0xFC;	  // 设置定时初值
// 	TF0 = 0;	  // 清除TF0标志
// 	TR0 = 1;	  // 定时器0开始计时
// 	ET0 = 1;
// 	EA = 1;
// 	PT0 = 0;
// }

// void Delay_music(unsigned int t)
// {
// 	unsigned int j;
// 	while (t--)
// 	{

// 		for (j = 125; j > 0; --j)
// 		{
// 		}
// 	}
// }

// void Music_score()
// {
// 	unsigned int i;
	
	
// 	for (MusicSelect = 0, i = 0; i < 39; MusicSelect++, i++)
// 	{
// 		Music_arry[i] = Music[MusicSelect]; 
// 	}
// 	// rtemp[i] = 0xff;
//     At24c02_WriteData(0xa0,0x02,Music_arry,39);


// //     for (i = 0; i < 126; MusicSelect++, i++)
// // 	{
// // 		Music_arry[i] = Music[MusicSelect]; 
// // 	}
// //    At24c02_WriteData(0xa0,0x82,Music_arry,126);
// }

