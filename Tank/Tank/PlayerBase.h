#pragma once
#include "TankClass.h"

/* ��Ӧ��ֵ
* 	#define _WALL		'1'
	#define _FOREST		'2'
	#define _ICE		'3'
	#define _RIVER		'4'
	#define _STONE		'5'
*/
struct BoxMarkStruct
{
	int box_8[26][26];			// 8*8 ���ӵı��, ̹���ƶ�,�����øø��Ӽ��
	int box_4[52][52];			// 4*4 ���ӵı��, ǽ�������øñ�Ǽ��
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

private:
	void ChangeDir(int new_dir);					// ���ķ���
	bool CheckMoveable( byte dir, BoxMarkStruct*);	// ��⵱ǰ�����Ƿ�����ƶ�

private:
	byte player_id : 1;						// [0-1] ���
	PlayerTank* mPlayerTank;				// ̹����
	//static BoxMarkStruct* mBms;			// ָ�� main �ļ��ڵı�ǽṹ

	IMAGE m12PImage;						// 1P\2P ͼ��
	int m12PImage_x, m12PImage_y;			// ͼ������

	IMAGE mPlayerTankIcoImage;				// ���̹��ͼ��
	int mPlayerTankIcoImage_x,				// ͼ������
		mPlayerTankIcoImage_y;

	int mPlayerLife;						// �������, Ĭ��3
	int mPlayerLife_x, mPlayerLife_y;		// ͼ������
	IMAGE mBlackNumberImage;				// 0123456789 ��ɫ����

	int mTankX, mTankY;						// ��ҿ��Ƶ�̹������, ̹�����ĵ�����
	byte mPlayerTankLevel : 2;				// [0-3] ̹�˼���,��õ��ߺ�����̹��
	byte mTankDir : 2;						// ��ǰ̹�˷���0-��,1-��,2��,3-��
	static int mDevXY[4][2];				// �ĸ������ƫ����

	///////////////// ���޸�,�޸ĳɸ���̹�˼���,�ƶ��ٶȲ�ͬ
	int mSpeed;								// mSpeed * mDevXY �õ��˶��ٶ�
	//byte mKeyCounter : 2;					// ��������,�೤�����Ӧ���ΰ���
};