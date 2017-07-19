#include "stdafx.h"
#include "PlayerBase.h"

//----------------- PlayerBase �ྲ̬����

int PlayerBase::mDevXY[4][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };	// ������������
PropClass* PlayerBase::mProp = NULL;
bool PlayerBase::mTimeProp = false;
list<PlayerBase*>* PlayerBase::mPList = NULL;

PlayerBase::PlayerBase(byte player, BoxMarkStruct* b/*, PropClass* pc*/)
{
	int i = 0;
	player_id = player;
	mPlayerTank = new PlayerTank(player_id);
	bms = b;
	mProp = new PropClass(b);

	mPlayerLife = 2;		// ��� HP
	mPlayerTankLevel = 3;

	// ��ͬ����̹���ƶ��ٶ�ϵ��
	int temp[4] = { 1, 1, 1, 1 };
	for (i = 0; i < 4; i++)
		mSpeed[i] = temp[i];

	Init();

	// ��ͬ������ݲ�ͬ
	if (player_id == 0)
		loadimage(&m12PImage, _T("./res/big/1P.gif"));		// 1P\2Pͼ��
	else
		loadimage(&m12PImage, _T("./res/big/2P.gif"));

	// ��ͬ������
	loadimage(&mPlayerTankIcoImage, _T("./res/big/playertank-ico.gif"	));	// ���̹��ͼ��
	loadimage(&mBlackNumberImage,	_T("./res/big/black-number.gif"		));	// ��ɫ����
		
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

	// ��ըͼƬ
	for (i = 0; i < 3; i++)
	{
		_stprintf_s(buf, _T("./res/big/bumb%d.gif"), i);
		loadimage(&BombStruct::mBombImage[i], buf);
	}

	// ̹�˱�ըͼƬ�ṹ
	for (i = 0; i < 5; i++)
	{
		_stprintf_s(buf, _T("./res/big/blast/%d.gif"), i);
		loadimage(&BlastStruct::image[i], buf);
	}

	// �����Ľ�����˸
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(buf, _T("./res/big/star%d.gif"), i);
		loadimage(&StarClass::mStarImage[i], buf);
	}

	// ����������
	for (int i = 0; i < 2; i++)
	{
		_stprintf_s(buf, _T("./res/big/ring%d.gif"), i);
		loadimage(&RingClass::image[i], buf);
	}

	mBombTimer.SetDrtTime(20);	// ��������̫С..
	mBlastTimer.SetDrtTime(36);

	// ��ʾ�������
	mScorePanel = new ScorePanel(player_id);
}

PlayerBase::~PlayerBase()
{
	delete mPlayerTank;
	delete mScorePanel;

	// ��̬���ݷ��ö�� delete
	if (mProp != NULL)
	{
		delete mProp;
		mProp = NULL;
	}

	printf("PLayerBase:: ~PlayerBase()\n");
}

