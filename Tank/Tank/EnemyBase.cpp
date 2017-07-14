#include "stdafx.h"
#include "EnemyBase.h"

/////////////////////////////////////////////////////////
// �л�̹�˿���

int EnemyBase::mDevXY[4][2] = { { -1, 0 },{ 0, -1 },{ 1, 0 },{ 0, 1 } };	// ������������
//bool EnemyBase::isOuting = false;
//bool EnemyBase::mIsPause = false;
//int EnemyBase::mPauseCounter = 0;

// ����ĳ����𼶱�ĵл�
EnemyBase::EnemyBase(TANK_KIND kind, byte level, BoxMarkStruct* b)
{
	mEnemyTankKind = kind;
	mEnemyTankLevel = level;
	mDied = false;
	mIsShootCamp = false;
	bms = b;

	int tempx[3] = {BOX_SIZE, 13 * BOX_SIZE, 25 * BOX_SIZE};	// ̹��������ֵ�����λ�� x ����
	mTankX = tempx[rand() % 3];
	mTankY = BOX_SIZE;
	mTankDir = DIR_DOWN;
	mTankImageIndex = 0;

	mStep = rand() % 200;					// ��ǰ��������ƶ��Ĳ���

	// ��ͬ����̹���ƶ��ٶ�ϵ��
	int temp[4] = { 1, 1, 1, 1 };
	for (int i = 0; i < 4; i++)
		mSpeed[i] = temp[i];

	mTankNumberReduce = true;				// ̹��������һ��־

	// �ӵ���ʼ��
	// .. ���������� PlayerBase ������
	mBulletStruct.x = SHOOTABLE_X;
	mBulletStruct.y = -100;
	mBulletStruct.dir = DIR_DOWN;
	for (int i = 0; i < 4; i++)
		mBulletStruct.speed[i] = 3;		// ���ܳ��� 4
	mBulletStruct.mKillId = 0;			// ��¼�������̹�˵�id
	
	//mBulletT1 = timeGetTime();
	//mBulletT2 = timeGetTime();
	//mBulletT = rand() % 111 + 320;		// �����ӵ��ļ��ʱ�� 4000 

	// ��ըͼƬ
	mBombS.mBombX = -100;
	mBombS.mBombY = -100;
	mBombS.canBomb = false;
	mBombS.counter = 0;

	// �洢�ӵ��������,��ҵ�id
	mShootedPlayerID = 0;

	// �л��ƶ�ʱ����
	mTankTimer.SetDrtTime(50);

	// �ӵ��ƶ�ʱ����
	mBulletTimer.SetDrtTime(40);

	// �����ӵ�Ƶ��
	mShootTimer.SetDrtTime(30);

	// �ӵ���ը�ٶ�
	mBombTimer.SetDrtTime(37);

	// ̹�˱�ը����
	mBlastTimer.SetDrtTime(37);
}

EnemyBase::~EnemyBase()
{
	//if (mEnemyTank != NULL)
		//delete mEnemyTank;
}

// ��ʾ̹��
bool EnemyBase::ShowStar(const HDC& center_hdc, int& remainnumber)
{
	// ̹���Ѿ�����,������˸,ֱ�ӷ���
	if (mStar.mIsOuted == true)
		return STOP_SHOW_STAR;

	// һ��ʱ������ʾ�Ľ���, ֮ǰ����
	if (mStar.mTankOutAfterCounter-- > 0)
		return SHOWING_STAR;

	// ���̹�˳��ֵ�λ�ñ�������, �ȴ���һ�����ѭ���ڳ���
	if (CheckBox_8() == false)
	{
		mStar.mTankOutAfterCounter = rand() % 100 + 10;
		return SHOWING_STAR;
	}

	// �Ľ��ǳ���, ʣ��̹����-1;
	if (mTankNumberReduce)
	{
		mEnemyId = TOTAL_ENEMY_NUMBER - remainnumber;
		remainnumber -= 1;
		mTankNumberReduce = false;

		// ���Ϊ STAR_SIGN = 2000, 2000 ����̹�˲��ܴ��еı�־
		SignTank_8(mTankX, mTankY, STAR_SIGN);
	}

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
		if (mStar.mStarCounter == 35)
		{
			mStar.mIsOuted = true;						// ������˸, TankMoving() ������ʼѭ��, ̹�˿�ʼ�ƶ�
			//mShootTimer.Timer1();	//ע�ͺ�̹��һ����һ�㶼�����̷����ӵ�??
			SignTank_8(mTankX, mTankY, ENEMY_SIGN + mEnemyId);		// ̹�˳���, ���Ľ��Ǳ�Ǹ�Ϊ̹�˱��
			return STOP_SHOW_STAR;
		}
	}

	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&StarClass::mStarImage[mStar.mStarIndex]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000 );

	return SHOWING_STAR;
}

