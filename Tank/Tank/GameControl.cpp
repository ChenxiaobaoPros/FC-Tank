#include "stdafx.h"
#include "GameControl.h"

int GameControl::mCurrentStage = 2;	// [1-35]
GameControl::GameControl( Graphics* grap, HDC des_hdc, HDC image_hdc, BoxMarkStruct* bms)
{
	mGraphics = grap;
	mDes_hdc = des_hdc;
	mImage_hdc = image_hdc;
	mBoxMarkStruct = bms;
	Init();
}

GameControl::~GameControl()
{
}

void GameControl::Init()
{

	loadimage( &mBlackBackgroundImage, _T("./res/big/bg_black.gif"	));		// ��ɫ����
	loadimage( &mGrayBackgroundImage , _T("./res/big/bg_gray.gif"	));		// ��ɫ����
	loadimage( &mStoneImage			 , _T("./res/big/stone.gif"		));		// 12*12��ʯͷ
	loadimage( &mForestImage		 , _T("./res/big/forest.gif"	));		// ����
	loadimage( &mIceImage			 , _T("./res/big/ice.gif"		));		// ����
	loadimage( &mRiverImage[0]		 , _T("./res/big/river-0.gif"	));		// ����
	loadimage( &mRiverImage[1]		 , _T("./res/big/river-1.gif"	));		//
	loadimage( &mWallImage			 , _T("./res/big/wall.gif"		));		// ��ǽ

	mActiveEnemyTankNumber = 0;												// �Ѿ������ڵ�ͼ�ϵĵл�����,�����ʾ6��
	mRemainEnemyTankNumber = 20;											// ʣ��δ���ֵĵл�����

	mEnemyTankIcoImage = Image::FromFile( L"./res/big/enemytank-ico.gif" );	// �л�ͼ��
	mFlagImage = Image::FromFile( L"./res/big/flag.gif");					// ����
	mBlackNumberImage = Image::FromFile(L"./res/big/black-number.gif");		// 0123456789 ��ɫ����
}

// �����ҽ�����
void GameControl::AddPlayer(int player_num)
{
	for ( int i = 0; i < player_num; i++ )
		PlayerList.push_back( *(new PlayerBase(i)) );	// �����������
}

/*
* ��ȡdata���ݻ��Ƶ�ͼ,
* ��ʾ�л�����\�������\�ؿ�\����Ϣ
*/
void GameControl::LoadMap()
{
	// ��ȡ��ͼ�ļ�����
	FILE* fp = NULL;
	if ( 0 != fopen_s(&fp, "./res/data/map.txt", "rb") )
		throw _T("��ȡ��ͼ�����ļ��쳣.");
	fseek(fp, sizeof(Map) * (mCurrentStage - 1), SEEK_SET );
	fread(&mMap, sizeof(Map), 1, fp);
	fclose(fp);

	int x = 0, y = 0;
	for ( int i = 0; i < 26; i++ )
	{
		for ( int j = 0; j < 26; j++ )
		{
			SignBoxMark( i, j, mMap.buf[i][j] );		// ��� 26*26 �� 52*52 ����
		}
	}

	while (StartGame())
	{
		Sleep(30);
	}
}