void PlayerBase::Init()
{
	mProp->Init();
	mTimeProp = false;
	mBombProp = false;

	// ��ͬ������ݲ�ͬ
	if (player_id == 0)
	{
		m12PImage_x = 233;									// 1P\2P ����
		m12PImage_y = 129;
		mPlayerTankIcoImage_x = 232;						// ���̹��ͼ������
		mPlayerTankIcoImage_y = 137;
		mPlayerLife_x = 240;								// �������ֵ����
		mPlayerLife_y = 137;
		mTankX = 4 * 16 + BOX_SIZE;							// ̹���״γ���ʱ�����������
		mTankY = 12 * 16 + BOX_SIZE;

		mTankTimer.SetDrtTime(15);		// ̹���ƶ��ٶ�, ��ͬ����ͬ��� ��һ��
		mBulletTimer.SetDrtTime(43);
	}
	else
	{
		m12PImage_x = 233;
		m12PImage_y = 153;
		mPlayerTankIcoImage_x = 232;
		mPlayerTankIcoImage_y = 161;
		mPlayerLife_x = 240;
		mPlayerLife_y = 161;
		mTankX = 8 * 16 + BOX_SIZE;
		mTankY = 12 * 16 + BOX_SIZE;

		mTankTimer.SetDrtTime(15);
		mBulletTimer.SetDrtTime(43);
	}

	int i = 0;
	mDied = false;		
	mTankDir = DIR_UP;		// ̹�˷���

	// �ӵ��ṹ����
	int temp_speed[4] = { 2,3,3,4 };			// ���ܳ��� 4 !! ����Ծ�����ж�.����̹�˼�������ӵ��ٶ�ϵ��
	for (i = 0; i < 2; i++)
	{
		mBulletStruct[i].x = SHOOTABLE_X;		// x ���������ж��Ƿ���Է���
		mBulletStruct[i].y = -1000;
		mBulletStruct[i].dir = DIR_UP;
		mBulletStruct[i].mKillId = 0;			// ��¼���еĵл� id

												// ����̹�˼�������ӵ��ٶ�ϵ��
		for (int j = 0; j < 4; j++)
			mBulletStruct[i].speed[j] = temp_speed[j];
	}

	mBullet_1_counter = 9;				// �ӵ� 1 �˶� N ��ѭ����ſ��Է����ӵ� 2 
	mMoving = false;

	// ��ըͼƬ
	for (i = 0; i < 2; i++)
	{
		mBombS[i].mBombX = -100;
		mBombS[i].mBombY = -100;
		mBombS[i].canBomb = false;
		mBombS[i].counter = 0;
	}

	// SendKillNumToScorePanel() ��ʹ��
	mHasSendKillNumToScorePanel = false;

	// �Ƿ���д�Ӫ
	mIsShootCamp = false;

	// ɱ����
	for (i = 0; i< 4; i++)
		mKillEnemyNumber[i] = 0;

	mPause = false;
	mPauseCounter = 0;
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

bool PlayerBase::ShowStar(const HDC& center_hdc)
{
	// ̹���Ѿ�����,������˸,ֱ�ӷ���
	if (mStar.mIsOuted == true)
		return STOP_SHOW_STAR;

	// ��ʼ��˸�Ľ���
	if (mStar.mStarCounter++ % 2 == 0)
	{
		if (mStar.mStarIndex + mStar.mStarIndexDev < 0)
		{
			mStar.mStarIndex = 1;
			mStar.mStarIndexDev = 1;
		}
		else if (mStar.mStarIndex + mStar.mStarIndexDev > 3)
		{
			mStar.mStarIndex = 2;
			mStar.mStarIndexDev = -1;
		}
		else
		{
			mStar.mStarIndex += mStar.mStarIndexDev;
		}
		if (mStar.mStarCounter == 25)
		{
			mStar.mIsOuted = true;						// ������˸, TankMoving() ������ʼѭ��, ̹�˿�ʼ�ƶ�
			SignBox_8(mTankX, mTankY, _EMPTY);		// ��ֹ��һ��Ƶ�ͼ��̹�˳��ֵ�λ���ڵ�ס
			SignBox_4(mTankX, mTankY, PLAYER_SIGN + player_id);
			return STOP_SHOW_STAR;
		}
	}

	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&StarClass::mStarImage[mStar.mStarIndex]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);

	return SHOWING_STAR;
}

//
void PlayerBase::DrawPlayerTank(const HDC& canvas_hdc)
{
	if (!mStar.mIsOuted || mDied)
		return;

	// 0-5����ʾ̹��. 6-11 ��ʾ.. ��������
	if (mPause && mPauseCounter++ / 10 % 2 != 0)
	{
		//printf("adas3432\n");
		if (mPauseCounter > 266)
			mPause = false;
		return;
	}

	IMAGE tank = mPlayerTank->GetTankImage(mPlayerTankLevel, mTankDir, mMoving);
	TransparentBlt(canvas_hdc, (int)(mTankX - BOX_SIZE), (int)(mTankY - BOX_SIZE), BOX_SIZE * 2, BOX_SIZE * 2, GetImageHDC(&tank), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);

	// // ��ʾ������
	if (mRing.canshow)
	{
		if (mRing.ShowRing(canvas_hdc, mTankX, mTankY) == false)
			// ��ֹ��ұ�ը��������ʱ��,����λ�ñ��л�ռ��,Ȼ����ұ�Ǳ�����
			SignBox_4(mTankX, mTankY, PLAYER_SIGN + player_id);
	}
}

void PlayerBase::DrawBullet(const HDC & center_hdc)
{
	if (mDied)
		return;

	for (int i = 0; i < 2; i++)
	{
		// �ӵ����ƶ�
		if (mBulletStruct[i].x != SHOOTABLE_X)
		{
			int dir = mBulletStruct[i].dir;

			TransparentBlt(center_hdc, mBulletStruct[i].x, mBulletStruct[i].y, BulletStruct::mBulletSize[dir][0],
				BulletStruct::mBulletSize[dir][1], GetImageHDC(&BulletStruct::mBulletImage[dir]),
				0, 0, BulletStruct::mBulletSize[dir][0], BulletStruct::mBulletSize[dir][1], 0x000000);
		}
	}
}