void EnemyBase::TankMoving(const HDC& center_hdc)
{
	if (!mStar.mIsOuted || mDied || mTankTimer.IsTimeOut() == false )
		return;
	
	//mBulletT2 = timeGetTime();

	// �ƶ�ǰȡ�����
	SignTank_8(mTankX, mTankY, _EMPTY);

	// �ض���
	if (mStep-- < 0)
		RejustDirPosition();
	
	// ���ƶ�
	if (CheckMoveable())
	{
		//SignBox_8(0);
		mTankX += mDevXY[mTankDir][0] * mSpeed[mEnemyTankLevel];
		mTankY += mDevXY[mTankDir][1] * mSpeed[mEnemyTankLevel];
		//SignBox_8(ENEMY_SIGN + mEnemyId);
	}

	// �����ƶ�,�ض���
	else
	{
		RejustDirPosition();
	}

	// ����λ�����±��
	SignTank_8(mTankX, mTankY, ENEMY_SIGN + mEnemyId);
}
/*
void EnemyBase::DrawTank(const HDC& center_hdc)
{
	if (!mStar.mIsOuted || mDied)
		return;
	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
			GetImageHDC(&mEnemyTank->GetTankImage(mTankDir, mTankImageIndex++)), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
	
}*/

// 
void EnemyBase::DrawBullet(const HDC& center_hdc)
{
	// ����ӵ�û���ƶ����ߵл�����
	if (mBulletStruct.x == SHOOTABLE_X || mDied)
		return;
	int dir = mBulletStruct.dir;

	TransparentBlt(center_hdc, mBulletStruct.x, mBulletStruct.y, BulletStruct::mBulletSize[dir][0],
		BulletStruct::mBulletSize[dir][1], GetImageHDC(&BulletStruct::mBulletImage[dir]),
		0, 0, BulletStruct::mBulletSize[dir][0], BulletStruct::mBulletSize[dir][1], 0x000000);
}

//
bool EnemyBase::ShootBullet()
{
	if (mBulletStruct.x != SHOOTABLE_X || !mShootTimer.IsTimeOut() || mDied || mStar.mIsOuted == false )
		return false;

	// �ӵ����������
	mBulletStruct.x = mTankX + BulletStruct::devto_tank[mTankDir][0];
	mBulletStruct.y = mTankY + BulletStruct::devto_tank[mTankDir][1];
	mBulletStruct.dir = mTankDir;

	return true;
}

//
void EnemyBase::BulletMoving(const HDC& center_hdc)
{
	// ����ӵ�û���ƶ����ߵл�����
	if (mBulletStruct.x == SHOOTABLE_X || mDied || !mBulletTimer.IsTimeOut() )
		return;
	
	// �����ҳԵ���ͣ����
	//if (CheckPause())
	//	return;

	// ����ӵ��ڱ�ը
	if (CheckBomb())
		return;

	int dir = mBulletStruct.dir;
	mBulletStruct.x += mDevXY[dir][0] * mBulletStruct.speed[mEnemyTankLevel];
	mBulletStruct.y += mDevXY[dir][1] * mBulletStruct.speed[mEnemyTankLevel];
	/*
	TransparentBlt(center_hdc, mBulletStruct.x, mBulletStruct.y, BulletStruct::mBulletSize[dir][0],
		BulletStruct::mBulletSize[dir][1], GetImageHDC(&BulletStruct::mBulletImage[dir]),
		0, 0, BulletStruct::mBulletSize[dir][0], BulletStruct::mBulletSize[dir][1], 0x000000);
		*/
}

