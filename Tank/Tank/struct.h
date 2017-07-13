#pragma once

#define TOTAL_ENEMY_NUMBER	20
#define SHOWING_STAR	true		// ������ʾ�Ľ���
#define STOP_SHOW_STAR	false		// �Ľ�����ʾ����
enum TANK_KIND { PROP, COMMON };

struct BulletStruct
{
	int x, y;					// �ӵ�����, ���ݲ�ͬ����ָ����ͬ��λ�ô��� x,y, ָ�� x=SHOOTABLE_X ��ʱ����Է����ӵ�
	int dir;					// �ӵ�����
	int speed[4];				// �ӵ��ٶ�, ����̹�˼�����費ͬ�ٶ�ϵ��. ÿ���ƶ����ܳ���4�����ص�!! ��Ȼ���Խ 4*4 �ĸ���!!���bug

	/* �����һ��ел�,���ø�ֵΪ�л���id, GameControl ѭ���ڼ���ֵ, Ȼ��ɾ���õл�
	* ����ǵл��������, ���ø�ֵΪ���id, ��Ϸѭ�����ټ���ֵ�����������*/
	int mKillId;				// ��¼�ӵ����еĵл�/��� id

	static IMAGE mBulletImage[4];		// ͼƬ
	static int mBulletSize[4][2];		// {{4,3},{3,4},{4,3},{3,4}} �ߴ�: ����-3*4 / ����-4*3
	static int devto_tank[4][2];		// �涨�ӵ������������tank���ĵ��ƫ����
	static int devto_head[4][2];		// �涨�ӵ�ͼƬ���Ͻ�����ڵ�ͷ�����ƫ����;���·���ͷ�����ڵ�ͷ���ұ�;�������ڵ�ͷ�������Ǹ���
	static int bomb_center_dev[4][2];	// ��ը����������ӵ�ͷ��ƫ����
};

struct BombStruct
{
	static IMAGE mBombImage[3];				// �ӵ���ըͼ
	int mBombX, mBombY;						// ��ը����������
	bool canBomb;							// �Ƿ�ʼ��ʾ��ըͼƬ flag
	int counter;						// ȡģ������, ���ٴ�ѭ������һ��ͼƬ
};

class BlastStruct
{
public:
	BlastStruct();
	~BlastStruct() {}

	static IMAGE image[5];
	int blastx, blasty;			// ��������, 32*32
	bool canBlast;
	int counter;				// �������ٴλ�һ��ͼƬ
};

class StarClass
{
public:
	StarClass();
	static IMAGE mStarImage[4];	// �Ľ���ͼƬ, ����Ҹ�ֵ
	int mStarIndexDev;			// �����ı仯��, -1, 1  -1��star��С���, 1 ��star�ɴ��С
	byte mStarIndex : 2;		// �Ľ����±������仯���� 0-1-2-3-2-1-0-1-2-3-...
	int mStarCounter;			// ����,���ٴα��һ��ͼ��
	bool mIsOuted;				// �Ľ�����ʧ��ʱ��̹�˳���, ֹͣ�����Ľ�����˸ͼ

	// Enemy ר��! Player �ò���
	int mTankOutAfterCounter;	// һ���������֮��, �Ľ��ǿ�ʼ��˸,̹�˳���
};

// ���̹�˳��ֵ�ʱ����ʾ����˸��״����Ȧ
class RingClass
{
public:
	RingClass();
	void ShowRing(const HDC&, int centerx, int centery);		//
	void SetShowable();				// ���ÿ�����ʾ��״, PlayerBase �ڻ�ȡ���ߺ����
	static IMAGE image[2];
	bool canshow;			// �Ƿ������ʾ��״
	int index_counter;		// �仯�±�����
};

// ��������
#define ADD_PROP	0	// �ӻ�
#define STAR_PROP	1	// �����
#define TIME_PROP	2	// ʱ��
#define BOMB_PROP	3	// ����
#define SHOVEL_PRO	4	// ����
#define CAP_PROP	5	// ñ��

/*** ������,
* �� PlayerBase ʵ������Աָ��
* PlayerBase ���캯���ڵ��ø���һ���������� BoxMarkStuct* ����
* �������к������� PlayerBase �ڵ���
***/
class PropClass
{
	void SignPropBox(int val);			// ��� prop_8 ����

public:
	PropClass();
	void ShowProp(const HDC&);			// GameControl ��ѭ�����ú���
	void StartShowProp(int x, int y);	// ����̹�˱����к���øú���
	void StopShowProp();			// ֹͣ��ʾ����, ��ʱ���߱���һ��
	void SetBoxMarkStruct(BoxMarkStruct*);

private:
	BoxMarkStruct* bms;
	int leftx, topy;			// �������ĵ�����
	int index_counter = 0;			// �±�任����
	/*static */IMAGE image[6];
	/*static */int prop_kind;			// ��������
	bool can_show;				// �Ƿ������ʾ����	
};