//
bool PlayerBase::PlayerControl()
{
	if (mDied || !mStar.mIsOuted)
		return true;

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
BulletShootKind PlayerBase::BulletMoving(const HDC& center_hdc)
{
	if (mDied || mBulletTimer.IsTimeOut() == false)
		return BulletShootKind::None;


	for (int i = 0; i < 2; i++)
	{
		// �ӵ����ƶ�
		if (mBulletStruct[i].x != SHOOTABLE_X)
		{
			int dir = mBulletStruct[i].dir;


			// �������ϰ������
			BulletShootKind kind = CheckBomb(i);
			if (kind == BulletShootKind::Camp)
				return BulletShootKind::Camp;
			else if (kind == BulletShootKind::Player_1 || kind == BulletShootKind::Player_2)
				return kind;
			else if (kind == BulletShootKind::Other )
				continue;

			// �ȼ����ȡ�����
			SignBullet(mBulletStruct[i].x, mBulletStruct[i].y, dir, _EMPTY);

			mBulletStruct[i].x += mDevXY[dir][0] * mBulletStruct[i].speed[mPlayerTankLevel];
			mBulletStruct[i].y += mDevXY[dir][1] * mBulletStruct[i].speed[mPlayerTankLevel];

			// ��¼�ӵ� 1 �Ĳ���, �����ɷ����ӵ� 2
			if ( i == 0 )
				mBullet_1_counter--;

			SignBullet(mBulletStruct[i].x, mBulletStruct[i].y, dir, P_B_SIGN + player_id * 10 + i);
		}
	}

	return BulletShootKind::None;
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
		// bug?	if (mBombTimer.IsTimeOut())
			{
				if (mBombS[i].counter++ >= 3)
				{
					mBombS[i].counter = 0;
					mBombS[i].canBomb = false;
				}
			}
		}
	}
}

// GameControl �ڵ���, ͨ�������� mBulletStruct.mKillId ���ݽ�ȥ
void PlayerBase::GetKillEnemy(int& bullet1, int& bullet2)
{
	bullet1 = mBulletStruct[0].mKillId;
	bullet2 = mBulletStruct[1].mKillId;
	
	// ���ñ�־
	mBulletStruct[0].mKillId = 0;
	mBulletStruct[1].mKillId = 0;
}

//
bool PlayerBase::IsShootCamp()
{
	return mIsShootCamp;
}

void PlayerBase::BeKill()
{
	SignBox_4(mTankX, mTankY, _EMPTY);
	mDied = true;

	// ���ñ�ը����
	mBlast.blastx = mTankX;
	mBlast.blasty = mTankY;
	mBlast.canBlast = true;
}

// ��ұ����б�ը
bool PlayerBase::Blasting(const HDC & center_hdc)
{
	int index[8] = { 0,1,2,3,4,3,2,1 };
	if (mBlast.canBlast)
	{
		TransparentBlt(center_hdc, mBlast.blastx - BOX_SIZE * 2, mBlast.blasty - BOX_SIZE * 2, BOX_SIZE * 4, BOX_SIZE * 4,
			GetImageHDC(&BlastStruct::image[index[mBlast.counter % 8]]), 0, 0, BOX_SIZE * 4, BOX_SIZE * 4, 0x000000);
		if (mBlastTimer.IsTimeOut())
		{
			if (mBlast.counter++ >= 7)
			{
				mBlast.counter = 0;
				mBlast.canBlast = false;

				// ����Ƿ��������
				if (mPlayerLife-- == 0)
					mPlayerLife = 0;
				else
					Reborn();
				return true;
			}
		}
	}
	return false;
}

//
const int& PlayerBase::GetID()
{
	return player_id;
}

/*GameControl ��ѭ������*/
bool PlayerBase::IsGetTimeProp()
{
	bool temp = mTimeProp;
	mTimeProp = false;
	return temp;
}

/*GameControl ��ѭ������*/
bool PlayerBase::IsGetBombProp()
{
	bool temp = mBombProp;
	mBombProp = false;
	return temp;
}

//
void PlayerBase::ShowProp(const HDC& center_hdc)
{
	mProp->ShowProp(center_hdc);
}

//
bool PlayerBase::ShowScorePanel(const HDC& image_hdc)
{
	return mScorePanel->show(image_hdc);// ���Ż���������ʾ image ��������
}

//
void PlayerBase::SetPause()
{
	mPause = true;
	mPauseCounter = 0;
}

//
void PlayerBase::SetShowProp()
{
	mProp->StartShowProp(100, 100);
}
void PlayerBase::AddKillEnemyNum(byte enemy_level)
{
	mKillEnemyNumber[enemy_level]++;
}
void PlayerBase::ResetScorePanelData(const int& player_num, const int& stage)
{
	mScorePanel->ResetData(mKillEnemyNumber, player_num, stage);
}
/////////////////////////////////////////////////////////////


