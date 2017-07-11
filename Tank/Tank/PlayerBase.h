#pragma once
#include "TankClass.h"

#define SHOOTABLE_X		-100	// �涨�ӵ����� x = -100 �ӵ����Է���

struct BulletStruct
{
	int x, y;					// �ӵ�����, ���ݲ�ͬ����ָ����ͬ��λ�ô��� x,y, ָ�� x=SHOOTABLE_X ��ʱ����Է����ӵ�
	int dir;					// �ӵ�����
	int speed[4];				// �ӵ��ٶ�, ����̹�˼�����費ͬ�ٶ�ϵ��. ÿ���ƶ����ܳ���4�����ص�!! ��Ȼ���Խ 4*4 �ĸ���!!���bug
	
	static IMAGE mBulletImage[4];		// ͼƬ
	static int mBulletSize[4][2];		// {{4,3},{3,4},{4,3},{3,4}} �ߴ�: ����-3*4 / ����-4*3
	static int devto_tank[4][2];		// �涨�ӵ������������tank���ĵ��ƫ����
	static int devto_head[4][2];		// �涨�ӵ�ͼƬ���Ͻ�����ڵ�ͷ�����ƫ����;���·���ͷ�����ڵ�ͷ���ұ�;�������ڵ�ͷ�������Ǹ���
	//static int bomb_box[4][2];			// ��ͷ��Χ�ĸ� 1/4 ���ӵ�ƫ����, �����ĸ�����,�ж��Ƿ�ը
};

struct BombStruct
{
	static IMAGE mBombImage[3];				// �ӵ���ըͼ
	int mBombX, mBombY;						// ��ը����������
	bool canBomb;							// �Ƿ�ʼ��ʾ��ըͼƬ flag
	int counter;						// ȡģ������, ���ٴ�ѭ������һ��ͼƬ
};

/************* ��ҿ��� *************
* һ�����ʵ����һ������
* ���Թ����Լ�������
*/

class PlayerBase
{
public:
	PlayerBase(byte player, BoxMarkStruct*);						// player [0-1]
	~PlayerBase();
	void PlayerLoop(const HDC&);							// ���ѭ��
	void DrawPlayerTankIco(const HDC& );					// �����Ҳ�����\2P\1P\̹��ͼ��\ʣ������ֵ
	void DrawPlayerTank( const HDC& );						// �������̹��
	bool PlayerControl();				// ��ҿ���̹���ƶ�
	void BulletMoving(const HDC&);							// �ӵ��ƶ�, ��GameControl ��ѭ������
	void Bombing(const HDC&);							// ��ը

private:
	void Move(int new_dir);					// ���ķ���, ���ƶ�. ͬʱ�������굽����������, 
	bool CheckMoveable( byte dir);			// ��⵱ǰ�����Ƿ�����ƶ�
	bool ShootBullet(int bullet_id);			// ���� id ���ӵ�[0,1]
	bool CheckBomb(int);						// ���ɷ�ը

private:
	byte player_id : 1;						// [0-1] ���
	PlayerTank* mPlayerTank;				// ̹����
	BoxMarkStruct* bms;

	IMAGE m12PImage;						// 1P\2P ͼ��
	int m12PImage_x, m12PImage_y;			// ͼ������

	IMAGE mPlayerTankIcoImage;				// ���̹��ͼ��
	int   mPlayerTankIcoImage_x,			// ͼ������
		  mPlayerTankIcoImage_y;

	int mPlayerLife;						// �������, Ĭ��3
	int mPlayerLife_x, 						// ͼ������
		mPlayerLife_y;

	IMAGE mBlackNumberImage;				// 0123456789 ��ɫ����
	int mTankX, mTankY;						// ̹�����ĵ�����
	byte mPlayerTankLevel : 2;				// [0-3] ̹�˼���,��õ��ߺ�����̹��
	byte mTankDir : 2;						// ��ǰ̹�˷���0-��,1-��,2��,3-��
	static int mDevXY[4][2];				// �ĸ������ƫ����
	int mSpeed[4];							// mSpeed * mDevXY �õ��˶��ٶ�, �±��Ӧ mPlayerTankLevel, ��ͬ�����ٶȲ�һ��

	//static IMAGE mBulletImage[4];			// �ĸ������ӵ�, 
	//static int mBulletSize[4][2];			// {{4,3},{3,4},{4,3},{3,4}} �ߴ�: ����-3*4 / ����-4*3
	//int mBulletX[2], mBulletY[2];			// �ӵ�����, һ���������; 3/4�����̹�˿��Է��������ӵ�
	//int mBulletDir[2];					// �ӵ�����

	
	BulletStruct mBulletStruct[2];			// �����ӵ�
	int mBullet_1_counter;					// �ӵ� 1 �ļ���, �ӵ� 1 �����ú���ܷ����ӵ� 2
	bool mMoving;							// ָʾ̹���Ƿ��ƶ�, ���ݵ� GetTankImage() ��ȡ�ƶ���̹��

	BombStruct mBombS[2];					// ��ը�ṹ��
};