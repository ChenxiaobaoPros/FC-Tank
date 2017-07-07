#include "stdafx.h"
#include "PlayerBase.h"

//BoxMarkStruct* PlayerBase::mBms = NULL;
int PlayerBase::mDevXY[4][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };	// ������������

PlayerBase::PlayerBase(byte player)
{
	player_id = player;
	mPlayerTank = new PlayerTank(player_id);
	//mBms = bms;												// ���ӱ�ǽṹָ��, ָ�� main �ļ������һ���ṹ��

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
	mPlayerTankLevel = 0;
	mTankDir = DIR_LEFT;
	mSpeed = 2;
	//mKeyCounter = 3;
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
		//if ( mKeyCounter++ != 3 )
		//	break;
		if (GetAsyncKeyState('A') & 0x8000)
		{
			if (mTankDir != DIR_LEFT)			// ��ת��
			{
				mTankDir = DIR_LEFT;
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_LEFT, bms))
				{
					mTankX += mDevXY[DIR_LEFT][0] * mSpeed;
					mTankY += mDevXY[DIR_LEFT][1] * mSpeed;
				}
			}
		}
		else if (GetAsyncKeyState('W') & 0x8000)
		{
			if (mTankDir != DIR_UP)			// ��ת��
			{
				mTankDir = DIR_UP;
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_UP, bms))
				{
					mTankX += mDevXY[DIR_UP][0] * mSpeed;
					mTankY += mDevXY[DIR_UP][1] * mSpeed;
				}
			}
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			if (mTankDir != DIR_RIGHT)		// ��ת��
			{
				mTankDir = DIR_RIGHT;
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_RIGHT, bms))
				{
					mTankX += mDevXY[DIR_RIGHT][0] * mSpeed;
					mTankY += mDevXY[DIR_RIGHT][1] * mSpeed;
				}
			}
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			if (mTankDir != DIR_DOWN)			// ��ת��
			{
				mTankDir = DIR_DOWN;
			}
			else								// �ƶ�
			{
				if (CheckMoveable(DIR_DOWN, bms))
				{
					mTankX += mDevXY[DIR_DOWN][0] * mSpeed;
					mTankY += mDevXY[DIR_DOWN][1] * mSpeed;
				}
			}
		}
		break;

	case 1:										// ��Ҷ�
		//if ( mKeyCounter++ != 3 )
		//	break;
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
					mTankX += mDevXY[DIR_LEFT][0] * mSpeed;
					mTankY += mDevXY[DIR_LEFT][1] * mSpeed;
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
					mTankX += mDevXY[DIR_UP][0] * mSpeed;
					mTankY += mDevXY[DIR_UP][1] * mSpeed;
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
					mTankX += mDevXY[DIR_RIGHT][0] * mSpeed;
					mTankY += mDevXY[DIR_RIGHT][1] * mSpeed;
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
					mTankX += mDevXY[DIR_DOWN][0] * mSpeed;
					mTankY += mDevXY[DIR_DOWN][1] * mSpeed;
				}
			}
		}
		break;
	default:
		break;
	}

	return true;
}

//---------------------------------------------------------------- private function ---------

// �����ͬʱ����̹�����ڸ���. ���뱣֤̹�������ڸ�������
void PlayerBase::ChangeDir(int new_dir)
{
	// ԭ���ұ����·���
	if (mTankDir == DIR_LEFT || mTankDir == DIR_RIGHT)
	{
		if (mTankX > (mTankX / BOX_SIZE) * BOX_SIZE + BOX_SIZE / 2)	// ����ǿ����������ϵ��ұ߽ڵ�
			mTankX = (mTankX / BOX_SIZE + 1) * BOX_SIZE;
		else
			mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;				// �����������ϵ���߽ڵ�
	}
	// ���±�����
	else
	{
		if (mTankY > (mTankY / BOX_SIZE) * BOX_SIZE + BOX_SIZE / 2)	// ����ǿ����������ϵ��±߽ڵ�
			mTankY = (mTankY / BOX_SIZE + 1) * BOX_SIZE;
		else
			mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;				// �����������ϵ��ϱ߽ڵ�
	}

	mTankDir = new_dir;
}

// �жϵ�ǰ����ɷ��ƶ�
bool PlayerBase::CheckMoveable(byte dir, BoxMarkStruct* bms)
{
	int tempx = mTankX + mDevXY[mTankDir][0] * mSpeed;
	int tempy = mTankY + mDevXY[mTankDir][1] * mSpeed;

	// ת�����ص����ڵ� xy[26][26] �±�
	int index_i = tempx / BOX_SIZE;
	int index_j = tempy / BOX_SIZE;

	int dev[4][2][2] = { {{0,0},{0,1}},{{0,0},{1,0}},{{2,0},{2,1}},{{0,2},{1,2}} };
	//if ( bms->box_8[index_i][index_j] != '0' )
		//return false;
	if (bms->box_8[index_j + dev[dir][0][0]][index_i + dev[dir][0][1]] != 0 ||
		bms->box_8[index_j + dev[dir][1][0]][index_i + dev[dir][1][1]] != 0)
	{

		/*printf("%d, %d - %d/%d/%d/%d - %d, %d\n", index_i, index_j,
			index_i + dev[dir][0][0],
			index_j + dev[dir][0][1],
			index_i + dev[dir][1][0],
			index_j + dev[dir][1][1],
			bms->box_8[index_i + dev[dir][0][0]][index_j + dev[dir][0][1]], bms->box_8[index_i + dev[dir][1][0]][index_j + dev[dir][1][1]]);
	*/
		//return false;
	}
	return true;
}