void PlayerBase::SignBullet(int lx, int ty, byte dir, int val)
{
	// ת����ͷ����
	int hx = lx + BulletStruct::devto_head[dir][0];
	int hy = ty + BulletStruct::devto_head[dir][1];

	// ת���� 4*4 �����±�����
	int b4i = hy / SMALL_BOX_SIZE;
	int b4j = hx / SMALL_BOX_SIZE;

	bms->bullet_4[b4i][b4j] = val;
}

//---------------------------------------------------------------- private function ---------
void PlayerBase::Reborn()
{
	mDied = false;
	mTankX = (4 + 4 * player_id) * 16 + BOX_SIZE;				// ̹���״γ���ʱ�����������
	mTankY = 12 * 16 + BOX_SIZE;
	SignBox_4(mTankX, mTankY, PLAYER_SIGN + player_id);		// ̹�˳���, ���Ľ��Ǳ�Ǹ�Ϊ̹�˱��

	mPlayerTankLevel = 0;				// ̹�˼��� [0-3]
	mTankDir = DIR_UP;		// ̹�˷���

	for (int i = 0; i < 2; i++)
	{
		mBulletStruct[i].x = SHOOTABLE_X;		// x ���������ж��Ƿ���Է���
		mBulletStruct[i].y = -1000;
		mBulletStruct[i].dir = DIR_UP;
		mBulletStruct[i].mKillId = 0;			// ��¼���еĵл� id
	}

	mBullet_1_counter = 9;				// �ӵ� 1 �˶� N ��ѭ����ſ��Է����ӵ� 2 
	mMoving = false;
	mRing.SetShowable();			// ��ʾ������
}

void PlayerBase::DispatchProp(int prop_kind)
{
	mProp->StopShowProp();

	switch (prop_kind)
	{
	case ADD_PROP:			// �ӻ�
		mPlayerLife = mPlayerLife + 1 > 5 ? 5 : mPlayerLife + 1;
		break;
	case STAR_PROP:			// �����
		mPlayerTankLevel = mPlayerTankLevel + 1 > 3 ? 3 : mPlayerTankLevel + 1;
		break;
	case TIME_PROP:			// ʱ��
		mTimeProp = true;
		break;
	case  BOMB_PROP:		// ����
		mBombProp = true;
		break;
	case SHOVEL_PRO:		// ����
		break;
	case  CAP_PROP:			// ñ��
		mRing.SetShowable();
		break;
	default:
		break;
	}
}

