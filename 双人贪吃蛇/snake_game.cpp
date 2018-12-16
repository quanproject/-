//主要功能实现完毕  10.30
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<graphics.h>
#include<windows.h>
#include"snake.h"  //蛇define头文件


typedef struct Lnode {                                          //
	COORD cor;             //记录坐标的变量                     //
	struct Lnode *next;                                         //
}LNode, *LinkList;

//函数声明   
void thru(LinkList L);
void init_game();
int menu();
void resistsgame();
void login();
COORD generate_food(COORD x);
void singlegame();
void savescore();

int player1_len = 1;  //蛇的长度
int player2_len = 1;

int p1_speedup = 0;  //当次数为奇数就加速蛇1
int p2_speedup = 0;

int speedtime = 0;  //用来给另一条蛇减速

int foodscore = 2; //果子初始分数
int p1_score = 0;  //初始分数
int p2_score = 0;
int result = 0; //游戏结果
double gametimes = GAMETIME;  //游戏时间

COORD P1_ScoreBoard = { 34 * SIZE,6 * SIZE };//双人记分板位置
COORD P2_ScoreBoard = { 34 * SIZE,7 * SIZE + 20 };
COORD TIME = { 34 * SIZE,2 * SIZE + 11 };  //时间板位置
COORD SCORE_BORAD = { 34 * SIZE,16 * SIZE };  //单人分数版位置
COORD food, oldfood;  //初始食物的位置
LinkList player1_head, player2_head;  //定义蛇头指针
user A, B;  //用户
FILE *fp;  //文件指针
user ranklist[10]; //排行榜输出用

				   //链表结构                                                      //


				   //边界穿越
