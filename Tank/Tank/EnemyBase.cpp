#include "stdafx.h"
#include "EnemyBase.h"

/////////////////////////////////////////////////////////
// �л�̹�˿���

int EnemyBase::mDevXY[4][2] = { { -1, 0 },{ 0, -1 },{ 1, 0 },{ 0, 1 } };	// ������������
//bool EnemyBase::isOuting = false;

// ����ĳ����𼶱�ĵл�
EnemyBase::EnemyBase(byte kind, int level, BoxMarkStruct* b)
{
	mEnemyTankKind = kind;
	mEnemyTankLevel = level;
	mEnemyTank = new TankInfo(mEnemyTankKind, mEnemyTankLevel, true);
	bms = b;

	int tempx[3] = {BOX_SIZE, 13 * BOX_SIZE, 25 * BOX_SIZE};	// ̹��������ֵ�����λ�� x ����
	mTankX = tempx[rand() % 3];
	mTankY = BOX_SIZE;
	mTankDir = DIR_DOWN;

	TCHAR buf[100];
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(buf, _T("./res/big/star%d.gif"), i);
		loadimage(&mStarImage[i], buf);
	}
	mStarIndexDev = -1;						// ���������±�����ӻ��Ǽ���
	mStarIndex = 3;							// star ͼƬ�����±�
	mStarCounter = 0;						// ���ٴθ��� star ͼƬ
	mTankOutAfterCounter = rand() % 400 + 10;
	mIsOuted = false;						// ̹���Ƿ��Ѿ�����

	mStep = rand() % 200;					// ��ǰ��������ƶ��Ĳ���

	// ��ͬ����̹���ƶ��ٶ�ϵ��
	int temp[4] = { 1, 1, 1, 1 };
	for (int i = 0; i < 4; i++)
		mSpeed[i] = temp[i];

	mTankNumberReduce = true;				// ̹��������һ��־
}

EnemyBase::~EnemyBase()
{
	//if (mEnemyTank != NULL)
		//delete mEnemyTank;
}

// ��ʾ̹��
bool EnemyBase::ShowStar(const HDC& center_hdc, int& total)
{
	// ̹���Ѿ�����,������˸,ֱ�ӷ���
	if (mIsOuted == true)
		return STOP_SHOW_STAR;

	// һ��ʱ������ʾ�Ľ���, ֮ǰ����
	if (mTankOutAfterCounter-- > 0)
		return SHOWING_STAR;

	// �Ľ��ǳ���, ʣ��̹����-1;
	if (mTankNumberReduce)
	{
		total -= 1;
		mTankNumberReduce = false;
	}

	// ��ʼ��˸�Ľ���
	if (mStarCounter++ % 2 == 0)
	{
		if (mStarIndex + mStarIndexDev < 0)
		{
			mStarIndex = 1;
			mStarIndexDev = 1;
		}
		else if (mStarIndex + mStarIndexDev > 3)
		{
			mStarIndex = 2;
			mStarIndexDev = -1;
		}
		else
		{
			mStarIndex += mStarIndexDev;
		}
		if (mStarCounter == 35)
		{
			mIsOuted = true;				// ������˸, TankMoving() ������ʼѭ��, ̹�˿�ʼ�ƶ�
			return STOP_SHOW_STAR;
		}
	}

	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&mStarImage[mStarIndex]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000 );

	return SHOWING_STAR;
}

void EnemyBase::TankMoving(const HDC& center_hdc)
{
	if (!mIsOuted)
		return;

	// �ض���
	if (mStep-- < 0)
		RejustDirPosition();

	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
			GetImageHDC(&mEnemyTank->GetTankImage(mTankDir)), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
	
	// ���ƶ�
	if (CheckMoveable())
	{
		UnSignBox_4();
		mTankX += mDevXY[mTankDir][0] * mSpeed[mEnemyTankLevel];
		mTankY += mDevXY[mTankDir][1] * mSpeed[mEnemyTankLevel];
		SignBox_4(ENEMY_SIGN);
	}

	// �����ƶ�,�ض���
	else
	{
		RejustDirPosition();
	}
}

//----------------- ˽�к��� ------------------------

// ��ǻ�ȡ��̹�����ڵ� 4*4 = 16 ������
void EnemyBase::SignBox_4(int value)
{
	// box_4[i][j] ��Ӧ����
	int ix = (int)(2 * mTankY) / BOX_SIZE - 2;		// -2 �Ǵ����ĵ����µ�һ�������Ƶ����Ͻ��Ǹ�����
	int jy = (int)(2 * mTankX) / BOX_SIZE - 2;
	for (int i = ix; i < ix + 4; i++)
	{
		for (int j = jy; j < jy + 4; j++)
		{
			bms->box_4[i][j] = value;
		}
	}
}

// ȡ�����
void EnemyBase::UnSignBox_4()
{
	int ix = 2 * (int)mTankY / BOX_SIZE - 2;
	int jy = 2 * (int)mTankX / BOX_SIZE - 2;
	for (int i = ix; i < ix + 4; i++)
	{
		for (int j = jy; j < jy + 4; j++)
		{
			bms->box_4[i][j] = 0;
		}
	}
}

//
bool EnemyBase::CheckSignBox(int x, int y)
{
	int ix = x / (BOX_SIZE / 2) - 2;
	int jy = y / (BOX_SIZE / 2) - 2;

	for (int i = ix; i < ix + 4; i++)
	{
		for (int j = jy; j < jy + 4; j++)
		{
			if (bms->box_4[i][j] != 0)
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
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (mTankDir)
		{
		case DIR_LEFT:	mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;	break;	// mTankX �� tempx ֮���Խ�˸���, ��̹�˷ŵ�mTankX���ڵĸ�������
		case DIR_UP:	mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_RIGHT: mTankX = (tempx / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_DOWN:	mTankY = (tempy / BOX_SIZE) * BOX_SIZE;	break;
		default:														break;
		}
		return false;
	}
	// ת�����ص����ڵ� xy[26][26] �±�
	int index_i = (int)tempy / BOX_SIZE;
	int index_j = (int)tempx / BOX_SIZE;

	int dev[4][2][2] = { { { -1,-1 },{ 0,-1 } },{ { -1,-1 },{ -1,0 } },{ { -1,1 },{ 0,1 } },{ { 1,-1 },{ 1,0 } } };

	if (bms->box_8[index_i + dev[mTankDir][0][0]][index_j + dev[mTankDir][0][1]] > 2 ||
		bms->box_8[index_i + dev[mTankDir][1][0]][index_j + dev[mTankDir][1][1]] > 2)
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
	return true;
}

void EnemyBase::RejustDirPosition()
{
	mStep = rand() % 50;

	// ��Ҫ���±��, ����λ�ÿ��ܻ�ı����ڵ� 4*4 ����
	UnSignBox_4();

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
	SignBox_4(ENEMY_SIGN);
}