// �����ͬʱ����̹�����ڸ���. ���뱣֤̹�������ڸ�������
void PlayerBase::Move(int new_dir)
{
	if (!mTankTimer.IsTimeOut() || mDied)
		return;

	// �����ұ���һ����һ�����ͣ
	if (mPause)
	{
		return;
	}

	SignBox_4(mTankX, mTankY, _EMPTY);

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
		if (CheckMoveable())
		{
			mTankX += mDevXY[mTankDir][0] * mSpeed[mPlayerTankLevel];
			mTankY += mDevXY[mTankDir][1] * mSpeed[mPlayerTankLevel];
		}
	}
	SignBox_4(mTankX, mTankY, PLAYER_SIGN + player_id);
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
bool PlayerBase::CheckMoveable()
{
	// ̹����������
	int tempx = mTankX + mDevXY[mTankDir][0] * mSpeed[mPlayerTankLevel];
	int tempy = mTankY + mDevXY[mTankDir][1] * mSpeed[mPlayerTankLevel];

	if (tempx < BOX_SIZE || tempy < BOX_SIZE || tempy > CENTER_WIDTH - BOX_SIZE || tempx > CENTER_HEIGHT - BOX_SIZE)
	{
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (mTankDir)
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

	// �ĸ�����̹�����ĵ������Ҫ�������� 8*8 ���ӵ��±�ƫ����
	int dev[4][2][2] = { {{-1,-1},{0,-1}},  {{-1,-1},{-1,0}},  {{-1,1},{0,1}}, { {1,-1},{1,0}} };

	// 8*8 �ϰ�����Ӽ��
	int temp1 = bms->box_8[index_i + dev[mTankDir][0][0]][index_j + dev[mTankDir][0][1]];
	int temp2 = bms->box_8[index_i + dev[mTankDir][1][0]][index_j + dev[mTankDir][1][1]];

	// prop_8���߸��Ӽ��
	int curi = mTankY / BOX_SIZE;	// ��ǰ̹�����ڵ�����, ������һ��������, �����жϵ���
	int curj = mTankX / BOX_SIZE;
	int prop[4] = { bms->prop_8[curi][curj],	 bms->prop_8[curi - 1][curj], 
					bms->prop_8[curi][curj - 1], bms->prop_8[curi - 1][curj - 1] };
	for (int i = 0; i < 4; i++)
	{
		if (prop[i] >= PROP_SIGN && prop[i] < PROP_SIGN + 6)
		{
			DispatchProp(prop[i] - PROP_SIGN);
			break;
		}
	}

	// ���̹�� 4*4 ����
	// �ĸ�������Ҫ�������� 4*4 �ĸ�����̹���������� 4*4 ���ӵ��±�ƫ����
	int  dev_4[4][4][2] = { {{-2,-2},{1,-2},{-1,-2},{0,-2}}, {{-2,-2},{-2,1},{-2,-1},{-2,0}},
							{{-2,2},{1,2},{-1,2},{0,2}}, {{2,-2},{2,1},{2,-1},{2,0}} };
	// ת���� [52][52] �±�
	int index_4i = tempy / SMALL_BOX_SIZE;
	int index_4j = tempx / SMALL_BOX_SIZE;
	
	// -1, 0, 1, 2 �������ƶ�
	bool tank1 = bms->box_4[index_4i + dev_4[mTankDir][0][0]][index_4j + dev_4[mTankDir][0][1]] <= _FOREST;
	bool tank2 = bms->box_4[index_4i + dev_4[mTankDir][1][0]][index_4j + dev_4[mTankDir][1][1]] <= _FOREST;
	bool tank3 = bms->box_4[index_4i + dev_4[mTankDir][2][0]][index_4j + dev_4[mTankDir][2][1]] <= _FOREST;
	bool tank4 = bms->box_4[index_4i + dev_4[mTankDir][3][0]][index_4j + dev_4[mTankDir][3][1]] <= _FOREST;

	// �����ϰ����������
	if (temp1 > 2 || temp2 > 2)
	{
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (mTankDir)
		{
		case DIR_LEFT:	mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;	break;	// mTankX �� tempx ֮���Խ�˸���, ��̹�˷ŵ�mTankX���ڵĸ�������
		case DIR_UP:	mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_RIGHT: mTankX = (tempx / BOX_SIZE) * BOX_SIZE;		break;
		case DIR_DOWN:	mTankY = (tempy / BOX_SIZE) * BOX_SIZE;		break;
		default:													break;
		}
		return false;
	}
	// ������Ҳ��õ���
	else if (!tank1 || !tank2 || !tank3 || !tank4)
		return false;
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
			mBulletStruct[0].x = (int)(mTankX + BulletStruct::devto_tank[mTankDir][0]);
			mBulletStruct[0].y = (int)(mTankY + BulletStruct::devto_tank[mTankDir][1]);
			mBulletStruct[0].dir = mTankDir;
			mBullet_1_counter = 6;

			SignBullet(mBulletStruct[0].x, mBulletStruct[0].y, mBulletStruct[0].dir, P_B_SIGN + player_id * 10 + bullet_id );
			return true;

		case 1:
			// 2 ���ӵ�����ʧ��
			if (mPlayerTankLevel < 2 || mBulletStruct[1].x != SHOOTABLE_X || mBullet_1_counter > 0 || mBombS[1].canBomb == true)
				return false;

			// �ӵ����������
			mBulletStruct[1].x = (int)(mTankX + BulletStruct::devto_tank[mTankDir][0]);
			mBulletStruct[1].y = (int)(mTankY + BulletStruct::devto_tank[mTankDir][1]);
			mBulletStruct[1].dir = mTankDir;

			SignBullet(mBulletStruct[1].x, mBulletStruct[1].y, mBulletStruct[1].dir, P_B_SIGN + player_id * 10 + bullet_id);
			return true;

		default:
			break;
	}
	return false;
}

