//2018.10.18      by quan

//�ĸ�����궨��
//player 1
#define UP 105  //i ��
#define DOWN 107  //k ��
#define LEFT 106  //j ��
#define RIGHT 108  //l ��

#define SPEED 101 //e ����

//player 2
#define up 119  //w ��
#define down 115  //s ��
#define left 97  //a ��
#define right 100  //d ��

#define speed 117 //u ����

#define SIZE 30 //ÿ�����С
#define frame_height 32//��ͼ�߸���
#define frame_width 32 //��ͼ������
#define GAMETIME 610 //��Ϸʱ��

//��Ϸ�ڸ���������

#define SNAKE 1 //��1
#define snake 10  //��2
#define FOOD 2  //ʳ��
#define BAR 3   //Χǽ
#define OBSTACLE 4  //�ϰ���
// 0 �ǿյ�ͼ

#define Azure 0xe4e6e5//����ɫ
#define p1ɫ  0x7d62da 
#define p1����ɫ 0x6038a5
#define p2ɫ 0xb0a458
#define p2����ɫ 0x513f37

//����ȽϺ���
int cor_cmp(COORD pt1, COORD pt2)
{
	return (pt1.X == pt2.X&&pt1.Y == pt2.Y); //��ȱ㷵��1
}

//�û����ṹ��
typedef struct Users
{
	char id[10];
	char password[10];
	int score = 0;
}user;

//��������
void const drawdre(COORD x)
{
	solidrectangle(x.X*SIZE, x.Y* SIZE, x.X*SIZE + SIZE, x.Y* SIZE + SIZE);
}