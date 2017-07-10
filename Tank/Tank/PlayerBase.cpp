#include "stdafx.h"
#include "PlayerBase.h"

// ---------------- �ӵ��ṹ��̬����
IMAGE BulletStruct::mBulletImage[4];
int BulletStruct::mBulletSize[4][2] = { { 4,3 },{ 3,4 },{ 4,3 },{ 3,4 } };			// ��ͬ�����ӵ��ߴ�(width height)

// �ӵ����̹�����ĵ�ƫ����
int BulletStruct::devto_tank[4][2] = { { -BOX_SIZE - 1, -1 },{ -2, -BOX_SIZE - 1 },{ BOX_SIZE - 3, -1 },{ -2, BOX_SIZE - 3 } };

//----------------- PlayerBase �ྲ̬����

int PlayerBase::mDevXY[4][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };	// ������������

PlayerBase::PlayerBase(byte player)
{
	int i = 0;
	player_id = player;
	mPlayerTank = new PlayerTank(player_id);

	// ��ͬ������ݲ�ͬ
	if (player_id == 0)
	{
		loadimage(&m12PImage, _T("./res/big/1P.gif"));		// 1P\2Pͼ��
		m12PImage_x = 233;									// 1P\2P ����
		m12PImage_y = 129;
		mPlayerTankIcoImage_x = 232;						// ���̹��ͼ������
		mPlayerTankIcoImage_y = 137;
		mPlayerLife_x = 240;								// �������ֵ����
		mPlayerLife_y = 137;
		mTankX = 4 * 16 + BOX_SIZE;							// ̹���״γ���ʱ�����������
		mTankY = 7 * 16 + BOX_SIZE;
	}
	else
	{
		loadimage(&m12PImage, _T("./res/big/2P.gif"));
		m12PImage_x = 233;
		m12PImage_y = 153;
		mPlayerTankIcoImage_x = 232;
		mPlayerTankIcoImage_y = 161;
		mPlayerLife_x = 240;
		mPlayerLife_y = 161;
		mTankX = 8 * 16 + BOX_SIZE;
		mTankY = 12 * 16 + BOX_SIZE;
	}

	// ��ͬ������
	loadimage(&mPlayerTankIcoImage, _T("./res/big/playertank-ico.gif"	));	// ���̹��ͼ��
	loadimage(&mBlackNumberImage,	_T("./res/big/black-number.gif"		));	// ��ɫ����
	mPlayerLife = 2;		// ��� HP
	mPlayerTankLevel = 0;													// ̹�˼��� [0-3]
	mTankDir = DIR_UP;		// ̹�˷���

	// ��ͬ����̹���ƶ��ٶ�ϵ��
	int temp[4] = {4, 2, 3, 3};
	for ( i = 0; i < 4; i++ )
		mSpeed[i] = temp[i];

	/*********************************
	* BulletStruct ���ݳ�ʼ��
	*********************************/
	// �����ӵ�ͼƬ
	TCHAR buf[100];
	for (i = 0; i < 4; i++)
	{
		_stprintf_s(buf, _T("./res/big/bullet-%d.gif"), i);
		loadimage(&BulletStruct::mBulletImage[i], buf);
	}

	// �ӵ��ṹ����
	int temp_speed[4] = {6, 7, 7, 8};			// ����̹�˼�������ӵ��ٶ�ϵ��
	for (i = 0; i < 2; i++)
	{
		mBulletStruct[i].x = SHOOTABLE_X;		// x ���������ж��Ƿ���Է���
		mBulletStruct[i].y = -1000;
		mBulletStruct[i].dir = DIR_UP;

		// ����̹�˼�������ӵ��ٶ�ϵ��
		for (int j = 0; j < 4; j++)
			mBulletStruct[i].speed[j] = temp_speed[j];
	}

	mMoving = false;
}

PlayerBase::~PlayerBase()
{
}

// ������ҵ�һЩ����: 1P\2P ̹��ͼ�� ����
void PlayerBase::DrawPlayerTankIco(const HDC& right_panel_hdc)
{
	// ����1P/2P
	TransparentBlt(right_panel_hdc, m12PImage_x, m12PImage_y, PLAYER_12_ICO_SIZE_X, PLAYER_12_ICO_SIZE_Y,
							GetImageHDC(&m12PImage), 0, 0, PLAYER_12_ICO_SIZE_X, PLAYER_12_ICO_SIZE_Y, 0xffffff);
	// ����̹��ͼ��
	TransparentBlt(right_panel_hdc, mPlayerTankIcoImage_x, mPlayerTankIcoImage_y, PLAYER_TANK_ICO_SIZE_X, PLAYER_TANK_ICO_SIZE_Y,
									GetImageHDC(&mPlayerTankIcoImage), 0, 0, PLAYER_TANK_ICO_SIZE_X, PLAYER_TANK_ICO_SIZE_Y, 0x000000);
	// ���������������
	TransparentBlt(right_panel_hdc, mPlayerLife_x, mPlayerLife_y, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
		GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * mPlayerLife, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff );
}