void thru(LinkList L)  //穿越边界传送函数
{
	if (player1_head->cor.X < 0)
	{
		setfillcolor(p1色);
		drawdre({ player1_head->cor.X + 32, player1_head->cor.Y });
		player1_head->cor.X = 31;
	}
	if (player2_head->cor.X < 0)
	{
		setfillcolor(p2色);
		if (p2_speedup % 2 == 0)
			drawdre({ player2_head->cor.X + 32, player2_head->cor.Y });
		else
		{
			setfillcolor(p2加速色);
			drawdre({ player2_head->cor.X + 32, player2_head->cor.Y });
		}
		player2_head->cor.X = 31;
	}
	if (player1_head->cor.X >31)
	{
		setfillcolor(p1色);
		drawdre({ player1_head->cor.X - 32, player1_head->cor.Y });
		setfillcolor(0xa2a2a6);//遮盖尾部最后一节
		drawdre(player1_head->cor);
		player1_head->cor.X = 0;
	}

	if (player2_head->cor.X > 31)
	{
		setfillcolor(0xa2a2a6);
		drawdre(player2_head->cor);
		setfillcolor(p2色);
		drawdre({ player2_head->cor.X - 32, player2_head->cor.Y });
		player2_head->cor.X = 0;
	}
	if (player1_head->cor.Y < 0)
	{

		setfillcolor(p1色);
		drawdre({ player1_head->cor.X, player1_head->cor.Y + 32 });
		player1_head->cor.X = 31;
	}
	if (player2_head->cor.Y <0)
	{
		setfillcolor(p2色);
		drawdre({ player2_head->cor.X, player2_head->cor.Y + 32 });
		player2_head->cor.Y = 31;

	}
	if (player2_head->cor.Y > 31)
	{
		setfillcolor(0xa2a2a6);
		drawdre(player2_head->cor);
		setfillcolor(p2色);
		drawdre({ player2_head->cor.X, player2_head->cor.Y - 32 });
		player2_head->cor.Y = 0;
	}
	if (player1_head->cor.Y> 31)
	{
		setfillcolor(p1色);
		drawdre({ player1_head->cor.X, player1_head->cor.Y - 32 });
		setfillcolor(0xa2a2a6);//遮盖尾部最后一节
		drawdre(player1_head->cor);
		player1_head->cor.Y = 0;
	}

}
//初始化界面
void init_game()
{
	player1_head = (LinkList)malloc(sizeof(LNode));  //创建一个蛇头
	player1_head->cor.X = 24;  //钦定蛇头位置
	player1_head->cor.Y = 24;
	player1_head->next = NULL;  //指针对空防爆

	player2_head = (LinkList)malloc(sizeof(LNode));  //创建一个蛇头
	player2_head->cor.X = 12;  //钦定蛇头位置
	player2_head->cor.Y = 12;
	player2_head->next = NULL;  //指针对空防爆

								//初始化p1的蛇头
	setfillcolor(p1色);
	drawdre(player1_head->cor);
	//初始化p2的蛇头
	setfillcolor(p2色);
	drawdre(player2_head->cor);
	//初始化食物
	generate_food({ 0, 0 });
	food = generate_food({ 0,0 });
	setlinecolor(BLUE);
	setfillcolor(BLACK);
	fillcircle(food.X*SIZE + SIZE / 2, food.Y*SIZE + SIZE / 2, SIZE / 2);




}
//更新食物坐标
COORD generate_food(COORD x)  //随机生成食物坐标函数
{
	COORD food_;
	LinkList p = NULL;   //指针对空防爆
	int in_snake;

	srand((unsigned int)time(NULL));

	do {
		in_snake = 0;
		food_.X = rand() % (frame_height - 2) + 1;
		food_.Y = rand() % (frame_width - 2) + 1;
		for (p = player1_head; p != NULL; p = p->next) //判断是否生成在蛇1身上
			if (cor_cmp(food_, p->cor))
				in_snake = 1;
		for (p = player2_head; p != NULL; p = p->next) //判断是否生成在蛇2身上
			if (cor_cmp(food_, p->cor))
				in_snake = 1;
		if (cor_cmp(x, food_))
			in_snake = 1;
	} while (in_snake);
	foodscore += 2; //食物分数上涨
	return food_;
}
//移动p1蛇
void move_snake1(int dir)
{
	LinkList p, t = NULL;
	LinkList newp;
	newp = (LinkList)malloc(sizeof(LNode));
	newp->cor = player1_head->cor;        //记录坐标值
	newp->next = player1_head;    //把新头的next指针指向旧头
	player1_head = newp;   //头权易位
	int grow = 0;  //判断蛇是否增长的flag
	switch (dir) {
	case UP:
		player1_head->cor.Y--;
		break;
	case DOWN:
		player1_head->cor.Y++;
		break;
	case LEFT:
		player1_head->cor.X--;
		break;
	case RIGHT:
		player1_head->cor.X++;
		break;
	}
	if (cor_cmp(player1_head->cor, food)) 	//如果食物跟头坐标相同
	{
		grow = 1; //增长标志

		if (p2_speedup % 2 == 0)  //没加速正常得分
			p2_score += foodscore;
		else
			p2_score += foodscore * 2;  //翻倍得分
		oldfood = food;
		food = generate_food(oldfood);
	}

	if (p2_speedup % 2 == 0)
	{
		setlinecolor(Azure);
		setfillcolor(p1色);
		drawdre(player1_head->cor);
	}
	else
	{
		setlinecolor(Azure);
		setfillcolor(p1加速色);
		drawdre(player1_head->cor);
	}

	if (grow) {  //变长的话不再对链表进行操作
		player1_len++;
		setlinecolor(BLUE);
		setfillcolor(BLACK);
		fillcircle(food.X*SIZE + SIZE / 2, food.Y*SIZE + SIZE / 2, SIZE / 2);

	}
	else {
		for (p = player1_head; p->next->next != NULL; p = p->next)
			;//直达蛇倒数第二节
		t = p->next;//记录下最后一节的地址
		p->next = NULL;
		setfillcolor(Azure);//遮盖尾部最后一节
		drawdre(t->cor);
		free(t);//释放内存空间
	}

}
//移动p2蛇
void move_snake2(int dir)
{
	LinkList p, t = NULL;
	LinkList newp;
	newp = (LinkList)malloc(sizeof(LNode));
	newp->cor = player2_head->cor;        //记录坐标值
	newp->next = player2_head;    //把新头的next指针指向旧头
	player2_head = newp;   //头权易位
	int grow = 0;  //判断蛇是否增长的flag
	switch (dir) {
	case up:
		player2_head->cor.Y--;
		break;
	case down:
		player2_head->cor.Y++;
		break;
	case left:
		player2_head->cor.X--;
		break;
	case right:
		player2_head->cor.X++;
		break;
	}
	if (cor_cmp(player2_head->cor, food)) 	//如果食物跟头坐标相同
	{
		grow = 1;
		if (p1_speedup % 2 == 0)  //没加速正常得分
			p1_score += foodscore;
		else
			p1_score = p1_score + foodscore * 2;  //翻倍得分
		oldfood = food;
		food = generate_food(oldfood);
	}

	if (p1_speedup % 2 == 0)
	{
		setlinecolor(BLUE);
		setfillcolor(p2色);		drawdre(player2_head->cor);
	}
	else
	{
		setlinecolor(BLUE);
		setfillcolor(p2加速色);
		drawdre(player2_head->cor);
	}
	if (grow) {  //变长的话不再对链表进行操作
		player1_len++;
		setlinecolor(BLUE);
		setfillcolor(BLACK);
		fillcircle(food.X*SIZE + SIZE / 2, food.Y*SIZE + SIZE / 2, SIZE / 2);
	}
	else {
		for (p = player2_head; p->next->next != NULL; p = p->next)
			;//直达蛇倒数第二节
		t = p->next;//记录下最后一节的地址
		p->next = NULL;
		setfillcolor(Azure);
		drawdre(t->cor);
		free(t);//释放内存空间
	}
}
//获取移动方向
void gamemove(int *dir1, int *dir2)  //11.13修改
{
	int i = 0;  //输入值
	int flag = 0;  //决定输入内容的控制对象


	if (_kbhit())  //如果有输入
		i = _getch();
	if (i == up || i == down || i == right || i == left)
		flag = 2;  //aswd控制的蛇
	if (i == UP || i == DOWN || i == RIGHT || i == LEFT)
		flag = 1;  //jkli控制的蛇
	if (i == speed)
		p2_speedup++;
	if (i == SPEED)
		p1_speedup++;

	if (flag == 2)  //蛇2操作方向
	{
		if ((abs(i - *dir2) == 4) || (abs(i - *dir2) == 3)) //蛇不能回头
			i = *dir2;
		move_snake2(i);
		move_snake1(*dir1);
		*dir2 = i;
		fflush(stdin);
		Sleep(100);
	}


	if (flag == 1)  //蛇1操作方向
	{
		if (abs(i - *dir1) == 2)   //蛇不能回头
			i = *dir1;
		move_snake1(i);  //移动控制蛇
		move_snake2(*dir2);  //另一个蛇保持方向移动
		*dir1 = i;  //储存旧的方向准备下一次比较
		fflush(stdin);
		Sleep(100);
	}


	if (flag == 0)  //没有操作则蛇持续保持原方向移动
	{
		if (p1_speedup % 2 == 0 && p2_speedup % 2 == 0)  //都不加速
		{
			move_snake1(*dir1);
			move_snake2(*dir2);
			fflush(stdin);
			Sleep(100);
		}
		else if (p1_speedup % 2 != 0 && p2_speedup % 2 == 0)
		{
			move_snake2(*dir2);
			if (speedtime++ % 2 == 0)  //四倍速于另一条蛇
				move_snake1(*dir1);
			fflush(stdin);
			Sleep(50);
		}
		else if (p1_speedup % 2 == 0 && p2_speedup % 2 != 0)
		{
			move_snake1(*dir1);
			if (speedtime++ % 2 == 0)
				move_snake2(*dir2);
			fflush(stdin);
			Sleep(50);
		}
		else
		{
			move_snake1(*dir1);
			move_snake2(*dir2);
			fflush(stdin);
			Sleep(50);
		}
	}
}
//判断死活
int isalive(LinkList L, LinkList Z) //11.18 修改
{
	int lives_state = 0; //0为游戏正常运行 ，1为自己输，2为平局,3为p1赢，4为p2赢
	LinkList p = NULL;
	for (p = L->next; p != NULL; p = p->next)   //不装到自己
		if (cor_cmp(p->cor, L->cor))
			lives_state = 1;
	for (p = Z->next; p != NULL; p = p->next)   //不装到别人（蛇
	{
		if ((cor_cmp(p->cor, L->cor)) && (p1_speedup + p2_speedup) % 2 == 0)  //两蛇同状态下
			lives_state = 1;
		else if ((cor_cmp(p->cor, L->cor)) && p2_speedup % 2 != 0)  //p2加速易死
			lives_state = 3;
		else if ((cor_cmp(p->cor, L->cor)) && p1_speedup % 2 != 0) //p1加速易死
			lives_state = 4;
	}
	thru(L);//穿越边界
	if (cor_cmp(L->cor, Z->cor))
		lives_state = 2;
	//***********移动障碍物修改处**************** 10.18
	return lives_state;
}
//显示分数
void gamescore(COORD x, int playerscore)
{
	char score[255];
	sprintf(score, "%d", playerscore);  //转化数据类型
	setfillcolor(WHITE);
	settextstyle(40, 0, _T("黑体"));
	//	settextcolor(BLACK);  //字体颜色
	RECT r2 = { x.X, x.Y, x.X + 180,x.Y + 90 };

	drawtext(score, &r2, DT_VCENTER || DT_SINGLELINE);
}
//显示时间
int gametime()  //11.13修改
{
	static int flag = 0;
	if (p1_speedup % 2 == 0 && p2_speedup % 2 == 0) //蛇没有加速时时间正常流动
		gametimes -= 1;
	else        //加速时流动速度减半（对应双倍速
		gametimes -= 0.5;

	if (gametimes <= GAMETIME / 4 && flag == 0)
	{
		foodscore *= 2;
		flag = 1;
	}
	int a = gametimes / 10;
	char time[200];   //要输出的时间（char
	sprintf(time, "%d", a);  //转化数据类型
	settextcolor(BLACK);  //字体颜色
	RECT r1 = { TIME.X, TIME.Y, TIME.X + 180, TIME.Y + 40 }; //确定时间位置
	drawtext(time, &r1, DT_NOCLIP || DT_VCENTER || DT_SINGLELINE); //画出时间
	return a;
}
//游戏结果
void gameresult(int result) //11.18 修改
{
	char str[5];
	int x;
	int flag = 1;
	if (result == 1 || result == 3)
	{
		settextcolor(YELLOW);
		outtextxy(500, 200, _T("P1 WIN!"));
	}
	if (result == 0 || result == 4)
	{
		settextcolor(YELLOW);
		outtextxy(500, 200, _T("P2 WIN!"));
	}
	if (result == 2)
	{
		settextcolor(YELLOW);
		outtextxy(500, 200, _T("旗鼓相当的对手!"));
	}
	if (result == 3)  //单人游戏储存成绩
	{
		settextcolor(YELLOW);
		outtextxy(500, 200, _T("YOU LOSE"));
		savescore();   //保存成绩
		flag = 2;
	}
	while (flag == 1)//双人模式
	{
		InputBox(str, 5, _T("游戏结束1.返回主界面 2.再来一局"));//结束后的选择
		sscanf_s(str, "%d", &x);//转换类型
		switch (x)
		{
		case 1: {menu(); flag = 0; break; }
		case 2: {resistsgame(); flag = 0;  break; }
		default: {::MessageBox(NULL, "请输入正确的数值", "错误", MB_SETFOREGROUND); break; }
		}
	}
	while (flag == 2)//单人模式
	{
		InputBox(str, 5, _T("游戏结束1.返回主界面 2.再来一局"));//结束后的选择
		sscanf_s(str, "%d", &x);//转换类型
		switch (x)
		{
		case 1: {menu(); flag = 0; break; }
		case 2: {singlegame(); flag = 0;  break; }
		default: {::MessageBox(NULL, "请输入正确的数值", "错误", MB_SETFOREGROUND); break; }
		}
	}
}
//主界面排行榜显示
void ranking()
{
	int i = 130;
	char str[10], str1[10];
	loadimage(NULL, _T("mainrank.jpg"));

	settextcolor(WHITE);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(1020, i, "得分");
	outtextxy(1120, i, "ID");
	i += 85;
	fopen_s(&fp, "score.txt", "r");
	for (int NUM = 0; NUM < 9; NUM++)//从文件中读取十个数据,并输出
	{
		fscanf_s(fp, "%s%s", str, sizeof(str), str1, sizeof(str1));

		char no[5];   //要输出的位次
		sprintf(no, "%d", NUM + 1);  //转化数据类型

		settextstyle(20, 0, _T("黑体"));

		outtextxy(1000, i, no);
		outtextxy(1020, i, str);
		outtextxy(1120, i, str1);
		i += 75;
	}
	fclose(fp);
	getch();
	menu();
}
//获取排行榜
void getrank()
{
	char transcore[10];
	fopen_s(&fp, "score.txt", "r");
	for (int i = 0; i < 10; i++)
	{
		fscanf_s(fp, "%s%s", transcore, sizeof(transcore), ranklist[i].id, sizeof(ranklist[i].id));
		sscanf(transcore, "%d", &ranklist[i].score);//字符串转换为整形数据并存入
	}
	fclose(fp);
}
//排序
void sort(user scoresort[])
{
	user t;
	for (int i = 0; i < 10; i++)
	{
		if (scoresort[9].score > scoresort[i].score)
		{
			t = scoresort[i];
			scoresort[i] = scoresort[9];
			scoresort[9] = t;
		}
	}
}
//刷新排行榜
void displayrank()
{
	//sort(ranklist);
	int i = 330;
	for (int NUM = 0; NUM < 9; NUM++)//从文件中读取十个数据,并输出
	{
		char no[5], score[10];   //要输出的位次
		sprintf(no, "%d", NUM + 1);  //转化数据类型
		sprintf(score, "%d", ranklist[NUM].score);  //转化数据类型
		settextstyle(20, 0, _T("黑体"));

		outtextxy(1020, i, no);
		outtextxy(1050, i, score);
		outtextxy(1150, i, ranklist[NUM].id);
		i += 40;
	}
}
//注册
void registers()//注册账号密码
{
	fopen_s(&fp, "user.txt", "r");
	fscanf_s(fp, "%s%s", B.id, sizeof(B.id), B.password, sizeof(B.password));
	InputBox(A.id, 11, "注册请输入账号");
	while (1)
	{
		if (strcmp(A.id, B.id) != 0)//如果两串字符串不相等
		{
			if (!feof(fp))//如果未至文件末尾  
			{
				fscanf_s(fp, "%s%s", B.id, sizeof(B.id), B.password, sizeof(B.password));
			}
			else break;
		}
		else {
			::MessageBox(NULL, "用户名已被注册！", "错误！", MB_SETFOREGROUND);
			fclose(fp);
			return;  //返回选择注册登陆
		}
	}
	fclose(fp);
	InputBox(A.password, 10, "注册请输入密码");
	fopen_s(&fp, "user.txt", "a");
	fprintf_s(fp, "%s %s\n", A.id, A.password);
	::MessageBox(NULL, "恭喜，注册成功！", "恭喜！", MB_SETFOREGROUND);
	fclose(fp);

}
//登陆
void login()//登陆账号密码
{
	fopen_s(&fp, "user.txt", "r");
	fscanf_s(fp, "%s%s", B.id, sizeof(B.id), B.password, sizeof(B.password));
	InputBox(A.id, 11, "登录请输入账号");
	while (1)
	{
		if (strcmp(A.id, B.id) == 0) break;//如果找到了这个用户名  
		else {
			if (!feof(fp))//如果文件未读完   
				fscanf_s(fp, "%s%s", B.id, sizeof(B.id), B.password, sizeof(B.password));
			else
			{
				::MessageBox(NULL, "用户不存在", "错误", MB_SETFOREGROUND);
				login();

			}
		}
	}
	InputBox(A.password, 20, "登陆请输入密码");
	if (strcmp(A.password, B.password) == 0)//如果密码匹配 
		fclose(fp);
	else {

		::MessageBox(NULL, "密码错误", "错误", MB_SETFOREGROUND);
		login();


	}
}
//开始选择菜单
int menu()
{
	int dir;
	char s1[5], s2[5];
	int flag = 1;
	int x, y[10];  //x注册登陆选择变量
	setbkcolor(TRANSPARENT);

	loadimage(NULL, _T("main.jpg"));

	//字体与字
	settextstyle(40, 0, _T("黑体"));
	settextcolor(BLUE);

	int ox = 1000;
	outtextxy(ox, 200, _T("单人游戏"));//菜单界面文字输出
	settextcolor(RED);
	outtextxy(ox, 400, _T("双人游戏"));
	outtextxy(ox, 600, _T("排行榜"));
	outtextxy(ox, 800, _T("退出游戏"));
	while (1)
	{
		if (_kbhit)
		{//根据flag值进行移动

			dir = _getch();
			if (dir == 72 && flag != 1)
				flag--;
			if (dir == 80 && flag != 4)
				flag++;
			if (dir == 32)
				break;
			settextcolor(RED);
			outtextxy(ox, 200, _T("单人游戏"));//菜单界面文字输出
			outtextxy(ox, 400, _T("双人游戏"));
			outtextxy(ox, 600, _T("排行榜"));
			outtextxy(ox, 800, _T("退出游戏"));
			switch (flag)
			{
			case 1: settextcolor(BLUE);
				outtextxy(ox, 200, _T("单人游戏"));
				break;

			case 2: settextcolor(BLUE);
				outtextxy(ox, 400, _T("双人游戏"));
				break;
			case 3:settextcolor(BLUE);
				outtextxy(ox, 600, _T("排行榜"));
				break;
			case 4: settextcolor(BLUE);
				outtextxy(ox, 800, _T("退出游戏"));
				break;
			default: break;
			}
		}
	}
	if (flag == 1)//单人游戏
	{
		singlegame();
	}

	if (flag == 2)//双人游戏
	{
		resistsgame();
	}
	if (flag == 3)//排行榜
	{
		ranking();
	}

	return 0;
}
//储存成绩
void savescore()
{
	strcpy(ranklist[9].id, A.id); //记录当前玩家数据
	ranklist[9].score = p1_score;
	sort(ranklist);

	fopen_s(&fp, "score.txt", "w+");
	for (int i = 0; i < 9; i++)
	{
		char outscore[10];
		sprintf(outscore, "%d", ranklist[i].score);  //转化数据类型

		fprintf_s(fp, "%s %s\n", outscore, ranklist[i].id);//将得分和用户名记录到txt
	}
	fclose(fp);
}
//双人游戏
void resistsgame()
{
	//重置数据
	p1_speedup = 0;//加速状态
	p2_speedup = 0;
	p1_score = 0;  //得分
	p2_score = 0;
	foodscore = 2; //果子分数

	gametimes = GAMETIME;  //初始化游戏时间
	init_game();


	setbkcolor(0xcdd2cf);  //设置字背景
	cleardevice();

	loadimage(NULL, _T("1.jpg"));  //加载背景图片

	int player1_dir = RIGHT, player2_dir = right;  //蛇1,蛇2方向变量
	int result = 0;  //游戏结果
	init_game();
	::MessageBox(NULL, "准备！", "双人模式", MB_SETFOREGROUND);

	while (1)
	{
		gamemove(&player1_dir, &player2_dir); //蛇移动
		settextcolor(p1色);
		gamescore(P2_ScoreBoard, p2_score);  //两蛇分数板
		settextcolor(p2色);
		gamescore(P1_ScoreBoard, p1_score);
		if ((result = isalive(player1_head, player2_head)) || isalive(player2_head, player1_head)) //isalive结果不为0则停止游戏
			break;
		if (!gametime()) //时间结束
		{
			if (p1_score >= p2_score)
				result = 1;
			break;
		}
	}
	gameresult(result);
}
//单人移动
void single_gamemove(int *dir)  //控制键盘输入获取和蛇移动
{
	int i = 0;
	if (_kbhit())  //如果有输入
		i = _getch();
	if (i == SPEED)
		p1_speedup++;
	if ((i == up || i == down || i == right || i == left) && (abs(*dir - i) != 4 && abs(*dir - i) != 3)) //不能回头
		*dir = i;
	if (p1_speedup % 2 == 0)  //没有加速
	{
		move_snake2(*dir);
		Sleep(100);
	}
	else
	{
		move_snake2(*dir);  //加速
		Sleep(50);
	}
}
//单人存活
int single_isalive1(LinkList L) //判断蛇死活的函数
{
	int lives_state = 0; //0为游戏正常运行 ，3为自己输
	LinkList p = NULL;
	for (p = L->next; p != NULL; p = p->next)   //不撞到自己
		if (cor_cmp(p->cor, L->cor))
			lives_state = 3;
	thru(L);//穿越边界
	return lives_state;
}
//单人游戏
void singlegame()
{
	//初始化数据
	p1_speedup = 0;
	p1_score = 0;
	int player_dir = right;  //初始化方向
	int result = 0;  //游戏结果
	foodscore = 2;

	setbkcolor(0xcdd2cf);  //设置字的背景
	cleardevice();

	loadimage(NULL, _T("2.jpg"));//打印背景
	init_game();  //初始化界面

				  //消除p1
	setfillcolor(Azure);
	drawdre(player1_head->cor);

	getrank(); //获取排行榜
	displayrank();  //刷新排行榜

	while (1)
	{
		single_gamemove(&player_dir);   //蛇移动
		gamescore({ P2_ScoreBoard.X,P2_ScoreBoard.Y - 25 }, p1_score);  //分数板
		if (result = single_isalive1(player2_head)) //isalive结果不为0则停止游戏
			break;
	}
	gameresult(result); //获取游戏结果
}

int main()
{
	char s1[5], s2[5];  //返回用户输入
	int flag = 1; //标识
	int x;


	while (flag)  //登陆
	{
		InputBox(s2, 5, _T("请输入数字1.注册 2.登陆 3.退出"));
		sscanf_s(s2, "%d", &x);
		switch (x)
		{
		case 1: {registers();//外接注册函数

			flag = 1;
			break;
		}
		case 2: {login();//外接登陆函数
			flag = 0;
			break;
		case 3: {
			exit(-1);
		}
		}
		default: {::MessageBox(NULL, "请输入正确的数值", "错误", MB_SETFOREGROUND); break; }
		}
	}
	initgraph(1260, 960);//初始化图形界面
	loadimage(NULL, _T("C:\\3.jpg"));
	menu();

	//游戏结果
	_getch();
	closegraph();
	return 0;
}



//11.24 解决了一些bug和发现bug的人