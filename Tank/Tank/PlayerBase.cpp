#include "stdafx.h"
#include "PlayerBase.h"

// ---------------- �ӵ��ṹ��̬����
IMAGE BulletStruct::mBulletImage[4];
int BulletStruct::mBulletSize[4][2] = { { 4,3 },{ 3,4 },{ 4,3 },{ 3,4 } };			// ��ͬ�����ӵ��ߴ�(width height)

// �ӵ����̹�����ĵ�ƫ����
int BulletStruct::devto_tank[4][2] = { { -BOX_SIZE, -1 },{ -2, -BOX_SIZE },{ BOX_SIZE - 4, -1 },{ -2, BOX_SIZE - 4 } };

// �ӵ�ͼƬ���Ͻ�����ת������ͷ����,���ҷ�����͹���������ǵ�, ���·�������͹�����ұ��ǵ�
int BulletStruct::devto_head[4][2] = { {0, 1}, {2, 0}, {4, 1}, {2, 4} };

//int BulletStruct::bomb_box[4][2] = { {-1, -1},{-1, 0},{0, -1},{0, 0} };// ��ͷ����,�ĸ�����ƫ����

//------------------------

IMAGE BombStruct::mBombImage[3];

//----------------- PlayerBase �ྲ̬����

int PlayerBase::mDevXY[4][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };	// ������������

PlayerBase::PlayerBase(byte player, BoxMarkStruct* b)
{
	int i = 0;
	player_id = player;
	mPlayerTank = new PlayerTank(player_id);
	bms = b;

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
	mPlayerTankLevel = 2;													// ̹�˼��� [0-3]
	mTankDir = DIR_UP;		// ̹�˷���

	// ��ͬ����̹���ƶ��ٶ�ϵ��
	int temp[4] = {2, 3, 3, 3};
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
	int temp_speed[4] = {3, 3, 4, 4};			// ����̹�˼�������ӵ��ٶ�ϵ��
	for (i = 0; i < 2; i++)
	{
		mBulletStruct[i].x = SHOOTABLE_X;		// x ���������ж��Ƿ���Է���
		mBulletStruct[i].y = -1000;
		mBulletStruct[i].dir = DIR_UP;

		// ����̹�˼�������ӵ��ٶ�ϵ��
		for (int j = 0; j < 4; j++)
			mBulletStruct[i].speed[j] = temp_speed[j];
	}

	mBullet_1_counter = 9;				// �ӵ� 1 �˶� N ��ѭ����ſ��Է����ӵ� 2 
	mMoving = false;

	// ��ըͼƬ
	TCHAR c[100];
	for (int i = 0; i < 3; i++)
	{
		_stprintf_s(c, _T("./res/big/bumb%d.gif"), i);
		loadimage(&BombStruct::mBombImage[i], c);
	}
	for (int i = 0; i < 2; i++)
	{
		mBombS[i].mBombX = -100;
		mBombS[i].mBombY = -100;
		mBombS[i].canBomb = false;
		mBombS[i].counter = 0;
	}
}

PlayerBase::~PlayerBase()
{
}

