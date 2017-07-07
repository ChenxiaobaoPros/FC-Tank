#include "stdafx.h"
#include "GameControl.h"

int GameControl::mCurrentStage = 1;	// [1-35]
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
	// ��ɫ����
	mGrayBackgroundImage = Image::FromFile(L"./res/big/bg_gray.png");
	// ��ɫ����
	mBlackBackgroundImage = Image::FromFile(L"./res/big/bg_black.png");

	mStoneImage		= Image::FromFile(L"./res/big/stone.gif");				// 12*12��ʯͷ
	mForestImage	= Image::FromFile(L"./res/big/forest.gif");				// ����
	mIceImage		= Image::FromFile(L"./res/big/ice.gif");				// ����
	mRiverImage[0]	= Image::FromFile(L"./res/big/river-0.gif");			// ����
	mRiverImage[1]	= Image::FromFile(L"./res/big/river-1.gif");			//
	mWallImage		= Image::FromFile(L"./res/big/wall.gif");				// ��ǽ

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
	//StartGame();
}

bool GameControl::StartGame()
{
	/*
	Graphics* g = Graphics::FromImage(mGrayBackgroundImage);
	HDC hdc = g->GetHDC();
	BitBlt(mImage_hdc, 0, 0, CANVAS_WIDTH + 10, CANVAS_HEIGHT + 10, hdc, 0, 0, SRCCOPY );
	g->ReleaseHDC(hdc);*/
	mGraphics->DrawImage(mGrayBackgroundImage, 0, 0, CANVAS_WIDTH + 10, CANVAS_HEIGHT + 10);			// ��ɫ����
	mGraphics->DrawImage(mBlackBackgroundImage, CENTER_X, CENTER_Y, CENTER_WIDTH, CENTER_HEIGHT);		// ���ĺ�ɫ������Ϸ��
	//BitBlt(mImage_hdc, 0, 0, CANVAS_WIDTH + 10, CANVAS_HEIGHT + 10, Graphics::FromImage(mBlackBackgroundImage)->GetHDC(), 0, 0, SRCCOPY );


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
				mGraphics->DrawImage(mWallImage, x, y, BOX_SIZE, BOX_SIZE);
				break;
			case _FOREST:
				mGraphics->DrawImage(mForestImage, x, y, BOX_SIZE, BOX_SIZE);
				break;
			case _ICE:
				mGraphics->DrawImage(mIceImage, x, y, BOX_SIZE, BOX_SIZE);
				break;
			case _RIVER:
				mGraphics->DrawImage(mRiverImage[0], x, y, BOX_SIZE, BOX_SIZE);
				break;
			case _STONE:
				mGraphics->DrawImage( mStoneImage, x, y, BOX_SIZE, BOX_SIZE );
				break;
			default:
				break;
			}
		}
	}
	
	// ��ʾ�л�����ͼ��
	ShowEnemyTankIco();		

	// ���1P\2P\̹��ͼ��\������
	list<PlayerBase>::iterator itor;
	for ( itor = PlayerList.begin(); itor != PlayerList.end(); itor++ )
	{
		itor->DrawPlayerTankIco( mGraphics );		// ̹��ͼ��
		itor->DrawPlayerTank( mGraphics );			// ̹��
		if( itor->PlayerControl( mBoxMarkStruct ) == false )
			return false;
	}
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

	// ������ʾ image ��������
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
	int x[2] = {233, 240};
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