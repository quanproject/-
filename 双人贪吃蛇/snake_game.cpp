//��Ҫ����ʵ�����  10.30
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<graphics.h>
#include<windows.h>
#include"snake.h"  //��defineͷ�ļ�


typedef struct Lnode {                                          //
	COORD cor;             //��¼����ı���                     //
	struct Lnode *next;                                         //
}LNode, *LinkList;

//��������   
void thru(LinkList L);
void init_game();
int menu();
void resistsgame();
void login();
COORD generate_food(COORD x);
void singlegame();
void savescore();

int player1_len = 1;  //�ߵĳ���
int player2_len = 1;

int p1_speedup = 0;  //������Ϊ�����ͼ�����1
int p2_speedup = 0;

int speedtime = 0;  //��������һ���߼���

int foodscore = 2; //���ӳ�ʼ����
int p1_score = 0;  //��ʼ����
int p2_score = 0;
int result = 0; //��Ϸ���
double gametimes = GAMETIME;  //��Ϸʱ��

COORD P1_ScoreBoard = { 34 * SIZE,6 * SIZE };//˫�˼Ƿְ�λ��
COORD P2_ScoreBoard = { 34 * SIZE,7 * SIZE + 20 };
COORD TIME = { 34 * SIZE,2 * SIZE + 11 };  //ʱ���λ��
COORD SCORE_BORAD = { 34 * SIZE,16 * SIZE };  //���˷�����λ��
COORD food, oldfood;  //��ʼʳ���λ��
LinkList player1_head, player2_head;  //������ͷָ��
user A, B;  //�û�
FILE *fp;  //�ļ�ָ��
user ranklist[10]; //���а������

				   //����ṹ                                                      //


				   //�߽紩Խ
