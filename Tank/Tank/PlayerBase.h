#pragma once
#include "TankClass.h"
#include "struct.h"

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
	void GetKillEnemy(int&, int&);						// GameControl �ڵ���, ͨ�������� mBulletStruct.mKillId ���ݽ�ȥ
	bool IsShootCamp();						// ����Ƿ���д�Ӫ

private:
	void Move(int new_dir);					// ���ķ���, ���ƶ�. ͬʱ�������굽����������, 
	bool CheckMoveable( byte dir);			// ��⵱ǰ�����Ƿ�����ƶ�
	bool ShootBullet(int bullet_id);			// ���� id ���ӵ�[0,1]
	bool CheckBomb(int);						// ���ɷ�ը
	void ClearWallOrStone(int, int,int);				// ����ǽ����ʯͷ
	void SignBox_8(int x, int y, int val);						// ���̹�����ڵ� 8*8 �ĸ���

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
	int mTankX, mTankY;					// ̹�����ĵ�����
	byte mPlayerTankLevel : 2;				// [0-3] ̹�˼���,��õ��ߺ�����̹��
	byte mTankDir : 2;						// ��ǰ̹�˷���0-��,1-��,2��,3-��
	static int mDevXY[4][2];				// �ĸ������ƫ����
	int mSpeed[4];							// mSpeed * mDevXY �õ��˶��ٶ�, �±��Ӧ mPlayerTankLevel, ��ͬ�����ٶȲ�һ��

	//static IMAGE mBulletImage[4];			// �ĸ������ӵ�, 
	//static int mBulletSize[4][2];			// {{4,3},{3,4},{4,3},{3,4}} �ߴ�: ����-3*4 / ����-4*3
	//int mBulletX[2], mBulletY[2];			// �ӵ�����, һ���������; 3/4�����̹�˿��Է��������ӵ�
	//int mBulletDir[2];					// �ӵ�����

	BulletStruct mBulletStruct[2];			// �����ӵ�
	//int mKillEnemyId[2];					// ��¼ÿ���ӵ����еĵл� id, #define _NONE ��ʶû����
	int mBullet_1_counter;					// �ӵ� 1 �ļ���, �ӵ� 1 �����ú���ܷ����ӵ� 2
	bool mMoving;							// ָʾ̹���Ƿ��ƶ�, ���ݵ� GetTankImage() ��ȡ�ƶ���̹��

	BombStruct mBombS[2];					// ��ը�ṹ��
	BlastStruct mBlast[2];				// ̹�˱�ը�ṹ, ��������̹��ͬʱ��ը,����Ҫ����

	bool mIsShootCamp;					// �Ƿ���д�Ӫ
};