#include "stdafx.h"
#include "PlayerBase.h"

int PlayerBase::mDevXY[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};	// ������������

PlayerBase::PlayerBase( byte player )
{
	player_id = player;
	mPlayerTank = new PlayerTank(player_id);

	// ��ͬ������ݲ�ͬ
	if (player_id == 0)
	{
		m12PImage = Image::FromFile(L"./res/big/1P.gif");	// 1P\2Pͼ��
		m12PImage_x = 233;									// 1P\2P ����
		m12PImage_y = 129;
		mPlayerTankIcoImage_x = 232;						// ���̹��ͼ������
		mPlayerTankIcoImage_y = 137;
		mPlayerLife_x = 240;								// �������ֵ����
		mPlayerLife_y = 137;
		mTankX =  4 * 16 + CENTER_X;						// ̹���״γ���ʱ�������
		mTankY = 7 * 16 + CENTER_Y;
	}
	else
	{
		m12PImage = Image::FromFile(L"./res/big/2P.gif");
		m12PImage_x = 233;
		m12PImage_y = 153;
		mPlayerTankIcoImage_x = 232;
		mPlayerTankIcoImage_y = 161;
		mPlayerLife_x = 240;
		mPlayerLife_y = 161;
		mTankX =  8 * 16 + CENTER_X;
		mTankY = 12 * 16 + CENTER_Y;
	}

	// ��ͬ������
	mPlayerTankIcoImage = Image::FromFile(L"./res/big/playertank-ico.gif");		// ���̹��ͼ��
	mPlayerLife = 2;
	mBlackNumberImage = Image::FromFile(L"./res/big/black-number.gif");
	mPlayerTankLevel = 0;
	mTankDir = DIR_LEFT;
	mSpeed = 4;
	//mKeyCounter = 3;
}

PlayerBase::~PlayerBase()
{
}

// ������ҵ�һЩ����: 1P\2P ̹��ͼ�� ����
void PlayerBase::DrawPlayerTankIco( Graphics* graphics )
{
	// ����1P/2P
	graphics->DrawImage( m12PImage, m12PImage_x, m12PImage_y, PLAYER_12_ICO_SIZE_X, PLAYER_12_ICO_SIZE_Y );
	// ����̹��ͼ��
	graphics->DrawImage( mPlayerTankIcoImage, mPlayerTankIcoImage_x, mPlayerTankIcoImage_y, PLAYER_TANK_ICO_SIZE_X, PLAYER_TANK_ICO_SIZE_Y );
	// ���������������
	graphics->DrawImage( mBlackNumberImage, Rect(mPlayerLife_x, mPlayerLife_y, BLACK_NUMBER_SIZE,
		BLACK_NUMBER_SIZE), BLACK_NUMBER_SIZE * mPlayerLife, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, UnitPixel);
}

//
void PlayerBase::DrawPlayerTank( HDC canvas_hdc )
{
	IMAGE tank = mPlayerTank->GetTankImage( mPlayerTankLevel, mTankDir );
	TransparentBlt( canvas_hdc, mTankX, mTankY, BOX_SIZE * 2, BOX_SIZE * 2, GetImageHDC(&tank), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000 );
}

//
bool PlayerBase::PlayerControl( BoxMarkStruct* bms )
{
	if ( GetAsyncKeyState(27) & 0x8000 )
		return false;

	switch( player_id )
	{
	case 0:										// ���һ
		//if ( mKeyCounter++ != 3 )
		//	break;
		if ( GetAsyncKeyState('A') & 0x8000 )
		{
			if ( mTankDir != DIR_LEFT )			// ��ת��
			{
				mTankDir = DIR_LEFT;
			}
			else								// �ƶ�
			{
				mTankX += mDevXY[DIR_LEFT][0] * mSpeed;
				mTankY += mDevXY[DIR_LEFT][1] * mSpeed;
			}
		}
		else if ( GetAsyncKeyState('W') & 0x8000 )
		{
			if ( mTankDir != DIR_UP )			// ��ת��
			{
				mTankDir = DIR_UP;
			}
			else								// �ƶ�
			{
				mTankX += mDevXY[DIR_UP][0] * mSpeed;
				mTankY += mDevXY[DIR_UP][1] * mSpeed;
			}
		}
		else if ( GetAsyncKeyState('D') & 0x8000 )
		{
			if ( mTankDir != DIR_RIGHT )		// ��ת��
			{
				mTankDir = DIR_RIGHT;
			}
			else								// �ƶ�
			{
				mTankX += mDevXY[DIR_RIGHT][0] * mSpeed;
				mTankY += mDevXY[DIR_RIGHT][1] * mSpeed;
			}
		}
		else if ( GetAsyncKeyState('S') & 0x8000 )
		{
			if ( mTankDir != DIR_DOWN )			// ��ת��
			{
				mTankDir = DIR_DOWN;
			}
			else								// �ƶ�
			{
				mTankX += mDevXY[DIR_DOWN][0] * mSpeed;
				mTankY += mDevXY[DIR_DOWN][1] * mSpeed;
			}
		}
		break;

	case 1:										// ��Ҷ�
		//if ( mKeyCounter++ != 3 )
		//	break;
		if ( GetAsyncKeyState(VK_LEFT) & 0x8000 )
		{
			if ( mTankDir != DIR_LEFT )			// ��ת��
			{
				mTankDir = DIR_LEFT;
			}
			else								// �ƶ�
			{
				mTankX += mDevXY[DIR_LEFT][0] * mSpeed;
				mTankY += mDevXY[DIR_LEFT][1] * mSpeed;
			}
		}
		else if ( GetAsyncKeyState(VK_UP) & 0x8000 )
		{
			if ( mTankDir != DIR_UP )			// ��ת��
			{
				mTankDir = DIR_UP;
			}
			else								// �ƶ�
			{
				mTankX += mDevXY[DIR_UP][0] * mSpeed;
				mTankY += mDevXY[DIR_UP][1] * mSpeed;
			}
		}
		else if ( GetAsyncKeyState(VK_RIGHT) & 0x8000 )
		{
			if ( mTankDir != DIR_RIGHT )		// ��ת��
			{
				mTankDir = DIR_RIGHT;
			}
			else								// �ƶ�
			{
				mTankX += mDevXY[DIR_RIGHT][0] * mSpeed;
				mTankY += mDevXY[DIR_RIGHT][1] * mSpeed;
			}
		}
		else if ( GetAsyncKeyState(VK_DOWN) & 0x8000 )
		{
			if ( mTankDir != DIR_DOWN )			// ��ת��
			{
				mTankDir = DIR_DOWN;
			}
			else								// �ƶ�
			{
				mTankX += mDevXY[DIR_DOWN][0] * mSpeed;
				mTankY += mDevXY[DIR_DOWN][1] * mSpeed;
			}
		}
		break;
	default:
		break;
	}

	return true;
}