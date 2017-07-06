#pragma once


#define LENGTH		10										// ÿ��С����ռ�ݴ�С, 4 * 4 ��С�������һ��̹��
#define NUM			100										// �л�����(����Ϊȫ�ֱ���)

#define BLOCK		500										// �ϰ�����ֵ,�ɱ��ݻ�
#define STONE		501										// ��ʯ��ǣ��ӵ�̹�˶����ܴ�Խ
#define CAMP		502										// ���Ѵ�Ӫ���
#define FOREST		503										// ɭ�ֱ�ǣ�̹������
#define RIVER		504										// ������ǣ�̹�˲��ܴ�Խ���ӵ����ԡ�

#define UP		0
#define DOWN	1
#define LEFT	2
#define RIGHT	3

enum KIND { MASTER, ENEMY /*, _TURRET, _TRANSFER */};						// �������л���������������

struct NEST												// ��Ѩ
{
	int Nx, Ny, Np;										// ������Ѩ����
	IMAGE Nimg[20];										// ������תͼƬ
	DWORD Nt1, Nt2, Ndt;								// ���ͼƬ�����������תЧ��

}Nest;

///////////////////////////////


struct TANK
{
	IMAGE Timg[4];										// �����ĸ�����̹��
	IMAGE Bimg[4];										// �����ĸ������ӵ�

	int X, Y;											// ̹�����Ͻ�����
	byte DIR  : 2;										// ����ֻ����λ�����Ʊ�ʾ��ֻ��ȡ0,1,2,3��
	KIND kind;											// 0 Ϊ������1 Ϊ�л�
	bool shoot, live;									// 
	//bool besend;										// �Ƿ��ڱ�����ing�����͹��������˺�
	bool appear;										// �Ƿ��������Ļ

	DWORD T1, T2, DRCT;									// �л��ƶ�����������ò���!!!
	int step;											// һ��������������ò���!!!
	DWORD Freq, Ft1, Ft2;								// �����ӵ�����������ò���!!!

	int b_x, b_y;										// �ӵ���������
	byte b_r : 2;										// �ӵ�����
	DWORD b_t1, b_t2, b_dt;								// �ӵ��ƶ��ٶ�

	static int DRCXY[4][2];								// ���������ƶ��ķ���
	static int T_Bult_of_Cent[4][2];					// ̹�˵��ӵ����ĵ��������
	static double Cent_Bult_of_LeUp[4][2];				// �ӵ����ĵ��ӵ����Ͻǵ��������
	static int HP;										// �������� 4

}Tank[NUM + 1];											// +1 ����
int TANK::DRCXY[4][2]					= { {0	 , -1}, {0	 ,  1}, {-1,	0}, { 1,	0} };	// 4 = ̹�� �� �ӵ��ķ���
int TANK::T_Bult_of_Cent[4][2]			= { {2	 ,  1}, {2	 ,  3}, { 1,	2}, { 3,	2} };	// 4 = ̹�˵�dir
double TANK::Cent_Bult_of_LeUp[4][2]	= { {-0.5, -1}, {-0.5, -1}, {-1, -0.5}, {-1, -0.5} };	// 4 = �ӵ��ķ���
int TANK::HP = 4;

////////////////////////////////////////

struct GAME												// ��ǰ��Ϸ״̬
{
	bool start, replay;
	int level;

}game;

////////////////////////////////////////

struct STATICPICTURE									// ��̬ͼƬ����ɭ�֡��������ϰ����ʯ
{
	IMAGE block, stone, forest, river;

}StaPic;

////////////////////////////////////////

struct LEVEL											// ��ǰ������Ϣ��̹����������ǰ��ʾ����
{
	static int base_tot_num;							// ����ֵ��tot_num �ڸû���������
	static int base_cur_num;							// ͬ��
	int next;											// ��һ��̹�˵� id
	int tot_num, cur_num;								// tot_num ���ܴ��� #define NUM !!!

}Level;
int LEVEL::base_tot_num = 30;
int LEVEL::base_cur_num = 4;