//
BulletShootKind PlayerBase::CheckBomb(int i)
{
	//if (mBombS[i].canBomb)
	//	return true;
	int dir = mBulletStruct[i].dir;

	// �ӵ�ͷ�Ӵ����ϰ�����Ǹ���, ���ҷ��������, ���·��������
	int bombx = mBulletStruct[i].x + BulletStruct::devto_head[dir][0];
	int bomby = mBulletStruct[i].y + BulletStruct::devto_head[dir][1];

	bool flag = false;
	int adjust_x = 0, adjust_y = 0;		// ��������ͼƬ��ʾ������

	// ������ bombx ���� mBulletStruct[i].x,����Ḳ���ϰ���ļ��
	if (mBulletStruct[i].x < 0 && mBulletStruct[i].dir == DIR_LEFT)
	{
		flag = true;
		adjust_x = 5;					// ����ըͼƬ������һ��
	}
	else if (mBulletStruct[i].y < 0 && mBulletStruct[i].dir == DIR_UP)
	{
		flag = true;
		adjust_y = 5;
	}

	// �����ȥ�ӵ��Ŀ� 4, ��Ȼ�ӵ�Խ��, �����⵼�� box_8 �±�Խ��
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
		// �趨��ը����, ������ը�������ڵĸ���,���һ�����ƫ��һ������֮���..
		mBulletStruct[i].x = SHOOTABLE_X;
		mBombS[i].canBomb = true;
		mBombS[i].mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
		mBombS[i].mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
		mBombS[i].counter = 0;

		return BulletShootKind::Other;
	}

	int tempi, tempj;

	// �������� 8*8 ���ӵ�����
	int b8i = bomby / BOX_SIZE;
	int b8j = bombx / BOX_SIZE;

	// ������ת���� 4*4 ��������
	int b4i = bomby / SMALL_BOX_SIZE;
	int b4j = bombx / SMALL_BOX_SIZE;

	switch (mBulletStruct[i].dir)
	{
	case DIR_LEFT:
	case DIR_RIGHT:
	{
		// �����������һ������ӵ�
		if (bms->bullet_4[b4i][b4j] == P_B_SIGN + (1 - player_id) * 10 + 0 || 
			bms->bullet_4[b4i][b4j] == P_B_SIGN + (1 - player_id) * 10 + 1 )
		{
			mBulletStruct[i].x = SHOOTABLE_X;
			printf("����ӵ������ӵ�\n");

			for (list<PlayerBase*>::iterator itor = mPList->begin(); itor != mPList->end(); itor++)
			{
				if ((*itor)->GetID() != player_id)
				{
					(*itor)->DisappearBullet(bms->bullet_4[b4i][b4j]);
					break;
				}
			}
			return BulletShootKind::Other;
		}

		// ������Ӻ���һ��
		int temp[2][2] = { {0, 0}, {-1, 0} };
		for (int n = 0; n < 2; n++)
		{
			// 8*8 ����, �ж��Ƿ���ел�
			tempi = b8i + temp[n][0];
			tempj = b8j + temp[n][1];
			if (bms->box_8[tempi][tempj] == CAMP_SIGN)
			{
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].counter = 0;
				mIsShootCamp = true;
				SignBox_8(13 * BOX_SIZE, 25 * BOX_SIZE, _EMPTY);
				return BulletShootKind::Camp;
			}

			// ���Ҽ���ӵ�ͷ���ڵ�4*4���Ӻ����������ڵ��Ǹ�
			// ��� 4*4 ����, �ɴ��ж��ϰ���
			tempi = b4i + temp[n][0];
			tempj = b4j + temp[n][1];
			if (bms->box_4[tempi][tempj] == _WALL || bms->box_4[tempi][tempj] == _STONE)
			{
				// �趨��ը����, ������ը�������ڵĸ���,���һ�����ƫ��һ������֮���..
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
				mBombS[i].mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
				mBombS[i].mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
				mBombS[i].counter = 0;
				ClearWallOrStone(i, bombx, bomby);
				return BulletShootKind::Other;
			}
			else if (bms->box_4[tempi][tempj] >= ENEMY_SIGN /*&& bms->box_4[tempi][tempj] < ENEMY_SIGN + TOTAL_ENEMY_NUMBER*/)
			{
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
				mBombS[i].mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
				mBombS[i].mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
				mBombS[i].counter = 0;

				// ��ǻ����˵л��� id
				mBulletStruct[i].mKillId = bms->box_4[tempi][tempj];
				//mKillEnemyNumber[bms->box_4[tempi][tempj] % 10000 / 1000]++;	// ��¼����л��ļ������������

				//	mProp->StartShowProp(100, 100);
				return BulletShootKind::Other;
			}
			else if (bms->box_4[tempi][tempj] == PLAYER_SIGN && player_id != 0 || bms->box_4[tempi][tempj] == PLAYER_SIGN + 1 && player_id != 1)
			{
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
				mBombS[i].mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
				mBombS[i].mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
				mBombS[i].counter = 0;
				return (BulletShootKind)bms->box_4[tempi][tempj];
			}
		}
	}
	break;

	// ����ֻ����������ڵ���������
	case DIR_UP:
	case DIR_DOWN:
	{
		// �����������һ������ӵ�
		if (bms->bullet_4[b4i][b4j] == P_B_SIGN + (1 - player_id) * 10 + 0 ||
			bms->bullet_4[b4i][b4j] == P_B_SIGN + (1 - player_id) * 10 + 1)
		{
			mBulletStruct[i].x = SHOOTABLE_X;
			printf("����ӵ������ӵ�\n");

			for (list<PlayerBase*>::iterator itor = mPList->begin(); itor != mPList->end(); itor++)
			{
				if ((*itor)->GetID() != player_id)
				{
					(*itor)->DisappearBullet(bms->bullet_4[b4i][b4j]);
					break;
				}
			}
			return BulletShootKind::Other;
		}

		// ������Ӻ������һ������
		int temp[2][2] = { { 0, 0 },{ 0, -1 } };
		for (int n = 0; n < 2; n++)
		{
			// 8*8 ����, �ж��Ƿ���ел�
			tempi = b8i + temp[n][0];
			tempj = b8j + temp[n][1];
			if (bms->box_8[tempi][tempj] == CAMP_SIGN)
			{
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].counter = 0;
				mIsShootCamp = true;
				SignBox_8(13 * BOX_SIZE, 25 * BOX_SIZE, _EMPTY);
				return BulletShootKind::Camp;
			}

			// ��� 4*4 �Ƿ�����ϰ�
			tempi = b4i + temp[n][0];
			tempj = b4j + temp[n][1];
			if (bms->box_4[tempi][tempj] == _WALL || bms->box_4[tempi][tempj] == _STONE)
			{
				// �趨��ը����, ������ը�������ڵĸ���,���һ�����ƫ��һ������֮���..
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
				mBombS[i].mBombX =( bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
				mBombS[i].mBombY =( bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
				mBombS[i].counter = 0;
				ClearWallOrStone(i, bombx, bomby );
				return BulletShootKind::Other;
			}
			else if (bms->box_4[tempi][tempj] >= ENEMY_SIGN/* && bms->box_4[tempi][tempj] < ENEMY_SIGN + TOTAL_ENEMY_NUMBER*/)
			{
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
				mBombS[i].mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
				mBombS[i].mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
				mBombS[i].counter = 0;

				// ��ǻ����˵л��� id
				mBulletStruct[i].mKillId = bms->box_4[tempi][tempj];
				//mKillEnemyNumber[bms->box_4[tempi][tempj] % 10000 / 1000]++;		// ��¼����л������������
				//mProp->StartShowProp(100, 100);
				return BulletShootKind::Other;
			}
			else if (bms->box_4[tempi][tempj] == PLAYER_SIGN && player_id != 0 || bms->box_4[tempi][tempj] == PLAYER_SIGN + 1 && player_id != 1)
			{
				mBulletStruct[i].x = SHOOTABLE_X;
				mBombS[i].canBomb = true;				// ָʾ i bomb ��ը
				mBombS[i].mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][0]) * SMALL_BOX_SIZE;
				mBombS[i].mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct[i].dir][1]) * SMALL_BOX_SIZE;
				mBombS[i].counter = 0;
				return (BulletShootKind)bms->box_4[tempi][tempj];
			}
		}
	}
		break;
	default:
		break;
	}
	return BulletShootKind::None;
}

