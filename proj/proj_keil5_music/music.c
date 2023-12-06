#include "music.h"
#include "at24c02.h"

unsigned int Speed = 600;
unsigned int FreqSelect, MusicSelect;
unsigned char idata Music_arry[128];

// 乐谱
unsigned char code Music[] =
{
    // 乐谱较长加上关键字code将其储存在ROM(flash)
    // 音符,时值,

    // 孤勇者
    // 1
    H1,1, H2,1, M7,1, H1,1, H1,2, H1,1, M7,1, H1,1, H2,1, M7,1, H1,1, H1,2, H1,1, H2,1,

    // 2
    H3,1, H2,1, H3,1, H2,1, H3,2, H3,1, H2,1, H3,2, H5,2, H3,2, M6,1, M7,1,

    // 3
    H1,1, H2,1, M7,1, H1,1, H1,2, H1,1, M7,1, H1,1, H2,1, M7,1, H1,1, H1,2, H1,1, M7,1,

    0xff,

    // 天空之城
    // 1
    M6,2, M7,2,
    // 2
    H1,6, M7,2, H1,4, H3,4,
    // 3
    M7,12, M3,2, M3,2,
    // 4
    M6,6, M5,2, M6,4, H1,4,
    // 5
    M5,12, M3,4,
    // 7
    M4,6, M3,2, M4,4, H1,4,
    // 8
    M3,8, P,2, H1,2, H1,2, H1,2,
    // 9 
    M7,6, M4,2, M4,4, M7,4, 
    // 10
    M7,8, P,4,
	// M6,2, M7,2,

    0xff,

    // 小星星
    // 1
    M1,4, M1,4, M5,4, M5,4,
    // 2
    M6,4, M6,4, M5,8, 
    // 3 
    M4,4, M4,4, M3,4, M3,4,
    // 4
    M2,4, M2,4, M1,8, 
    // 5
    M5,4, M5,4, M4,4, M4,4,
    // 6
    M3,4, M3,4, M2,8,
    // 7
    M5,4, M5,4, M4,4, M4,4,
    // 8
    M3,4, M3,4, M2,8,

    0xff,

    0xfe // 终止标志
};



// 索引与频率对照表
unsigned int code FreqTable[] =
{
    0,     // 0
    63628, // 1
    63731, // 2
    63835, // 3
    63928, // 4
    64021, // 5
    64103, // 6
    64185, // 7
    64260, // 8
    64331, // 9
    64400, // 10
    64463, // 11
    64528, // 12
    64580, // 13
    64633, // 14
    64684, // 15
    64732, // 16
    64777, // 17
    64820, // 18
    64860, // 19
    64898, // 20
    64934, // 21
    64968, // 22
    65000, // 23
    65030, // 24
    65058, // 25
    65085, // 26
    65110, // 27
    65134, // 28
    65157, // 29
    65178, // 30
    65198, // 31
    65217, // 32
    65235, // 33
    65252, // 34
    65268, // 35
    65283, // 36
};

// void Delay_music(unsigned int t)
// {
//     unsigned int j;
//     while (t--)
//     {
//         for (j = 125; j > 0; --j)
//         {
//         }
//     }
// }

/**
 * 约定:
 * 0-2 前三字节空出来
 * 3-7 第4-8字节存储音乐的起止地址 (最多4首歌)
 * 8-255 留于存储音乐
*/
void Music_score(void)
{
    unsigned char addr, temp, i;
    addr = 0x03;
    temp = 0x08;
    At24c02_WriteData(0xa0, addr, &temp, 1);
    for (i = 0; Music[i] != ALL_END; i++)
    {
        if (Music[i] == MUSIC_END)
        {
            temp = 0x09 + i;
            At24c02_WriteData(0xa0, ++addr, &temp, 1);
        }
    }
    while (++addr < 0x08)
        At24c02_WriteData(0xa0, addr, &temp, 1);
    At24c02_WriteData(0xa0, 0x08, Music, i);
}