//
void PlayerBase::DrawPlayerTank(const HDC& canvas_hdc)
{
	IMAGE tank = mPlayerTank->GetTankImage(mPlayerTankLevel, mTankDir, mMoving);
	TransparentBlt(canvas_hdc, mTankX - BOX_SIZE, mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2, GetImageHDC(&tank), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
}

//
bool PlayerBase::PlayerControl(BoxMarkStruct* bms)
{
	if (GetAsyncKeyState(27) & 0x8000)
		return false;

	switch (player_id)
	{
	case 0:										// ���һ
		if (GetAsyncKeyState('A') & 0x8000)
		{
			mMoving = true;
			Move(DIR_LEFT, bms);
		}
		else if (GetAsyncKeyState('W') & 0x8000)
		{
			mMoving = true;
			Move(DIR_UP, bms);
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			mMoving = true;
			Move(DIR_RIGHT, bms);
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			mMoving = true;
			Move(DIR_DOWN, bms);
		}
		else
		{
			mMoving = false;
		}

		// ���ܼ� else if, ��Ȼ�ƶ�ʱ���޷������ӵ�
		if (GetAsyncKeyState('J') & 0x8000)	// �����ӵ�
		{
			if (!ShootBullet(0))
				ShootBullet(1);
		}
		break;

	case 1:										// ��Ҷ�
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			mMoving = true;
			Move(DIR_LEFT, bms);
		}
		else if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			mMoving = true;
			Move(DIR_UP, bms);
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			mMoving = true;
			Move(DIR_RIGHT, bms);
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			mMoving = true;
			Move(DIR_DOWN, bms);
		}else
		{
			mMoving = false;
		}

		// ���ּ� 1 �����ӵ�
		if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		{
			if (!ShootBullet(0))
				ShootBullet(1);
		}
		break;
	default:
		break;
	}
	return true;
}

//
void PlayerBase::BulletMoving(const HDC& center_hdc)
{
	// 1���ӵ����ƶ�
	if (mBulletStruct[0].x != SHOOTABLE_X)
	{
		int dir = mBulletStruct[0].dir;
		mBulletStruct[0].x += mDevXY[dir][0] * mBulletStruct[0].speed[mPlayerTankLevel];
		mBulletStruct[0].y += mDevXY[dir][1] * mBulletStruct[0].speed[mPlayerTankLevel];

		TransparentBlt(center_hdc, mBulletStruct[0].x, mBulletStruct[0].y, BulletStruct::mBulletSize[dir][0],
			BulletStruct::mBulletSize[dir][1], GetImageHDC(&BulletStruct::mBulletImage[dir]),
			0, 0, BulletStruct::mBulletSize[dir][0], BulletStruct::mBulletSize[dir][1], 0x000000);

		if (mBulletStruct[0].x < 0 || mBulletStruct[0].x > CENTER_WIDTH || mBulletStruct[0].y < 0 || mBulletStruct[0].y > CENTER_HEIGHT)
			mBulletStruct[0].x = SHOOTABLE_X;
	}
	/*
	if (mPlayerTankLevel > 1 && mBulletX[1] != SHOOTABLE_X)
	{
		mBulletX[1] += mDevXY[mBulletDir[1]][0] * 6;
		mBulletY[1] += mDevXY[mBulletDir[1]][1] * 6;
	}*/
}

//---------------------------------------------------------------- private function ---------

