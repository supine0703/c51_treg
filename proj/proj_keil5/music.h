#include <REG52.H>


// 音符与索引对应表，P：休止符，L：低音，M：中音，H：高音，下划线：升半音符号#
#define P 0
#define L1 1
#define L1_ 2
#define L2 3
#define L2_ 4
#define L3 5
#define L4 6
#define L4_ 7
#define L5 8
#define L5_ 9
#define L6 10
#define L6_ 11
#define L7 12
#define M1 13
#define M1_ 14
#define M2 15
#define M2_ 16
#define M3 17
#define M4 18
#define M4_ 19
#define M5 20
#define M5_ 21
#define M6 22
#define M6_ 23
#define M7 24
#define H1 25
#define H1_ 26
#define H2 27
#define H2_ 28
#define H3 29
#define H4 30
#define H4_ 31
#define H5 32
#define H5_ 33
#define H6 34
#define H6_ 35
#define H7 36

unsigned int Speed = 600;
unsigned int FreqSelect, MusicSelect;

// 索引与频率对照表
unsigned int FreqTable[] =
	{
		0,
		63628,
		63731,
		63835,
		63928,
		64021,
		64103,
		64185,
		64260,
		64331,
		64400,
		64463,
		64528,
		64580,
		64633,
		64684,
		64732,
		64777,
		64820,
		64860,
		64898,
		64934,
		64968,
		65000,
		65030,
		65058,
		65085,
		65110,
		65134,
		65157,
		65178,
		65198,
		65217,
		65235,
		65252,
		65268,
		65283,
};

// 乐谱
unsigned int code Music[] =
	{
		// 乐谱较长加上关键字code将其储存在ROM(flash)
		// 音符,时值,

		// 孤勇者
		// 1
		// H1,1, H2,1, M7,1, H1,1, H1,2, H1,1, M7,1, H1,1, H2,1, M7,1, H1,1, H1,2, H1,1, H2,1,

		// // 2
		// H3,1, H2,1, H3,1, H2,1, H3,2, H3,1, H2,1, H3,2, H5,2, H3,2, M6,1, M7,1,

		// // 3
		// H1,1, H2,1, M7,1, H1,1, H1,2, H1,1, M7,1, H1,1, H2,1, M7,1, H1,1, H1,2, H1,1, M7,1,

		// //15
		// H3,1, H2,1, H3,1, H2,1, H3,2, H3,1, H2,1, H3,2, H5,2, H3,2, H5,2,

		// //16
		// H3,3, H5,1, H3,3, H5,1, H3,1, H5,1, H6,1, H3,1, H5,2, H5,2,

		// //17
		// H3,3, H5,1, H3,3, H5,1, H3,1, H5,1, H6,1, H3,1, H5,2, H5,1, H5,1,

		// //18
		// H3,1, H2,1, H2,2, H2,2, H1,1, H3,1, H3,1, H2,1, H2,2, H2,2, H1,1, H1,1,

		// //19
		// M6,8, P,4, P,2, H5,1, H5,1,

		// //20
		// H3,1, H2,1, H2,2, H2,2, H1,1, H3,1, H3,1, H2,1, H2,2, H2,2, H1,1, H1,1,

		// 天空之城
		// 1
		M6, 2, M7, 2,
		// 2
		H1, 6, M7, 2, H1, 4, H3, 4,
		// 3
		M7, 12, M3, 2, M3, 2,
		// 4
		M6, 6, M5, 2, M6, 4, H1, 4,
		// 5
		M5, 12, M3, 4,
		// 7
		M4, 6, M3, 2, M4, 4, H1, 4,

		0xFF // 终止标志
};



void Timer0Init(void);
void Delay(unsigned int t);
void Music_store(void);