//
void EnemyBase::Bombing(const HDC & center_hdc)
{
	int index[3] = { 0,1,2 };
	if (mBombS.canBomb)
	{
		TransparentBlt(center_hdc, mBombS.mBombX - BOX_SIZE, mBombS.mBombY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
			GetImageHDC(&BombStruct::mBombImage[index[mBombS.counter % 3]]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
		if (mBombTimer.IsTimeOut())
		{
			if ( mBombS.counter++ == 3 )
				mBombS.canBomb = false;
		}
	}
}

// ��Gamecontrol�ڼ��, Ȼ�����
void EnemyBase::BeKill()
{
	// ����л���û�г���
	if (mStar.mIsOuted == false)
		return;

	mDied = true;
	SignTank_8(mTankX, mTankY, _EMPTY);

	// ���ñ�ը����
	mBlast.blastx = mTankX;
	mBlast.blasty = mTankY;
	mBlast.canBlast = true;
}

// ��ʾ̹�˱�ըЧ��, GameControl ��ѭ�����
bool EnemyBase::Blasting(const HDC& center_hdc)
{
	int index[6] = {0,1,2,3,4,2};
	if (mBlast.canBlast)
	{
		TransparentBlt(center_hdc, mBlast.blastx - BOX_SIZE * 2, mBlast.blasty - BOX_SIZE * 2, BOX_SIZE * 4, BOX_SIZE * 4,
			GetImageHDC(&BlastStruct::image[index[mBlast.counter % 6]]), 0, 0, BOX_SIZE * 4, BOX_SIZE * 4, 0x000000);
		if (mBlastTimer.IsTimeOut())
		{
			if (mBlast.counter++ == 6)
			{
				mBlast.canBlast = false;
				return true;
			}
		}
	}
	return false;
}

bool EnemyBase::IsShootCamp()
{
	return mIsShootCamp;
}

// ���ر�������� id ���� 0 û�л���
int EnemyBase::IsShootToPlayer()
{
	int temp = mShootedPlayerID;
	mShootedPlayerID = 0;			// ��ȡ�����! ��Ȼһֱ����ǻ��и����
	return temp;
}

int EnemyBase::GetId()
{
	return mEnemyId;
}

TANK_KIND EnemyBase::GetKind()
{
	return mEnemyTankKind;
}

//----------------- ˽�к��� ------------------------
/*
void EnemyBase::SetPause(bool val)
{
	mIsPause = val;
}*/

// ��ǻ�ȡ��̹�����ڵ� 4*4 = 16 ������
void EnemyBase::SignBox_4(int value)
{
	// box_4[i][j] ��Ӧ����
	int ix = mTankY / (BOX_SIZE / 2) - 2;		// -2 �Ǵ����ĵ����µ�һ�������Ƶ����Ͻ��Ǹ�����
	int jy = mTankX / (BOX_SIZE / 2) - 2;
	for (int i = ix; i < ix + 4; i++)
	{
		for (int j = jy; j < jy + 4; j++)
		{
			bms->box_4[i][j] = value;
		}
	}
}

// x,y �� 16*16 ���ĵ�����
void EnemyBase::SignBox_8(int x, int y, int value)
{
	// ��̹����������ת�����Ͻ��Ǹ��� ��������
	int iy = y / BOX_SIZE - 1;
	int jx = x / BOX_SIZE - 1;
	for (int i = iy; i < iy + 2; i++)
	{
		for (int j = jx; j < jx + 2; j++)
		{
			bms->box_8[i][j] = value;
		}
	}
}

void EnemyBase::SignTank_8(int x, int y, int val)
{
	// ��̹����������ת�����Ͻ��Ǹ��� ��������
	int iy = y / BOX_SIZE - 1;
	int jx = x / BOX_SIZE - 1;
	for (int i = iy; i < iy + 2; i++)
	{
		for (int j = jx; j < jx + 2; j++)
		{
			bms->tank_8[i][j] = val;
		}
	}
}

// ���ĳ��16*16λ�ÿ��Է�̹����, x,y 16*16�����ĵ�
bool EnemyBase::CheckBox_8()
{
	int iy = mTankY / BOX_SIZE - 1;
	int jx = mTankX / BOX_SIZE - 1;
	for (int i = iy; i < iy + 2; i++)
	{
		for (int j = jx; j < jx + 2; j++)
		{
			if (bms->box_8[i][j] != STAR_SIGN && bms->box_8[i][j] != _EMPTY ||  
				bms->tank_8[i][j] >= PLAYER_SIGN && bms->tank_8[i][j] <= ENEMY_SIGN + TOTAL_ENEMY_NUMBER )
				return false;
		}
	}
	return true;
}

//
bool EnemyBase::CheckMoveable()
{
	// ̹����������
	int tempx = mTankX + mDevXY[mTankDir][0] * mSpeed[mEnemyTankLevel];
	int tempy = mTankY + mDevXY[mTankDir][1] * mSpeed[mEnemyTankLevel];

	if (tempx < BOX_SIZE || tempy < BOX_SIZE || tempy > CENTER_WIDTH - BOX_SIZE || tempx > CENTER_HEIGHT - BOX_SIZE)
	{
		//SignBox_8(_EMPTY);
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (mTankDir)
		{
		case DIR_LEFT:	mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;	break;	// mTankX �� tempx ֮���Խ�˸���, ��̹�˷ŵ�mTankX���ڵĸ�������
		case DIR_UP:	mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_RIGHT: mTankX = (tempx / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_DOWN:	mTankY = (tempy / BOX_SIZE) * BOX_SIZE;	break;
		default:														break;
		}
		//SignBox_8(ENEMY_SIGN + mEnemyId);
		return false;
	}
	// ת�����ص����ڵ� xy[26][26] �±�
	int index_i = (int)tempy / BOX_SIZE;
	int index_j = (int)tempx / BOX_SIZE;

	int dev[4][2][2] = { { { -1,-1 },{ 0,-1 } },{ { -1,-1 },{ -1,0 } },{ { -1,1 },{ 0,1 } },{ { 1,-1 },{ 1,0 } } };

	// ��������ϰ���������л�,����������־..
	if (bms->box_8[index_i + dev[mTankDir][0][0]][index_j + dev[mTankDir][0][1]] > 2 ||
		bms->tank_8[index_i + dev[mTankDir][1][0]][index_j + dev[mTankDir][0][1]] != ENEMY_SIGN + mEnemyId &&
		bms->tank_8[index_i + dev[mTankDir][1][0]][index_j + dev[mTankDir][0][1]] >= ENEMY_SIGN ||
		bms->box_8[index_i + dev[mTankDir][1][0]][index_j + dev[mTankDir][1][1]] > 2 ||
		bms->tank_8[index_i + dev[mTankDir][1][0]][index_j + dev[mTankDir][1][1]] != ENEMY_SIGN + mEnemyId &&
		bms->tank_8[index_i + dev[mTankDir][1][0]][index_j + dev[mTankDir][1][1]] >= ENEMY_SIGN)
	{
		//SignBox_8(_EMPTY);
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (mTankDir)
		{
		case DIR_LEFT:	mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;	break;	// mTankX �� tempx ֮���Խ�˸���, ��̹�˷ŵ�mTankX���ڵĸ�������
		case DIR_UP:	mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_RIGHT: mTankX = (tempx  / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_DOWN:	mTankY = (tempy  / BOX_SIZE) * BOX_SIZE;	break;
		default:													break;
		}
		//SignBox_8(ENEMY_SIGN + mEnemyId);
		return false;
	}
	return true;
}

//
void EnemyBase::RejustDirPosition()
{
	mStep = rand() % 250;

	// ��Ҫ���±��, ����λ�ÿ��ܻ�ı����ڵ� 4*4 ����
	//SignBox_8(0);

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

	// �ض���, �������λ�ú�������÷���
	mTankDir = rand() % 4;
	//SignBox_8(ENEMY_SIGN + mEnemyId);
}

//
bool EnemyBase::CheckBomb()
{
	int dir = mBulletStruct.dir;

	// �ӵ�ͷ�Ӵ����ϰ�����Ǹ���, ���ҷ��������, ���·��������
	int bombx = mBulletStruct.x + BulletStruct::devto_head[dir][0];
	int bomby = mBulletStruct.y + BulletStruct::devto_head[dir][1];

	bool flag = false;
	int adjust_x = 0, adjust_y = 0;		// ��������ͼƬ��ʾ������

	// ������ bombx ���� mBulletStruct[i].x,����Ḳ���ϰ���ļ��
	if (mBulletStruct.x < 0 && dir == DIR_LEFT)
	{
		flag = true;
		adjust_x = 5;					// ����ըͼƬ������һ��
	}
	else if (mBulletStruct.y < 0 && dir == DIR_UP)
	{
		flag = true;
		adjust_y = 5;
	}

	// �����ȥ�ӵ��Ŀ� 4, ��Ȼ�ӵ�Խ��, �����⵼�� box_8 �±�Խ��
	else if (mBulletStruct.x >= CENTER_WIDTH - 4 && dir == DIR_RIGHT)
	{
		flag = true;
		adjust_x = -4;
	}
	else if (mBulletStruct.y >= CENTER_HEIGHT - 4 && dir == DIR_DOWN)
	{
		flag = true;
		adjust_y = -4;
	}
	if (flag)
	{
		// �趨��ը����, ������ը�������ڵĸ���,���һ�����ƫ��һ������֮���..
		mBulletStruct.x = SHOOTABLE_X;
		mBombS.canBomb = true;
		mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[dir][0]) * SMALL_BOX_SIZE;
		mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[dir][1]) * SMALL_BOX_SIZE;
		mBombS.counter = 0;
		return true;
	}

	int tempi, tempj;

	// 4*4 ��������
	int bi = bomby / SMALL_BOX_SIZE;
	int bj = bombx / SMALL_BOX_SIZE;

	// �������� 8*8 ���ӵ�����
	int b8i = bomby / BOX_SIZE;
	int b8j = bombx / BOX_SIZE;

	switch (dir)
	{
		// ���Ҽ���ӵ�ͷ���ڵ�4*4���Ӻ����������ڵ��Ǹ�
	case DIR_LEFT:
	case DIR_RIGHT:
	{
		int temp[2][2] = { { 0, 0 },{ -1, 0 } };
		for (int n = 0; n < 2; n++)
		{
			// 8*8 ����, �ж��Ƿ�������
			tempi = b8i + temp[n][0];
			tempj = b8j + temp[n][1];
			if (bms->tank_8[tempi][tempj] == PLAYER_SIGN || bms->tank_8[tempi][tempj] == PLAYER_SIGN + 1)
			{
				mBulletStruct.x = SHOOTABLE_X;
				mBombS.canBomb = true;				// ָʾ i bomb ��ը
				mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][0]) * SMALL_BOX_SIZE;
				mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][1]) * SMALL_BOX_SIZE;
				mBombS.counter = 0;

				mShootedPlayerID = bms->tank_8[tempi][tempj];
				return true;
			}
			else if (bms->box_8[tempi][tempj] == CAMP_SIGN)
			{
				mBombS.counter = 0;
				mBulletStruct.x = SHOOTABLE_X;
				mIsShootCamp = true;
				SignBox_8(13 * BOX_SIZE, 25 * BOX_SIZE, _EMPTY);
				return true;
			}

			// 4*4 ���
			tempi = bi + temp[n][0];
			tempj = bj + temp[n][1];
			if (bms->box_4[tempi][tempj] == _WALL || bms->box_4[tempi][tempj] == _STONE )
			{
				// �趨��ը����, ������ը�������ڵĸ���,���һ�����ƫ��һ������֮���..
				mBulletStruct.x = SHOOTABLE_X;
				mBombS.canBomb = true;				// ָʾ i bomb ��ը
				mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][0]) * SMALL_BOX_SIZE;
				mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][1]) * SMALL_BOX_SIZE;
				mBombS.counter = 0;
				ShootWhat(bombx, bomby);
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
			// 8*8 ����, �ж��Ƿ����
			tempi = b8i + temp[n][0];
			tempj = b8j + temp[n][1];
			if (bms->tank_8[tempi][tempj] == PLAYER_SIGN || bms->tank_8[tempi][tempj] == PLAYER_SIGN + 1)
			{
				mBulletStruct.x = SHOOTABLE_X;
				mBombS.canBomb = true;				// ָʾ i bomb ��ը
				mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][0]) * SMALL_BOX_SIZE;
				mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][1]) * SMALL_BOX_SIZE;
				mBombS.counter = 0;

				mShootedPlayerID = bms->tank_8[tempi][tempj];
				return true;
			}
			else if (bms->box_8[tempi][tempj] == CAMP_SIGN)
			{
				mBombS.counter = 0;
				mBulletStruct.x = SHOOTABLE_X;
				mIsShootCamp = true;
				SignBox_8(13 * BOX_SIZE, 25 * BOX_SIZE, _EMPTY);
				return true;
			}

			// 4*4 ���
			tempi = bi + temp[n][0];
			tempj = bj + temp[n][1];
			if (bms->box_4[tempi][tempj] == _WALL || bms->box_4[tempi][tempj] == _STONE)
			{
				// �趨��ը����, ������ը�������ڵĸ���,���һ�����ƫ��һ������֮���..
				mBulletStruct.x = SHOOTABLE_X;
				mBombS.canBomb = true;				// ָʾ i bomb ��ը
				mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][0]) * SMALL_BOX_SIZE;
				mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][1]) * SMALL_BOX_SIZE;
				mBombS.counter = 0;
				ShootWhat(bombx, bomby);
				return true;
			}
		}
	}
	break;
	default:
		break;
	}
	return false;
}

