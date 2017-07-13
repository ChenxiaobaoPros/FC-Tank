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
	EnemyBase(TANK_KIND kind, byte level, BoxMarkStruct*);	// kind[0-1]; level [0-4]
	virtual ~EnemyBase();
	bool ShowStar(const HDC& center_hdc, int& total );		// ��ʾ��˸�Ľ���, true-������ʾ, false-��ʾ���
	void TankMoving(const HDC& center_hdc);		// �л��ƶ�
	virtual void DrawTank(const HDC&) {}			// ������̹��
	bool ShootBullet();			// �����ӵ�
	void BulletMoving(const HDC& );			// �ӵ��ƶ�
	void Bombing(const HDC&);
	void BeKill();					// �л�������, ��� SignBox ���
	bool Blasting(const HDC& );		// ��ʾ̹�˱�ըͼ, true ��ʶ��ը��,GameControl ��ȡ����ֵȻ�󽫸õл�ɾ��
	bool IsShootCamp();				// �Ƿ���д�Ӫ, GeamControl ��ѭ�����
	int  IsShootToPlayer();			// ��ȡ�����е���� id

	int GetId();				// ���صл� id
	TANK_KIND GetKind();		// ���صл�����, �Ƿ��ǵ���̹��

	/*  GameControl ��ѭ����� PlayerBase ���ڵ�һ��ֵ
	* ���ݸ�ֵ�жϵ��øú���
	* ���������� flag = true, ���ел�ֹͣ�ƶ�
	* �� GameControl �ڼ���, ������ɺ�����Ϊ false �л���ʼ�ƶ�
	*/
	//static void SetPause(bool);			// �л���ͣ�ƶ�


private:
	void SignBox_4(int value);		// ��ǻ�ȡ�� 4*4 ��С�ĸ���Ϊ̹��;
	void SignBox_8(int, int, int value);
	bool CheckBox_8();	// ���ĳ�� box_8 �Ƿ���Է���̹��, ������ 16*16 ���ӵ����ĵ�, ��̹�����������ͬ
	bool CheckMoveable();			// 
	void RejustDirPosition();		// ���¶�λ̹�˷���, ����̹��λ��, �����ڸ�����
	bool CheckBomb();				// �ƶ��ӵ�
	void ShootWhat(int, int);		// ������к���

protected:
	int mEnemyId;				// ����л���л�
	TANK_KIND mEnemyTankKind;		// �л����, ����̹�˺���̹ͨ������, [0-1]
	byte mEnemyTankLevel : 2;	// �л�̹��4������ [0-3]
	bool mDied;					// �Ƿ񱻱�����, �����к�����Ϊ true, �л�����ֵ�����ƶ�
	bool mIsShootCamp;			// �Ƿ���д�Ӫ
	//TankInfo* mEnemyTank;		// ָ��̹����ϸ��Ϣ
	BoxMarkStruct* bms;			// ָ����ӱ�ǽṹ, �� GameControl ���ݽ���

	int mTankX, mTankY;			// ̹������, ̹�˵����ĵ�
	byte mTankDir : 2;			// ̹�˷���
	byte mTankImageIndex : 1;	// ̹���ƶ��л�ͼƬ
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
	//static bool mIsPause;				// ��һ�ȡʱ�ӵ���, ͨ�� GameControl �������ø�ֵ
	//static int mPauseCounter;			// ��ͣ���
};

// ǰ������̹ͨ��
class CommonTank : public EnemyBase
{
public:
	CommonTank(byte level, BoxMarkStruct* bm);
	void DrawTank(const HDC&);				// ������̹��
	TankInfo* mTank;			// ��ɫ̹��
};

// ǰ���ֵ���̹��
class PropTank : public EnemyBase
{
public:
	PropTank(byte level, BoxMarkStruct* bm);
	void DrawTank(const HDC&);		// ������̹��
	TankInfo* mTank[2];				// �洢��ɫ�ͺ�ɫ��̹��
	byte index_counter : 6;		// �±�������ɫ
};

// ���������̹�� (�����Ǻ�ƻ�, ��ͨ���̻ƻ�)
class BigestTank : public EnemyBase
{
public:
	BigestTank(TANK_KIND kind, BoxMarkStruct* bm);
	void DrawTank(const HDC&);	// ������̹��
	TankInfo* mTank[4];			// ��,���,��
	byte index_counter : 6;		// �±�������ɫ
};
