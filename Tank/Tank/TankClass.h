#pragma once

/************ ����̹����Ϣ *********
* ���̹����Ϣ
* �л�̹����Ϣ
***********************************/
class TankInfo
{
public:
	TankInfo();
	TankInfo(byte enemy_kind, int enemy_level, bool notuse );	// enemy_kind [0-1]; enemy_level [0-4], notuse->�������̹��
	TankInfo(byte player, byte level);							// player [0-1]; level [0-3]
	~TankInfo();

	IMAGE GetTankImage( byte dir );
private:
	IMAGE mTankImage[4][2];		// �ĸ�����,һ������������ͼʵ��̹���ƶ�����.
};

/************ ���̹�˼��� **********/

class PlayerTank
{
public:
	PlayerTank(byte player);		// player [0-1]
	~PlayerTank();
	IMAGE GetTankImage( byte level, byte dir );
private:
	TankInfo* mTankInfo[4];			// 4 ������̹��
};

/************** �л�̹�� ************
* һ���л�ʵ����һ������
* �ṩ�л����(�Ƿ��ǵ���̹��), �л�����[0-4]
* Ĭ������ 0-0 �л�, ��0���ĵ���̹��
************************************/
class EnemyTank
{
public:
	EnemyTank();
	EnemyTank(byte kind, int level);	// kind[0-1]; level [0-4]
	~EnemyTank();
private:
	byte mEnemyTankKind;		// �л����, ����̹�˺���̹ͨ������, [0-1]
	int mEnemyTankLevel;		// �л�̹��5������ [0-4]
	TankInfo* mEnemyTank;		// ָ��̹����ϸ��Ϣ
};
