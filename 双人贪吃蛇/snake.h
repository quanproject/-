//2018.10.18      by quan

//四个方向宏定义
//player 1
#define UP 105  //i 上
#define DOWN 107  //k 下
#define LEFT 106  //j 左
#define RIGHT 108  //l 右

#define SPEED 101 //e 加速

//player 2
#define up 119  //w 上
#define down 115  //s 下
#define left 97  //a 左
#define right 100  //d 右

#define speed 117 //u 加速

#define SIZE 30 //每方块大小
#define frame_height 32//地图高格数
#define frame_width 32 //地图长格数
#define GAMETIME 610 //游戏时间

//游戏内各个量定义

#define SNAKE 1 //蛇1
#define snake 10  //蛇2
#define FOOD 2  //食物
#define BAR 3   //围墙
#define OBSTACLE 4  //障碍物
// 0 是空地图

#define Azure 0xe4e6e5//背景色
#define p1色  0x7d62da 
#define p1加速色 0x6038a5
#define p2色 0xb0a458
#define p2加速色 0x513f37

//坐标比较函数
int cor_cmp(COORD pt1, COORD pt2)
{
	return (pt1.X == pt2.X&&pt1.Y == pt2.Y); //相等便返回1
}

//用户名结构体
typedef struct Users
{
	char id[10];
	char password[10];
	int score = 0;
}user;

//画正方形
void const drawdre(COORD x)
{
	solidrectangle(x.X*SIZE, x.Y* SIZE, x.X*SIZE + SIZE, x.Y* SIZE + SIZE);
}