// �ӵ������ϰ��ﱬը���øú���, ���б߽粻�ɵ���, �±��Խ��[52][52]
void PlayerBase::ClearWallOrStone(int bulletid, int bulletx, int bullety)
{
	int boxi = bullety / SMALL_BOX_SIZE;
	int boxj = bulletx / SMALL_BOX_SIZE;
	int tempx, tempy;
	switch (mBulletStruct[bulletid].dir)
	{
	case DIR_LEFT:
	case DIR_RIGHT:
	{
		// ��ͬһֱ�����ڵ��ĸ� 4*4 ����, ˳���ܱ�, �����õ��±��ж�
		int temp[4][2] = { { -2, 0 },{ -1, 0 },{ 0, 0 },{ 1, 0 } };
		for (int i = 0; i < 4; i++)
		{
			tempx = boxi + temp[i][0];
			tempy = boxj + temp[i][1];
			if (bms->box_4[tempx][tempy] == _WALL || mPlayerTankLevel == 3 && bms->box_4[tempx][tempy] == _STONE)
				bms->box_4[tempx][tempy] = _CLEAR;

			// ת�� tempx,tempy���ڵ� 8*8 ��������
			int n = tempx / 2;
			int m = tempy / 2;

			// ��� 8*8 �����ڵ�4�� 4*4 ��С�����Ƿ�ȫ�������,
			bool isClear = true;	
			for (int a = 2 * n; a < 2 * n + 2; a++)
			{
				for (int b = 2 * m; b < 2 * m + 2; b++)
				{
					if (bms->box_4[a][b] != _CLEAR)
						isClear = false;
				}
			}
			if (isClear)
			{
				bms->box_8[n][m] = _EMPTY;
			}
		}
	}
	break;

	case DIR_UP:
	case DIR_DOWN:
	{
		// ���ڵ��ĸ� 4*4 ����, ˳���ܱ�, �����õ��±��ж�
		int temp[4][2] = { {0, -2}, {0, -1}, {0, 0}, {0, 1} };
		for (int i = 0; i < 4; i++)
		{
			tempx = boxi + temp[i][0];
			tempy = boxj + temp[i][1];
			if (bms->box_4[tempx][tempy] == _WALL || mPlayerTankLevel == 3 && bms->box_4[tempx][tempy] == _STONE)
				bms->box_4[tempx][tempy] = _CLEAR;

			// ת�� tempx,tempy���ڵ� 8*8 ��������
			int n = tempx / 2;
			int m = tempy / 2;

			// ��� 8*8 �����ڵ�4�� 4*4 ��С�����Ƿ�ȫ�������,
			bool isClear = true;
			for (int a = 2 * n; a < 2 * n + 2; a++)
			{
				for (int b = 2 * m; b < 2 * m + 2; b++)
				{
					if (bms->box_4[a][b] != _CLEAR)
						isClear = false;
				}
			}
			if (isClear)
			{
				bms->box_8[n][m] = _EMPTY;
			}
		}
	}
	break;

	default:
		break;
	}
}

