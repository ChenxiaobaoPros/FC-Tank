#pragma once
#include "TankClass.h"

#define SHOOTABLE_X		-100	// �涨�ӵ����� x = -100 �ӵ����Է���
/* ��Ӧ��ֵ
* 	#defien _EMPTY		0
	#define _WALL		3
	#define _FOREST		1
	#define _ICE		2
	#define _RIVER		4
	#define _STONE		5
-------------------------------
* ��Ӧ�����ϵ
box[0][0-25] : ��һ��( y=0; x=[0-25] )
box[1][0-25] : �ڶ���( y=1; x=[0-25] )
...
box[y/BOX_SIZE][x/BOX_SIZE]
-------------------------------
*/
struct BoxMarkStruct
{
	int box_8[26][26];			// 8*8 ���ӵı��, ̹���ƶ�,�����øø��Ӽ��
	int box_4[52][52];			// 4*4 ���ӵı��, ǽ�������øñ�Ǽ��
};

struct BulletStruct
{
	int x, y;					// �ӵ�����, ���ݲ�ͬ����ָ����ͬ��λ�ô��� x,y
	int dir;					// �ӵ�����
	int speed;					// �ӵ��ٶ�
	IMAGE mBulletImage[4];		// ͼƬ
};

/************* ��ҿ��� *************
* һ�����ʵ����һ������
* ���Թ����Լ�������
*/

class PlayerBase
{
public:
	PlayerBase(byte player);						// player [0-1]
	~PlayerBase();
	void DrawPlayerTankIco( HDC );					// �����Ҳ�����\2P\1P\̹��ͼ��\ʣ������ֵ
	void DrawPlayerTank( HDC );						// �������̹��
	bool PlayerControl(BoxMarkStruct*);				// ��ҿ���̹���ƶ�
	void BulletMoving(HDC);							// �ӵ��ƶ�, ��GameControl ��ѭ������

private:
	void ChangeDir(int new_dir);					// ���ķ���, ͬʱ�������굽����������
	bool CheckMoveable( byte dir, BoxMarkStruct*);	// ��⵱ǰ�����Ƿ�����ƶ�
	bool ShootBullet(int bullet_id);				// ���� id ���ӵ�[0,1]

private:
	byte player_id : 1;						// [0-1] ���
	PlayerTank* mPlayerTank;				// ̹����

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

	static IMAGE mBulletImage[4];			// �ĸ������ӵ�, 
	static int mBulletSize[4][2];			// {{4,3},{3,4},{4,3},{3,4}} �ߴ�: ����-3*4 / ����-4*3
	int mBulletX[2], mBulletY[2];			// �ӵ�����, һ���������; 3/4�����̹�˿��Է��������ӵ�
	int mBulletDir[2];						// �ӵ�����
};