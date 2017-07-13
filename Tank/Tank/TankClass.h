#pragma once

/************ ����̹����Ϣ *********
* ���̹����Ϣ
* �л�̹����Ϣ
***********************************/
class TankInfo
{
public:
	TankInfo(byte enemy_kind, int enemy_level, bool notuse );	// enemy_kind [0-1]; enemy_level [0-4], notuse->�������̹��
	TankInfo(byte player, byte level);							// player [0-1]; level [0-3]
	~TankInfo();

	IMAGE GetTankImage( byte dir, int index = 0 );		// index ���Ų�ͬ������ͼƬ���� [0-1]
private:
	IMAGE mTankImage[4][2];			// �ĸ�����,һ������������ͼʵ��̹���ƶ�����.
};

/************ ���̹�˼��� **********/

class PlayerTank
{
public:
	PlayerTank(byte player);		// player [0-1]
	~PlayerTank();
	IMAGE GetTankImage( byte level, byte dir, bool moving = false );	// 

private:
	TankInfo* mTankInfo[4];			// 4 ������̹��
	byte mMoveIndex : 1;		// ����̹��ͼƬ���һ�������� �±�����
};