// ������ 16 * 16 ���ĵ���������, ��̹������������ͬ
void PlayerBase::SignBox_8(int x, int y, int val)
{
	// ��̹����������ת�����Ͻ��Ǹ��� ��������
	int iy = y / BOX_SIZE - 1;
	int jx = x / BOX_SIZE - 1;
	for (int i = iy; i < iy + 2; i++)
	{
		for (int j = jx; j < jx + 2; j++)
		{
			bms->box_8[i][j] = val;
		}
	}
}

// ����̹����������, ���16�� 4*4 ����
void PlayerBase::SignBox_4(int cx, int cy, int val)
{
	// ���ҵ��� cs,cy ��ռ�ݰٷֱ����� 16 �� 4*4 �ĸ�������
	if (mTankDir == DIR_LEFT || mTankDir == DIR_RIGHT)
	{
		if (cx > (cx / SMALL_BOX_SIZE) * SMALL_BOX_SIZE + SMALL_BOX_SIZE / 2)	// ����ǿ����ұ߽ڵ�, 
		{
			cx = (cx / SMALL_BOX_SIZE + 1) * SMALL_BOX_SIZE;
		}
		else {
			cx = (cx / SMALL_BOX_SIZE) * SMALL_BOX_SIZE;					// �����������ϵ���߽ڵ�
		}
	}
	// ����
	else
	{
		if (cy > (cy / SMALL_BOX_SIZE) * SMALL_BOX_SIZE + SMALL_BOX_SIZE / 2 )	// ����ǿ��������±߽ڵ�,
			cy = (cy / SMALL_BOX_SIZE + 1) * SMALL_BOX_SIZE;
		else
			cy = (cy / SMALL_BOX_SIZE) * SMALL_BOX_SIZE;					// �����������ϵ��ϱ߽ڵ�
	}

	// ��̹����������ת�����Ͻ��Ǹ��� ��������
	int iy = cy / SMALL_BOX_SIZE - 2;
	int jx = cx / SMALL_BOX_SIZE - 2;
	for (int i = iy; i < iy + 4; i++)
	{
		for (int j = jx; j < jx + 4; j++)
		{
			bms->box_4[i][j] = val;
		}
	}
}

// ��� 16*16 �����ڵ� 4*4 �����Ƿ��ж���
bool PlayerBase::CheckBox_4(int cx, int cy)
{
	// ��̹����������ת�����Ͻ��Ǹ��� ��������
	int iy = cy / SMALL_BOX_SIZE - 2;
	int jx = cx / SMALL_BOX_SIZE - 2;
	for (int i = iy; i < iy + 4; i++)
	{
		for (int j = jx; j < jx + 4; j++)
		{
			if (bms->box_4[i][j] > _FOREST)
				return false;
		}
	}
	return true;
}

void PlayerBase::DisappearBullet(int sign)
{
	int bid = sign % 10;
	mBulletStruct[bid].x = SHOOTABLE_X;
}