// �����ͬʱ����̹�����ڸ���. ���뱣֤̹�������ڸ�������
void PlayerBase::Move(int new_dir, BoxMarkStruct* bms)
{
	if (mTankDir != new_dir)
	{
		// ԭ���ұ����·���
		if (mTankDir == DIR_LEFT || mTankDir == DIR_RIGHT)
		{
			if (mTankX > (mTankX / BOX_SIZE) * BOX_SIZE + BOX_SIZE / 2 - 1)	// ����ǿ����������ϵ��ұ߽ڵ�, -1������
				mTankX = (mTankX / BOX_SIZE + 1) * BOX_SIZE;
			else
				mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;					// �����������ϵ���߽ڵ�
		}
		// ���±�����
		else
		{
			if (mTankY > (mTankY / BOX_SIZE) * BOX_SIZE + BOX_SIZE / 2 - 1)	// ����ǿ����������ϵ��±߽ڵ�, -1������
				mTankY = (mTankY / BOX_SIZE + 1) * BOX_SIZE;
			else
				mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;					// �����������ϵ��ϱ߽ڵ�
		}

		// ���ķ���, �����ȵ���λ�ú�������÷���!!
		mTankDir = new_dir;
	}
	else								// �ƶ�
	{
		if (CheckMoveable(mTankDir, bms))
		{
			mTankX += mDevXY[mTankDir][0] * mSpeed[mPlayerTankLevel];
			mTankY += mDevXY[mTankDir][1] * mSpeed[mPlayerTankLevel];
		}
	}
}

/* �жϵ�ǰ����ɷ��ƶ�
	box_8[i][j]
	i = y / BOX_SIZE		// y �����Ӧ���� i ֵ
	j = x / BOX_SIZE		// x �����Ӧ���� j ֵ
	---------
	| 1 | 2	|
	----a----
	| 3 | 4	|
	---------
* ��� (x,y) �� a ���, ת����� i,j ���ڸ��� 4
* ��� x ֵ�� a �����, ��ת����� j ���� 1��3; �ұ������� 2��4
* ��� y ֵ�� a ������, ��ת����� i ���� 1��2; ���������� 3��4
** ��� tempx,tempy ��Խ�˸����������ϰ�, ��ô�ͽ� mTankX �� mTankY ��������������,
*/
bool PlayerBase::CheckMoveable(byte dir, BoxMarkStruct* bms)
{
	// ̹����������
	int tempx = mTankX + mDevXY[mTankDir][0] * mSpeed[mPlayerTankLevel];
	int tempy = mTankY + mDevXY[mTankDir][1] * mSpeed[mPlayerTankLevel];

	if (tempx < BOX_SIZE || tempy < BOX_SIZE || tempy > CENTER_WIDTH - BOX_SIZE || tempx > CENTER_HEIGHT - BOX_SIZE)
	{
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (dir)
		{
		case DIR_LEFT:	mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;	break;	// mTankX �� tempx ֮���Խ�˸���, ��̹�˷ŵ�mTankX���ڵĸ�������
		case DIR_UP:	mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_RIGHT: mTankX = (tempx  / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_DOWN:	mTankY = (tempy  / BOX_SIZE) * BOX_SIZE;	break;
		default:													break;
		}
		return false;
	}
	// ת�����ص����ڵ� xy[26][26] �±�
	int index_i = tempy / BOX_SIZE;
	int index_j = tempx / BOX_SIZE;

	int dev[4][2][2] = { {{-1,-1},{0,-1}},  {{-1,-1},{-1,0}},  {{-1,1},{0,1}}, { {1,-1},{1,0}} };

	if (bms->box_8[index_i + dev[dir][0][0]][index_j + dev[dir][0][1]] > 2 ||
		bms->box_8[index_i + dev[dir][1][0]][index_j + dev[dir][1][1]] > 2 )
	{
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (dir)
		{
		case DIR_LEFT:	mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;	break;	// mTankX �� tempx ֮���Խ�˸���, ��̹�˷ŵ�mTankX���ڵĸ�������
		case DIR_UP:	mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_RIGHT: mTankX = (tempx  / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_DOWN:	mTankY = (tempy  / BOX_SIZE) * BOX_SIZE;	break;
		default:													break;
		}
		return false;
	}
	return true;
}

// �����ӵ�
bool PlayerBase::ShootBullet( int bullet_id )
{
	switch (bullet_id)
	{
		case 0:
			// 1���ӵ�����ʧ��
			if (mBulletStruct[0].x != SHOOTABLE_X)		
				return false;

			// �ӵ����������
			mBulletStruct[0].x = mTankX + BulletStruct::devto_tank[mTankDir][0];
			mBulletStruct[0].y = mTankY + BulletStruct::devto_tank[mTankDir][1];
			mBulletStruct[0].dir = mTankDir;
			return true;

		case 1:
			// 2 ���ӵ�����ʧ��
			if (mPlayerTankLevel < 2 || mBulletStruct[1].x != SHOOTABLE_X)
				return false;

			// �ӵ����������
			mBulletStruct[1].x = mTankX + BulletStruct::devto_tank[mTankDir][0];
			mBulletStruct[1].y = mTankY + BulletStruct::devto_tank[mTankDir][1];
			mBulletStruct[0].dir = mTankDir;
			return true;

		default:
			break;
	}
	return false;
}
