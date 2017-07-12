#pragma once
#include "TankClass.h"
#include "struct.h"


/************** �л�̹�� ************
* һ���л�ʵ����һ������
* �ṩ�л����(�Ƿ��ǵ���̹��), �л�����[0-4]
* Ĭ������ 0-0 �л�, ��0���ĵ���̹��
************************************/
class EnemyBase
{
public:
	EnemyBase(byte kind, int level, BoxMarkStruct*);	// kind[0-1]; level [0-4]
	~EnemyBase();
	bool ShowStar(const HDC& center_hdc, int& total );		// ��ʾ��˸�Ľ���, true-������ʾ, false-��ʾ���
	void TankMoving(const HDC& center_hdc);		// �л��ƶ�
	bool ShootBullet();			// �����ӵ�
	void BulletMoving(const HDC& );			// �ӵ��ƶ�
	void Bombing(const HDC&);
	void BeKill();					// �л�������, ��� SignBox ���
	bool Blasting(const HDC& );		// ��ʾ̹�˱�ըͼ, true ��ʶ��ը��,GameControl ��ȡ����ֵȻ�󽫸õл�ɾ��
	bool IsShootCamp();				// �Ƿ���д�Ӫ, GeamControl ��ѭ�����
	int  IsShootToPlayer();			// ��ȡ�����е���� id

	int GetId();				// ���صл� id

private:
	void SignBox_4(int value);		// ��ǻ�ȡ�� 4*4 ��С�ĸ���Ϊ̹��;
	void SignBox_8(int, int, int value);
	bool CheckBox_8();	// ���ĳ�� box_8 �Ƿ���Է���̹��, ������ 16*16 ���ӵ����ĵ�, ��̹�����������ͬ
	bool CheckMoveable();			// 
	void RejustDirPosition();		// ���¶�λ̹�˷���, ����̹��λ��, �����ڸ�����
	bool CheckBomb();
	void ShootWhat(int, int);		// ������к���

private:
	int mEnemyId;				// ����л���л�
	byte mEnemyTankKind;		// �л����, ����̹�˺���̹ͨ������, [0-1]
	int mEnemyTankLevel;		// �л�̹��4������ [0-3]
	bool mDied;					// �Ƿ񱻱�����, �����к�����Ϊ true, �л�����ֵ�����ƶ�
	bool mIsShootCamp;			// �Ƿ���д�Ӫ
	TankInfo* mEnemyTank;		// ָ��̹����ϸ��Ϣ
	BoxMarkStruct* bms;			// ָ����ӱ�ǽṹ, �� GameControl ���ݽ���

	int mTankX, mTankY;			// ̹������, ̹�˵����ĵ�
	byte mTankDir : 2;			// ̹�˷���
	byte mTankImageIndex : 1;	// ̹���ƶ��л�ͼƬ
	/*
	IMAGE mStarImage[4];		// �Ľ���ͼƬ
	int mStarIndexDev ;			// �����ı仯��, -1, 1  -1��star��С���, 1 ��star�ɴ��С
	byte mStarIndex : 2;		// �Ľ����±������仯���� 0-1-2-3-2-1-0-1-2-3-...
	int mStarCounter;			// ����,���ٴα��һ��ͼ��
	int mTankOutAfterCounter;	// һ���������֮��, �Ľ��ǿ�ʼ��˸,̹�˳���
	bool mIsOuted;				// �Ľ���Сʱ��̹�˳���, ֹͣ�����Ľ�����˸ͼ
	*/
	int mStep;					// ��ǰ�����ƶ��Ĳ���, һ����������������ϰ���任�������¼���;
	static int mDevXY[4][2];	// �ĸ������ƫ����
	int mSpeed[4];					// mSpeed * mDevXY �õ��˶��ٶ�, �±��Ӧ mPlayerTankLevel, ��ͬ�����ٶȲ�һ��

	bool mTankNumberReduce;		// ���Ľ��ǿ�ʼ, true-̹��������һ,Ȼ�����ֵ=false, ֻ��һ��

	BulletStruct mBulletStruct;
	DWORD mBulletT1, mBulletT2;	// ������, ��÷���һ���ӵ�
	DWORD mBulletT;

	BombStruct mBombS;			// ��ը�ṹ��
	BlastStruct mBlast;			// ̹�˱�ը�ṹ

	int mShootedPlayerID;		// ��������ҵ�id
	StarClass mStar;			// �Ľ�����˸��
};