void thru(LinkList L)  //��Խ�߽紫�ͺ���
{
	if (player1_head->cor.X < 0)
	{
		setfillcolor(p1ɫ);
		drawdre({ player1_head->cor.X + 32, player1_head->cor.Y });
		player1_head->cor.X = 31;
	}
	if (player2_head->cor.X < 0)
	{
		setfillcolor(p2ɫ);
		if (p2_speedup % 2 == 0)
			drawdre({ player2_head->cor.X + 32, player2_head->cor.Y });
		else
		{
			setfillcolor(p2����ɫ);
			drawdre({ player2_head->cor.X + 32, player2_head->cor.Y });
		}
		player2_head->cor.X = 31;
	}
	if (player1_head->cor.X >31)
	{
		setfillcolor(p1ɫ);
		drawdre({ player1_head->cor.X - 32, player1_head->cor.Y });
		setfillcolor(0xa2a2a6);//�ڸ�β�����һ��
		drawdre(player1_head->cor);
		player1_head->cor.X = 0;
	}

	if (player2_head->cor.X > 31)
	{
		setfillcolor(0xa2a2a6);
		drawdre(player2_head->cor);
		setfillcolor(p2ɫ);
		drawdre({ player2_head->cor.X - 32, player2_head->cor.Y });
		player2_head->cor.X = 0;
	}
	if (player1_head->cor.Y < 0)
	{

		setfillcolor(p1ɫ);
		drawdre({ player1_head->cor.X, player1_head->cor.Y + 32 });
		player1_head->cor.X = 31;
	}
	if (player2_head->cor.Y <0)
	{
		setfillcolor(p2ɫ);
		drawdre({ player2_head->cor.X, player2_head->cor.Y + 32 });
		player2_head->cor.Y = 31;

	}
	if (player2_head->cor.Y > 31)
	{
		setfillcolor(0xa2a2a6);
		drawdre(player2_head->cor);
		setfillcolor(p2ɫ);
		drawdre({ player2_head->cor.X, player2_head->cor.Y - 32 });
		player2_head->cor.Y = 0;
	}
	if (player1_head->cor.Y> 31)
	{
		setfillcolor(p1ɫ);
		drawdre({ player1_head->cor.X, player1_head->cor.Y - 32 });
		setfillcolor(0xa2a2a6);//�ڸ�β�����һ��
		drawdre(player1_head->cor);
		player1_head->cor.Y = 0;
	}

}
//��ʼ������
void init_game()
{
	player1_head = (LinkList)malloc(sizeof(LNode));  //����һ����ͷ
	player1_head->cor.X = 24;  //�ն���ͷλ��
	player1_head->cor.Y = 24;
	player1_head->next = NULL;  //ָ��Կշ���

	player2_head = (LinkList)malloc(sizeof(LNode));  //����һ����ͷ
	player2_head->cor.X = 12;  //�ն���ͷλ��
	player2_head->cor.Y = 12;
	player2_head->next = NULL;  //ָ��Կշ���

								//��ʼ��p1����ͷ
	setfillcolor(p1ɫ);
	drawdre(player1_head->cor);
	//��ʼ��p2����ͷ
	setfillcolor(p2ɫ);
	drawdre(player2_head->cor);
	//��ʼ��ʳ��
	generate_food({ 0, 0 });
	food = generate_food({ 0,0 });
	setlinecolor(BLUE);
	setfillcolor(BLACK);
	fillcircle(food.X*SIZE + SIZE / 2, food.Y*SIZE + SIZE / 2, SIZE / 2);




}
//����ʳ������
COORD generate_food(COORD x)  //�������ʳ�����꺯��
{
	COORD food_;
	LinkList p = NULL;   //ָ��Կշ���
	int in_snake;

	srand((unsigned int)time(NULL));

	do {
		in_snake = 0;
		food_.X = rand() % (frame_height - 2) + 1;
		food_.Y = rand() % (frame_width - 2) + 1;
		for (p = player1_head; p != NULL; p = p->next) //�ж��Ƿ���������1����
			if (cor_cmp(food_, p->cor))
				in_snake = 1;
		for (p = player2_head; p != NULL; p = p->next) //�ж��Ƿ���������2����
			if (cor_cmp(food_, p->cor))
				in_snake = 1;
		if (cor_cmp(x, food_))
			in_snake = 1;
	} while (in_snake);
	foodscore += 2; //ʳ���������
	return food_;
}
//�ƶ�p1��
void move_snake1(int dir)
{
	LinkList p, t = NULL;
	LinkList newp;
	newp = (LinkList)malloc(sizeof(LNode));
	newp->cor = player1_head->cor;        //��¼����ֵ
	newp->next = player1_head;    //����ͷ��nextָ��ָ���ͷ
	player1_head = newp;   //ͷȨ��λ
	int grow = 0;  //�ж����Ƿ�������flag
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
	if (cor_cmp(player1_head->cor, food)) 	//���ʳ���ͷ������ͬ
	{
		grow = 1; //������־

		if (p2_speedup % 2 == 0)  //û���������÷�
			p2_score += foodscore;
		else
			p2_score += foodscore * 2;  //�����÷�
		oldfood = food;
		food = generate_food(oldfood);
	}

	if (p2_speedup % 2 == 0)
	{
		setlinecolor(Azure);
		setfillcolor(p1ɫ);
		drawdre(player1_head->cor);
	}
	else
	{
		setlinecolor(Azure);
		setfillcolor(p1����ɫ);
		drawdre(player1_head->cor);
	}

	if (grow) {  //�䳤�Ļ����ٶ�������в���
		player1_len++;
		setlinecolor(BLUE);
		setfillcolor(BLACK);
		fillcircle(food.X*SIZE + SIZE / 2, food.Y*SIZE + SIZE / 2, SIZE / 2);

	}
	else {
		for (p = player1_head; p->next->next != NULL; p = p->next)
			;//ֱ���ߵ����ڶ���
		t = p->next;//��¼�����һ�ڵĵ�ַ
		p->next = NULL;
		setfillcolor(Azure);//�ڸ�β�����һ��
		drawdre(t->cor);
		free(t);//�ͷ��ڴ�ռ�
	}

}
//�ƶ�p2��
void move_snake2(int dir)
{
	LinkList p, t = NULL;
	LinkList newp;
	newp = (LinkList)malloc(sizeof(LNode));
	newp->cor = player2_head->cor;        //��¼����ֵ
	newp->next = player2_head;    //����ͷ��nextָ��ָ���ͷ
	player2_head = newp;   //ͷȨ��λ
	int grow = 0;  //�ж����Ƿ�������flag
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
	if (cor_cmp(player2_head->cor, food)) 	//���ʳ���ͷ������ͬ
	{
		grow = 1;
		if (p1_speedup % 2 == 0)  //û���������÷�
			p1_score += foodscore;
		else
			p1_score = p1_score + foodscore * 2;  //�����÷�
		oldfood = food;
		food = generate_food(oldfood);
	}

	if (p1_speedup % 2 == 0)
	{
		setlinecolor(BLUE);
		setfillcolor(p2ɫ);		drawdre(player2_head->cor);
	}
	else
	{
		setlinecolor(BLUE);
		setfillcolor(p2����ɫ);
		drawdre(player2_head->cor);
	}
	if (grow) {  //�䳤�Ļ����ٶ�������в���
		player1_len++;
		setlinecolor(BLUE);
		setfillcolor(BLACK);
		fillcircle(food.X*SIZE + SIZE / 2, food.Y*SIZE + SIZE / 2, SIZE / 2);
	}
	else {
		for (p = player2_head; p->next->next != NULL; p = p->next)
			;//ֱ���ߵ����ڶ���
		t = p->next;//��¼�����һ�ڵĵ�ַ
		p->next = NULL;
		setfillcolor(Azure);
		drawdre(t->cor);
		free(t);//�ͷ��ڴ�ռ�
	}
}
//��ȡ�ƶ�����
void gamemove(int *dir1, int *dir2)  //11.13�޸�
{
	int i = 0;  //����ֵ
	int flag = 0;  //�����������ݵĿ��ƶ���


	if (_kbhit())  //���������
		i = _getch();
	if (i == up || i == down || i == right || i == left)
		flag = 2;  //aswd���Ƶ���
	if (i == UP || i == DOWN || i == RIGHT || i == LEFT)
		flag = 1;  //jkli���Ƶ���
	if (i == speed)
		p2_speedup++;
	if (i == SPEED)
		p1_speedup++;

	if (flag == 2)  //��2��������
	{
		if ((abs(i - *dir2) == 4) || (abs(i - *dir2) == 3)) //�߲��ܻ�ͷ
			i = *dir2;
		move_snake2(i);
		move_snake1(*dir1);
		*dir2 = i;
		fflush(stdin);
		Sleep(100);
	}


	if (flag == 1)  //��1��������
	{
		if (abs(i - *dir1) == 2)   //�߲��ܻ�ͷ
			i = *dir1;
		move_snake1(i);  //�ƶ�������
		move_snake2(*dir2);  //��һ���߱��ַ����ƶ�
		*dir1 = i;  //����ɵķ���׼����һ�αȽ�
		fflush(stdin);
		Sleep(100);
	}


	if (flag == 0)  //û�в������߳�������ԭ�����ƶ�
	{
		if (p1_speedup % 2 == 0 && p2_speedup % 2 == 0)  //��������
		{
			move_snake1(*dir1);
			move_snake2(*dir2);
			fflush(stdin);
			Sleep(100);
		}
		else if (p1_speedup % 2 != 0 && p2_speedup % 2 == 0)
		{
			move_snake2(*dir2);
			if (speedtime++ % 2 == 0)  //�ı�������һ����
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
//�ж�����
int isalive(LinkList L, LinkList Z) //11.18 �޸�
{
	int lives_state = 0; //0Ϊ��Ϸ�������� ��1Ϊ�Լ��䣬2Ϊƽ��,3Ϊp1Ӯ��4Ϊp2Ӯ
	LinkList p = NULL;
	for (p = L->next; p != NULL; p = p->next)   //��װ���Լ�
		if (cor_cmp(p->cor, L->cor))
			lives_state = 1;
	for (p = Z->next; p != NULL; p = p->next)   //��װ�����ˣ���
	{
		if ((cor_cmp(p->cor, L->cor)) && (p1_speedup + p2_speedup) % 2 == 0)  //����ͬ״̬��
			lives_state = 1;
		else if ((cor_cmp(p->cor, L->cor)) && p2_speedup % 2 != 0)  //p2��������
			lives_state = 3;
		else if ((cor_cmp(p->cor, L->cor)) && p1_speedup % 2 != 0) //p1��������
			lives_state = 4;
	}
	thru(L);//��Խ�߽�
	if (cor_cmp(L->cor, Z->cor))
		lives_state = 2;
	//***********�ƶ��ϰ����޸Ĵ�**************** 10.18
	return lives_state;
}
//��ʾ����
void gamescore(COORD x, int playerscore)
{
	char score[255];
	sprintf(score, "%d", playerscore);  //ת����������
	setfillcolor(WHITE);
	settextstyle(40, 0, _T("����"));
	//	settextcolor(BLACK);  //������ɫ
	RECT r2 = { x.X, x.Y, x.X + 180,x.Y + 90 };

	drawtext(score, &r2, DT_VCENTER || DT_SINGLELINE);
}
//��ʾʱ��
int gametime()  //11.13�޸�
{
	static int flag = 0;
	if (p1_speedup % 2 == 0 && p2_speedup % 2 == 0) //��û�м���ʱʱ����������
		gametimes -= 1;
	else        //����ʱ�����ٶȼ��루��Ӧ˫����
		gametimes -= 0.5;

	if (gametimes <= GAMETIME / 4 && flag == 0)
	{
		foodscore *= 2;
		flag = 1;
	}
	int a = gametimes / 10;
	char time[200];   //Ҫ�����ʱ�䣨char
	sprintf(time, "%d", a);  //ת����������
	settextcolor(BLACK);  //������ɫ
	RECT r1 = { TIME.X, TIME.Y, TIME.X + 180, TIME.Y + 40 }; //ȷ��ʱ��λ��
	drawtext(time, &r1, DT_NOCLIP || DT_VCENTER || DT_SINGLELINE); //����ʱ��
	return a;
}
//��Ϸ���
void gameresult(int result) //11.18 �޸�
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
		outtextxy(500, 200, _T("����൱�Ķ���!"));
	}
	if (result == 3)  //������Ϸ����ɼ�
	{
		settextcolor(YELLOW);
		outtextxy(500, 200, _T("YOU LOSE"));
		savescore();   //����ɼ�
		flag = 2;
	}
	while (flag == 1)//˫��ģʽ
	{
		InputBox(str, 5, _T("��Ϸ����1.���������� 2.����һ��"));//�������ѡ��
		sscanf_s(str, "%d", &x);//ת������
		switch (x)
		{
		case 1: {menu(); flag = 0; break; }
		case 2: {resistsgame(); flag = 0;  break; }
		default: {::MessageBox(NULL, "��������ȷ����ֵ", "����", MB_SETFOREGROUND); break; }
		}
	}
	while (flag == 2)//����ģʽ
	{
		InputBox(str, 5, _T("��Ϸ����1.���������� 2.����һ��"));//�������ѡ��
		sscanf_s(str, "%d", &x);//ת������
		switch (x)
		{
		case 1: {menu(); flag = 0; break; }
		case 2: {singlegame(); flag = 0;  break; }
		default: {::MessageBox(NULL, "��������ȷ����ֵ", "����", MB_SETFOREGROUND); break; }
		}
	}
}
//���������а���ʾ
void ranking()
{
	int i = 130;
	char str[10], str1[10];
	loadimage(NULL, _T("mainrank.jpg"));

	settextcolor(WHITE);
	settextstyle(30, 0, _T("����"));
	outtextxy(1020, i, "�÷�");
	outtextxy(1120, i, "ID");
	i += 85;
	fopen_s(&fp, "score.txt", "r");
	for (int NUM = 0; NUM < 9; NUM++)//���ļ��ж�ȡʮ������,�����
	{
		fscanf_s(fp, "%s%s", str, sizeof(str), str1, sizeof(str1));

		char no[5];   //Ҫ�����λ��
		sprintf(no, "%d", NUM + 1);  //ת����������

		settextstyle(20, 0, _T("����"));

		outtextxy(1000, i, no);
		outtextxy(1020, i, str);
		outtextxy(1120, i, str1);
		i += 75;
	}
	fclose(fp);
	getch();
	menu();
}
//��ȡ���а�
void getrank()
{
	char transcore[10];
	fopen_s(&fp, "score.txt", "r");
	for (int i = 0; i < 10; i++)
	{
		fscanf_s(fp, "%s%s", transcore, sizeof(transcore), ranklist[i].id, sizeof(ranklist[i].id));
		sscanf(transcore, "%d", &ranklist[i].score);//�ַ���ת��Ϊ�������ݲ�����
	}
	fclose(fp);
}
//����
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
//ˢ�����а�
void displayrank()
{
	//sort(ranklist);
	int i = 330;
	for (int NUM = 0; NUM < 9; NUM++)//���ļ��ж�ȡʮ������,�����
	{
		char no[5], score[10];   //Ҫ�����λ��
		sprintf(no, "%d", NUM + 1);  //ת����������
		sprintf(score, "%d", ranklist[NUM].score);  //ת����������
		settextstyle(20, 0, _T("����"));

		outtextxy(1020, i, no);
		outtextxy(1050, i, score);
		outtextxy(1150, i, ranklist[NUM].id);
		i += 40;
	}
}
//ע��
void registers()//ע���˺�����
{
	fopen_s(&fp, "user.txt", "r");
	fscanf_s(fp, "%s%s", B.id, sizeof(B.id), B.password, sizeof(B.password));
	InputBox(A.id, 11, "ע���������˺�");
	while (1)
	{
		if (strcmp(A.id, B.id) != 0)//��������ַ��������
		{
			if (!feof(fp))//���δ���ļ�ĩβ  
			{
				fscanf_s(fp, "%s%s", B.id, sizeof(B.id), B.password, sizeof(B.password));
			}
			else break;
		}
		else {
			::MessageBox(NULL, "�û����ѱ�ע�ᣡ", "����", MB_SETFOREGROUND);
			fclose(fp);
			return;  //����ѡ��ע���½
		}
	}
	fclose(fp);
	InputBox(A.password, 10, "ע������������");
	fopen_s(&fp, "user.txt", "a");
	fprintf_s(fp, "%s %s\n", A.id, A.password);
	::MessageBox(NULL, "��ϲ��ע��ɹ���", "��ϲ��", MB_SETFOREGROUND);
	fclose(fp);

}
//��½
void login()//��½�˺�����
{
	fopen_s(&fp, "user.txt", "r");
	fscanf_s(fp, "%s%s", B.id, sizeof(B.id), B.password, sizeof(B.password));
	InputBox(A.id, 11, "��¼�������˺�");
	while (1)
	{
		if (strcmp(A.id, B.id) == 0) break;//����ҵ�������û���  
		else {
			if (!feof(fp))//����ļ�δ����   
				fscanf_s(fp, "%s%s", B.id, sizeof(B.id), B.password, sizeof(B.password));
			else
			{
				::MessageBox(NULL, "�û�������", "����", MB_SETFOREGROUND);
				login();

			}
		}
	}
	InputBox(A.password, 20, "��½����������");
	if (strcmp(A.password, B.password) == 0)//�������ƥ�� 
		fclose(fp);
	else {

		::MessageBox(NULL, "�������", "����", MB_SETFOREGROUND);
		login();


	}
}
//��ʼѡ��˵�
int menu()
{
	int dir;
	char s1[5], s2[5];
	int flag = 1;
	int x, y[10];  //xע���½ѡ�����
	setbkcolor(TRANSPARENT);

	loadimage(NULL, _T("main.jpg"));

	//��������
	settextstyle(40, 0, _T("����"));
	settextcolor(BLUE);

	int ox = 1000;
	outtextxy(ox, 200, _T("������Ϸ"));//�˵������������
	settextcolor(RED);
	outtextxy(ox, 400, _T("˫����Ϸ"));
	outtextxy(ox, 600, _T("���а�"));
	outtextxy(ox, 800, _T("�˳���Ϸ"));
	while (1)
	{
		if (_kbhit)
		{//����flagֵ�����ƶ�

			dir = _getch();
			if (dir == 72 && flag != 1)
				flag--;
			if (dir == 80 && flag != 4)
				flag++;
			if (dir == 32)
				break;
			settextcolor(RED);
			outtextxy(ox, 200, _T("������Ϸ"));//�˵������������
			outtextxy(ox, 400, _T("˫����Ϸ"));
			outtextxy(ox, 600, _T("���а�"));
			outtextxy(ox, 800, _T("�˳���Ϸ"));
			switch (flag)
			{
			case 1: settextcolor(BLUE);
				outtextxy(ox, 200, _T("������Ϸ"));
				break;

			case 2: settextcolor(BLUE);
				outtextxy(ox, 400, _T("˫����Ϸ"));
				break;
			case 3:settextcolor(BLUE);
				outtextxy(ox, 600, _T("���а�"));
				break;
			case 4: settextcolor(BLUE);
				outtextxy(ox, 800, _T("�˳���Ϸ"));
				break;
			default: break;
			}
		}
	}
	if (flag == 1)//������Ϸ
	{
		singlegame();
	}

	if (flag == 2)//˫����Ϸ
	{
		resistsgame();
	}
	if (flag == 3)//���а�
	{
		ranking();
	}

	return 0;
}
//����ɼ�
void savescore()
{
	strcpy(ranklist[9].id, A.id); //��¼��ǰ�������
	ranklist[9].score = p1_score;
	sort(ranklist);

	fopen_s(&fp, "score.txt", "w+");
	for (int i = 0; i < 9; i++)
	{
		char outscore[10];
		sprintf(outscore, "%d", ranklist[i].score);  //ת����������

		fprintf_s(fp, "%s %s\n", outscore, ranklist[i].id);//���÷ֺ��û�����¼��txt
	}
	fclose(fp);
}
//˫����Ϸ
void resistsgame()
{
	//��������
	p1_speedup = 0;//����״̬
	p2_speedup = 0;
	p1_score = 0;  //�÷�
	p2_score = 0;
	foodscore = 2; //���ӷ���

	gametimes = GAMETIME;  //��ʼ����Ϸʱ��
	init_game();


	setbkcolor(0xcdd2cf);  //�����ֱ���
	cleardevice();

	loadimage(NULL, _T("1.jpg"));  //���ر���ͼƬ

	int player1_dir = RIGHT, player2_dir = right;  //��1,��2�������
	int result = 0;  //��Ϸ���
	init_game();
	::MessageBox(NULL, "׼����", "˫��ģʽ", MB_SETFOREGROUND);

	while (1)
	{
		gamemove(&player1_dir, &player2_dir); //���ƶ�
		settextcolor(p1ɫ);
		gamescore(P2_ScoreBoard, p2_score);  //���߷�����
		settextcolor(p2ɫ);
		gamescore(P1_ScoreBoard, p1_score);
		if ((result = isalive(player1_head, player2_head)) || isalive(player2_head, player1_head)) //isalive�����Ϊ0��ֹͣ��Ϸ
			break;
		if (!gametime()) //ʱ�����
		{
			if (p1_score >= p2_score)
				result = 1;
			break;
		}
	}
	gameresult(result);
}
//�����ƶ�
void single_gamemove(int *dir)  //���Ƽ��������ȡ�����ƶ�
{
	int i = 0;
	if (_kbhit())  //���������
		i = _getch();
	if (i == SPEED)
		p1_speedup++;
	if ((i == up || i == down || i == right || i == left) && (abs(*dir - i) != 4 && abs(*dir - i) != 3)) //���ܻ�ͷ
		*dir = i;
	if (p1_speedup % 2 == 0)  //û�м���
	{
		move_snake2(*dir);
		Sleep(100);
	}
	else
	{
		move_snake2(*dir);  //����
		Sleep(50);
	}
}
//���˴��
int single_isalive1(LinkList L) //�ж�������ĺ���
{
	int lives_state = 0; //0Ϊ��Ϸ�������� ��3Ϊ�Լ���
	LinkList p = NULL;
	for (p = L->next; p != NULL; p = p->next)   //��ײ���Լ�
		if (cor_cmp(p->cor, L->cor))
			lives_state = 3;
	thru(L);//��Խ�߽�
	return lives_state;
}
//������Ϸ
void singlegame()
{
	//��ʼ������
	p1_speedup = 0;
	p1_score = 0;
	int player_dir = right;  //��ʼ������
	int result = 0;  //��Ϸ���
	foodscore = 2;

	setbkcolor(0xcdd2cf);  //�����ֵı���
	cleardevice();

	loadimage(NULL, _T("2.jpg"));//��ӡ����
	init_game();  //��ʼ������

				  //����p1
	setfillcolor(Azure);
	drawdre(player1_head->cor);

	getrank(); //��ȡ���а�
	displayrank();  //ˢ�����а�

	while (1)
	{
		single_gamemove(&player_dir);   //���ƶ�
		gamescore({ P2_ScoreBoard.X,P2_ScoreBoard.Y - 25 }, p1_score);  //������
		if (result = single_isalive1(player2_head)) //isalive�����Ϊ0��ֹͣ��Ϸ
			break;
	}
	gameresult(result); //��ȡ��Ϸ���
}

int main()
{
	char s1[5], s2[5];  //�����û�����
	int flag = 1; //��ʶ
	int x;


	while (flag)  //��½
	{
		InputBox(s2, 5, _T("����������1.ע�� 2.��½ 3.�˳�"));
		sscanf_s(s2, "%d", &x);
		switch (x)
		{
		case 1: {registers();//���ע�ắ��

			flag = 1;
			break;
		}
		case 2: {login();//��ӵ�½����
			flag = 0;
			break;
		case 3: {
			exit(-1);
		}
		}
		default: {::MessageBox(NULL, "��������ȷ����ֵ", "����", MB_SETFOREGROUND); break; }
		}
	}
	initgraph(1260, 960);//��ʼ��ͼ�ν���
	loadimage(NULL, _T("C:\\3.jpg"));
	menu();

	//��Ϸ���
	_getch();
	closegraph();
	return 0;
}



//11.24 �����һЩbug�ͷ���bug����