bool GameControl::StartGame()
{
	/*
	Graphics* g = Graphics::FromImage(mGrayBackgroundImage);
	HDC hdc = g->GetHDC();
	BitBlt(mImage_hdc, 0, 0, CANVAS_WIDTH + 10, CANVAS_HEIGHT + 10, hdc, 0, 0, SRCCOPY );
	g->ReleaseHDC(hdc);*/
	//mGraphics->DrawImage(mGrayBackgroundImage, 0, 0, CANVAS_WIDTH + 10, CANVAS_HEIGHT + 10);			// ��ɫ����
	//mGraphics->DrawImage(mBlackBackgroundImage, CENTER_X, CENTER_Y, CENTER_WIDTH, CENTER_HEIGHT);		// ���ĺ�ɫ������Ϸ��
	StretchBlt(mImage_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY);
	BitBlt(mImage_hdc, CENTER_X, CENTER_Y, CENTER_WIDTH, CENTER_HEIGHT, GetImageHDC(&mBlackBackgroundImage), 0, 0, SRCCOPY );

	// ���1P\2P\̹��ͼ��\������
	list<PlayerBase>::iterator itor;
	for (itor = PlayerList.begin(); itor != PlayerList.end(); itor++)
	{
		itor->DrawPlayerTankIco(mGraphics);		// ̹��ͼ��
		itor->DrawPlayerTank(mImage_hdc);		// ̹��
		if (itor->PlayerControl(mBoxMarkStruct) == false)
			return false;
	}

	/* ��ʼ���������ļ����Ƶ�ͼ
	* ����Ϊ BOX_SIZE x BOX_SIZE �ĸ���
	* x���꣺ j*BOX_SIZE + CENTER_X
	* y���꣺ i*BOX_SIZE + CENTER_Y
	*/
	int x = 0, y = 0;
	for ( int i = 0; i < 26; i++ )
	{
		for ( int j = 0; j < 26; j++ )
		{
			x = j * BOX_SIZE + CENTER_X;
			y = i * BOX_SIZE + CENTER_Y;
			switch(mBoxMarkStruct->box_12[i][j])
			{
			case _WALL:
				BitBlt(mImage_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mWallImage), 0, 0, SRCCOPY );
				break;
			case _FOREST:
				BitBlt(mImage_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mForestImage), 0, 0, SRCCOPY);
				break;
			case _ICE:
				BitBlt(mImage_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mIceImage), 0, 0, SRCCOPY);
				break;
			case _RIVER:
				BitBlt(mImage_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mRiverImage[0]), 0, 0, SRCCOPY);
				break;
			case _STONE:
				BitBlt(mImage_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mStoneImage), 0, 0, SRCCOPY);
				break;
			default:
				break;
			}
		}
	}
	
	// ��ʾ�л�����ͼ��
	ShowEnemyTankIco();		

	// ����
	mGraphics->DrawImage( mFlagImage, 232, 177, FLAG_ICO_SIZE_X, FLAG_ICO_SIZE_Y );

	// �ؿ�
	if ( mCurrentStage < 10 )
		mGraphics->DrawImage( mBlackNumberImage, Rect( 238, 193, 7, 7), 7 * mCurrentStage, 0, 7, 7, UnitPixel ); // Gdiplus::Unit::UnitPixel
	else	// 10,11,12 .. ˫λ���ؿ�
	{
		mGraphics->DrawImage( mBlackNumberImage, Rect( 233, 193, 7, 7), 7 * (mCurrentStage / 10), 0, 7, 7, UnitPixel );
		mGraphics->DrawImage( mBlackNumberImage, Rect( 241, 193, 7, 7), 7 * (mCurrentStage % 10), 0, 7, 7, UnitPixel );
	}

	// ���Ż���������ʾ image ��������
	StretchBlt( mDes_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mImage_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY );
	FlushBatchDraw();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////// ˽�к���,����ʹ�� //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/* ����ʣ��л�������ʾ���ϱ�ͼ��
								�� ��
								�� ��
								�� ��
								�� ��
								�� ��
								�� ��
								...
*/
void GameControl::ShowEnemyTankIco()
{
	int x[2] = {233, 241};
	int n, index;
	for ( int i = 0; i < mRemainEnemyTankNumber; i++ )
	{
		n = i / 2;
		index = i % 2;

		mGraphics->DrawImage(mEnemyTankIcoImage, x[index], 19 + n * 8, ENEMY_TANK_ICO_SIZE, ENEMY_TANK_ICO_SIZE );
	}
}

// ��� 26*26 �� 52*52 �ĸ���
void GameControl::SignBoxMark(int i, int j, char sign_val)
{
	mBoxMarkStruct->box_12[i][j] = sign_val;	// 26*26
	int temp_i[4] = { 2 * i, 2 * i + 1, 2 * i, 2 * i + 1 };
	int temp_j[4] = { 2 * j, 2 * j, 2 * j + 1, 2 * j + 1 };

	for ( int i = 0; i < 4; i++ )
		mBoxMarkStruct->box_6[ temp_i[i] ][ temp_j[i] ] = sign_val;
}