void PlayerBase::PlayerLoop(const HDC& center_hdc)
{
	DrawPlayerTank(center_hdc);		// ̹��
	PlayerControl();
	BulletMoving(center_hdc);
	//Bombing(center_hdc);
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
bool PlayerBase::PlayerControl()
{
	if (GetAsyncKeyState(27) & 0x8000)
		return false;

	switch (player_id)
	{
	case 0:										// ���һ
		if (GetAsyncKeyState('A') & 0x8000)
		{
			mMoving = true;
			Move(DIR_LEFT);
		}
		else if (GetAsyncKeyState('W') & 0x8000)
		{
			mMoving = true;
			Move(DIR_UP);
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			mMoving = true;
			Move(DIR_RIGHT);
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			mMoving = true;
			Move(DIR_DOWN);
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
			Move(DIR_LEFT);
		}
		else if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			mMoving = true;
			Move(DIR_UP);
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			mMoving = true;
			Move(DIR_RIGHT);
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			mMoving = true;
			Move(DIR_DOWN);
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
	for (int i = 0; i < 2; i++)
	{
		// �ӵ����ƶ�
		if (mBulletStruct[i].x != SHOOTABLE_X)
		{
			// �������ϰ������
			if (CheckBomb(i))
				continue;

			int dir = mBulletStruct[i].dir;
			mBulletStruct[i].x += mDevXY[dir][0] * mBulletStruct[i].speed[mPlayerTankLevel];
			mBulletStruct[i].y += mDevXY[dir][1] * mBulletStruct[i].speed[mPlayerTankLevel];

			TransparentBlt(center_hdc, mBulletStruct[i].x, mBulletStruct[i].y, BulletStruct::mBulletSize[dir][0],
				BulletStruct::mBulletSize[dir][1], GetImageHDC(&BulletStruct::mBulletImage[dir]),
				0, 0, BulletStruct::mBulletSize[dir][0], BulletStruct::mBulletSize[dir][1], 0x000000);

			// ��¼�ӵ� 1 �Ĳ���, �����ɷ����ӵ� 2
			if ( i == 0 )
				mBullet_1_counter--;
		}
	}
	/*
	if (mPlayerTankLevel > 1 && mBulletX[1] != SHOOTABLE_X)
	{
		mBulletX[1] += mDevXY[mBulletDir[1]][0] * 6;
		mBulletY[1] += mDevXY[mBulletDir[1]][1] * 6;
	}*/
}

void PlayerBase::Bombing(const HDC& center_hdc)
{
	int index[3] = {0,1,2};
	for (int i = 0; i < 2; i++)
	{
		if (mBombS[i].canBomb)
		{
			TransparentBlt(center_hdc, mBombS[i].mBombX - BOX_SIZE, mBombS[i].mBombY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
				GetImageHDC(&BombStruct::mBombImage[index[mBombS[i].counter % 3]]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
			if (mBombS[i].counter++ == 3)
				mBombS[i].canBomb = false;
		}
	}
}

//---------------------------------------------------------------- private function ---------

// �����ͬʱ����̹�����ڸ���. ���뱣֤̹�������ڸ�������
void PlayerBase::Move(int new_dir)
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
		if (CheckMoveable(mTankDir))
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
bool PlayerBase::CheckMoveable(byte dir)
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
			// 1���ӵ�����ʧ��, ��ըδ���ǰ���ܷ���
			if (mBulletStruct[0].x != SHOOTABLE_X || mBombS[0].canBomb == true)		
				return false;

			// �ӵ����������
			mBulletStruct[0].x = mTankX + BulletStruct::devto_tank[mTankDir][0];
			mBulletStruct[0].y = mTankY + BulletStruct::devto_tank[mTankDir][1];
			mBulletStruct[0].dir = mTankDir;
			mBullet_1_counter = 6;
			return true;

		case 1:
			// 2 ���ӵ�����ʧ��
			if (mPlayerTankLevel < 2 || mBulletStruct[1].x != SHOOTABLE_X || mBullet_1_counter > 0 || mBombS[1].canBomb == true)
				return false;

			// �ӵ����������
			mBulletStruct[1].x = mTankX + BulletStruct::devto_tank[mTankDir][0];
			mBulletStruct[1].y = mTankY + BulletStruct::devto_tank[mTankDir][1];
			mBulletStruct[1].dir = mTankDir;
			return true;

		default:
			break;
	}
	return false;
}

//
bool PlayerBase::CheckBomb(int i)
{
	//if (mBombS[i].canBomb)
	//	return true;
	int dir = mBulletStruct[i].dir;

	// �ӵ�ͷ�Ӵ����ϰ�����Ǹ���, ���ҷ��������, ���·��������
	int bombx = mBulletStruct[i].x + BulletStruct::devto_head[dir][0];
	int bomby = mBulletStruct[i].y + BulletStruct::devto_head[dir][1];

	bool flag = false;
	int adjust_x = 0, adjust_y = 0;		// ��������ͼƬ��ʾ������
	if (mBulletStruct[i].x <= 0)
	{
		flag = true;
		adjust_x = 5;					// ����ըͼƬ������һ��
	}
	else if (mBulletStruct[i].y <= 0)
	{
		flag = true;
		adjust_y = 5;
	}
	else if (mBulletStruct[i].x >= CENTER_WIDTH - 4 && mBulletStruct[i].dir == DIR_RIGHT)
	{
		flag = true;
		adjust_x = -4;
	}
	else if (mBulletStruct[i].y >= CENTER_HEIGHT - 4 && mBulletStruct[i].dir == DIR_DOWN)
	{
		flag = true;
		adjust_y = -4;
	}
	if (flag)
	{
		//printf("%d - %d; %d - %d\n", bombx, bomby, mBulletStruct[i].x, mBulletStruct[i].y);
		mBulletStruct[i].x = SHOOTABLE_X;
		mBombS[i].canBomb = true;
		mBombS[i].mBombX = bombx + adjust_x;
		mBombS[i].mBombY = bomby + adjust_y;
		mBombS[i].counter = 0;

		return true;
	}

	int tempi, tempj;
	int bi = bomby / SMALL_BOX_SIZE;
	int bj = bombx / SMALL_BOX_SIZE;

	// ��ը����������ӵ�ͷ��ƫ����
	int bomb_center_dev[4][2] = { {1, 0}, {0, 1}, {0, 0}, {0, 0} };
	switch (mBulletStruct[i].dir)
	{
	// ���Ҽ���ӵ�ͷ���ڵ�4*4���Ӻ����������ڵ��Ǹ�
	case DIR_LEFT:
	case DIR_RIGHT:
	{
		int temp[2][2] = { {0, 0}, {-1, 0} };
		for (int n = 0; n < 2; n++)
		{
			tempi = bi + temp[n][0];
			tempj = bj + temp[n][1];
			if (bms->box_4[tempi][tempj] != 0)
			{
			printf("%d- %d;;%d - %d\n", bombx, bomby, tempi, tempj);
				// bomb
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
				mBombS[i].mBombX = (bombx / SMALL_BOX_SIZE + bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
				mBombS[i].mBombY = (bomby / SMALL_BOX_SIZE + bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
				mBombS[i].counter = 0;
				return true;
			}
		}
	}
	break;

	// ����ֻ����������ڵ���������
	case DIR_UP:
	case DIR_DOWN:
	{
		int temp[2][2] = { { 0, 0 },{ 0, -1 } };
		for (int n = 0; n < 2; n++)
		{
			tempi = bi + temp[n][0];
			tempj = bj + temp[n][1];
			if (bms->box_4[tempi][tempj] != 0)
			{
				// bomb
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
				mBombS[i].mBombX =( bombx / SMALL_BOX_SIZE + bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
				mBombS[i].mBombY =( bomby / SMALL_BOX_SIZE + bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
				mBombS[i].counter = 0;
				return true;
			}
		}
	}
		break;
	default:
		break;
	}
	/*
	for (int n = 0; n < 4; n++)
	{
		tempi = bi + BulletStruct::bomb_box[n][0];
		tempj = bj + BulletStruct::bomb_box[n][1];

		if (bms->box_4[tempi][tempj] != 0)
		{
		printf("%d- %d;;%d - %d\n", bombx, bomby, tempi, tempj);
			// bomb
			mBulletStruct[i].x = SHOOTABLE_X;
			mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
			mBombS[i].mBombX = bombx;
			mBombS[i].mBombY = bomby;
			mBombS[i].counter = 0;
			return true;
		}
	}*/
	return false;
}