void EnemyBase::ShootWhat(int bulletx, int bullety)
{
	int boxi = bullety / SMALL_BOX_SIZE;
	int boxj = bulletx / SMALL_BOX_SIZE;
	int tempx, tempy;
	switch (mBulletStruct.dir)
	{
	case DIR_LEFT:
	case DIR_RIGHT:
	{
		// ���ڵ��ĸ� 4*4 ����, ˳���ܱ�, �����õ��±��ж�
		int temp[4][2] = { { -2, 0 },{ -1, 0 },{ 0, 0 },{ 1, 0 } };
		for (int i = 0; i < 4; i++)
		{
			tempx = boxi + temp[i][0];
			tempy = boxj + temp[i][1];
			if (bms->box_4[tempx][tempy] == _WALL )
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
		int temp[4][2] = { { 0, -2 },{ 0, -1 },{ 0, 0 },{ 0, 1 } };
		for (int i = 0; i < 4; i++)
		{
			tempx = boxi + temp[i][0];
			tempy = boxj + temp[i][1];
			if (bms->box_4[tempx][tempy] == _WALL )
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




CommonTank::CommonTank( byte level, BoxMarkStruct* bms) :
	EnemyBase(TANK_KIND::COMMON, level, bms)
{
	mTank = new TankInfo(GRAY_TANK, level, true);
}

// 
void CommonTank::DrawTank(const HDC& center_hdc)
{
	if (!mStar.mIsOuted || mDied)
		return;
	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&mTank->GetTankImage(mTankDir, mTankImageIndex++)), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
}

//////////////////////////////////////////////////////////////////////////////


PropTank::PropTank(byte level, BoxMarkStruct* bms) :
	EnemyBase(TANK_KIND::PROP, level, bms)
{
	mTank[0] = new TankInfo(GRAY_TANK, level, true);
	mTank[1] = new TankInfo(RED_TANK, level, true);
}

// 
void PropTank::DrawTank(const HDC& center_hdc)
{
	if (!mStar.mIsOuted || mDied)
		return;
	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&mTank[ index_counter++ % 2 ]->GetTankImage(mTankDir, mTankImageIndex++)), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
}

//////////////////////////////////////////////////////////////

BigestTank::BigestTank(TANK_KIND kind, BoxMarkStruct * bms):
	EnemyBase(kind, 3, bms)
{
	mTank[GRAY_TANK] = new TankInfo(GRAY_TANK, 3, true);
	mTank[YELLOW_TANK] = new TankInfo(YELLOW_TANK, 3, true);
	mTank[RED_TANK] = new TankInfo(RED_TANK, 3, true);
	mTank[GREEN_TANK] = new TankInfo(GREEN_TANK, 3, true);
}

void BigestTank::DrawTank(const HDC & center_hdc)
{
	if (!mStar.mIsOuted || mDied)
		return;

	// ����̹�˺���̹ͨ�˱�ɫ����
	TankInfo* temp = NULL;
	switch (mEnemyTankKind)
	{
	case TANK_KIND::PROP:
		if (index_counter++ % 3)
			temp = mTank[RED_TANK];
		else
			temp = mTank[GRAY_TANK];
		break;
	case TANK_KIND::COMMON:
		if (index_counter++ % 3)
			temp = mTank[GREEN_TANK];
		else
			temp = mTank[GRAY_TANK];
		break;
	}

	if (temp == NULL)
	{
		printf("����!. EnemyBase.cpp");
		return;
	}
	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&temp->GetTankImage(mTankDir, mTankImageIndex++)), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
}
