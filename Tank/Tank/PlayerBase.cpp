#include "stdafx.h"
#include "PlayerBase.h"

int PlayerBase::mDevXY[4][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };	// ������������
IMAGE PlayerBase::mBulletImage[4];
int PlayerBase::mBulletSize[4][2] = { { 4,3 },{ 3,4 },{ 4,3 },{ 3,4 } };

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
	mPlayerLife = 2;
	mPlayerTankLevel = 0;													// ̹�˼��� [0-3]
	mTankDir = DIR_UP;

	// ��ͬ����̹���ƶ��ٶ�ϵ��
	int temp[4] = {2, 3, 3, 3};
	for ( i = 0; i < 4; i++ )
		mSpeed[i] = temp[i];

	// �����ӵ�ͼƬ,			�ظ�һ��, ��Ϊ��̬ IMAGE ���ó�ʼ��?!
	TCHAR buf[100];
	for (i = 0; i < 4; i++)
	{
		_stprintf_s(buf, _T("./res/big/bullet-%d.gif"), i);
		loadimage(&mBulletImage[i], buf);
	}

	// �ӵ�����
	for (i = 0; i < 2; i++)
	{
		mBulletX[i] = mBulletY[i] = SHOOTABLE_X;
		mBulletDir[i] = DIR_UP;
	}
}

PlayerBase::~PlayerBase()
{
}

// ������ҵ�һЩ����: 1P\2P ̹��ͼ�� ����
void PlayerBase::DrawPlayerTankIco(HDC right_panel_hdc)
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
void PlayerBase::DrawPlayerTank(HDC canvas_hdc)
{
	IMAGE tank = mPlayerTank->GetTankImage(mPlayerTankLevel, mTankDir);
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
			if (mTankDir != DIR_LEFT)			// ��ת��
			{
				ChangeDir(DIR_LEFT);
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_LEFT, bms))
				{
					mTankX += mDevXY[DIR_LEFT][0] * mSpeed[mPlayerTankLevel];
					mTankY += mDevXY[DIR_LEFT][1] * mSpeed[mPlayerTankLevel];
				}
			}
		}
		else if (GetAsyncKeyState('W') & 0x8000)
		{
			if (mTankDir != DIR_UP)			// ��ת��
			{
				ChangeDir(DIR_UP);
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_UP, bms))
				{
					mTankX += mDevXY[DIR_UP][0] * mSpeed[mPlayerTankLevel];
					mTankY += mDevXY[DIR_UP][1] * mSpeed[mPlayerTankLevel];
				}
			}
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			if (mTankDir != DIR_RIGHT)		// ��ת��
			{
				ChangeDir(DIR_RIGHT);
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_RIGHT, bms))
				{
					mTankX += mDevXY[DIR_RIGHT][0] * mSpeed[mPlayerTankLevel];
					mTankY += mDevXY[DIR_RIGHT][1] * mSpeed[mPlayerTankLevel];
				}
			}
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			if (mTankDir != DIR_DOWN)			// ��ת��
			{
				ChangeDir(DIR_DOWN);
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_DOWN, bms))
				{
					mTankX += mDevXY[DIR_DOWN][0] * mSpeed[mPlayerTankLevel];
					mTankY += mDevXY[DIR_DOWN][1] * mSpeed[mPlayerTankLevel];
				}
			}
		}
		else if (GetAsyncKeyState('J') & 0x8000)	// �����ӵ�
		{
		}
		break;

	case 1:										// ��Ҷ�
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			if (mTankDir != DIR_LEFT)			// ��ת��
			{
				ChangeDir(DIR_LEFT);
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_LEFT, bms))
				{
					mTankX += mDevXY[DIR_LEFT][0] * mSpeed[mPlayerTankLevel];
					mTankY += mDevXY[DIR_LEFT][1] * mSpeed[mPlayerTankLevel];
				}
			}
		}
		else if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			if (mTankDir != DIR_UP)			// ��ת��
			{
				ChangeDir(DIR_UP);
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_UP, bms))
				{
					mTankX += mDevXY[DIR_UP][0] * mSpeed[mPlayerTankLevel];
					mTankY += mDevXY[DIR_UP][1] * mSpeed[mPlayerTankLevel];
				}
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			if (mTankDir != DIR_RIGHT)		// ��ת��
			{
				ChangeDir(DIR_RIGHT);
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_RIGHT, bms))
				{
					mTankX += mDevXY[DIR_RIGHT][0] * mSpeed[mPlayerTankLevel];
					mTankY += mDevXY[DIR_RIGHT][1] * mSpeed[mPlayerTankLevel];
				}
			}
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			if (mTankDir != DIR_DOWN)			// ��ת��
			{
				ChangeDir(DIR_DOWN);
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_DOWN, bms))
				{
					mTankX += mDevXY[DIR_DOWN][0] * mSpeed[mPlayerTankLevel];
					mTankY += mDevXY[DIR_DOWN][1] * mSpeed[mPlayerTankLevel];
				}
			}
		}
		break;
	default:
		break;
	}

	return true;
}

//
void PlayerBase::BulletMoving(HDC center_hdc)
{
	// 1���ӵ����ƶ�
	if (mBulletX[0] != SHOOTABLE_X)
	{
		mBulletX[0] += mDevXY[mBulletDir[0]][0] * 6;
		mBulletY[0] += mDevXY[mBulletDir[0]][1] * 6;

		//TransparentBlt(center_hdc, mBulletX[0], mBulletY[0], mBulletSize[mBulletDir[0]][0] );
	}

	if (mPlayerTankLevel > 1 && mBulletX[1] != SHOOTABLE_X)
	{
		mBulletX[1] += mDevXY[mBulletDir[1]][0] * 6;
		mBulletY[1] += mDevXY[mBulletDir[1]][1] * 6;
	}
}

//---------------------------------------------------------------- private function ---------

// �����ͬʱ����̹�����ڸ���. ���뱣֤̹�������ڸ�������
void PlayerBase::ChangeDir(int new_dir)
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

	// ���ķ���
	mTankDir = new_dir;
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
	int dev[4][2] = { {-BOX_SIZE - 1, 0}, {0, -BOX_SIZE - 1}, {BOX_SIZE + 1, 0}, {0, BOX_SIZE + 1} };
	switch (bullet_id)
	{
		case 0:
			if (mBulletX[0] != SHOOTABLE_X)		// 1���ӵ�����ʧ��
				return false;

			// �ӵ����������
			mBulletX[0] = mTankX + dev[mTankDir][0];
			mBulletY[0] = mTankY + dev[mTankDir][1];
			return true;

		case 1:
			// ����С��2���ܷ��� 2���ӵ�
			if (mPlayerTankLevel < 2 || mBulletX[1] != SHOOTABLE_X)
				return false;

			// �ӵ����������
			mBulletX[1] = mTankX + dev[mTankDir][0];
			mBulletY[1] = mTankY + dev[mTankDir][1];
			return true;

		default:
			break;
	}

	return false;
}
