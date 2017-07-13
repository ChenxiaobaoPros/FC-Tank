#include "stdafx.h"
#include "struct.h"


// ---------------- �ӵ��ṹ��̬����
IMAGE BulletStruct::mBulletImage[4];
int BulletStruct::mBulletSize[4][2] = { { 4,3 },{ 3,4 },{ 4,3 },{ 3,4 } };			// ��ͬ�����ӵ��ߴ�(width height)

																					// �ӵ����̹�����ĵ�ƫ����
int BulletStruct::devto_tank[4][2] = { { -BOX_SIZE, -1 },{ -2, -BOX_SIZE },{ BOX_SIZE - 4, -1 },{ -2, BOX_SIZE - 4 } };

// �ӵ�ͼƬ���Ͻ�����ת������ͷ����,���ҷ�����͹���������ǵ�, ���·�������͹�����ұ��ǵ�
int BulletStruct::devto_head[4][2] = { { 0, 1 },{ 2, 0 },{ 4, 1 },{ 2, 4 } };

int BulletStruct::bomb_center_dev[4][2] = { { 1, 0 },{ 0, 1 },{ 0, 0 },{ 0, 0 } };	// ��ը����������ӵ�ͷ��ƫ����

//------------------------

IMAGE BombStruct::mBombImage[3];

//----------------------------------

IMAGE BlastStruct::image[5];
BlastStruct::BlastStruct()
{
	blastx = -100;
	blasty = -100;
	canBlast = false;
	counter = 0;
}

//-----------------------------------------

IMAGE StarClass::mStarImage[4];
StarClass::StarClass()
{
	mStarIndexDev = -1;						// ���������±�����ӻ��Ǽ���
	mStarIndex = 3;							// star ͼƬ�����±�
	mStarCounter = 0;						// ���ٴθ��� star ͼƬ
	mTankOutAfterCounter = rand() % 100 + 10;
	mIsOuted = false;						// ̹���Ƿ��Ѿ�����
}

//------------------------------------------
IMAGE RingClass::image[2];
RingClass::RingClass()
{
	canshow = true;
	index_counter = 0;
}

void RingClass::ShowRing(const HDC &canvas_hdc, int mTankX, int mTankY)
{
	if (index_counter > 290)
	{
		canshow = false;
		index_counter = 0;
		return;
	}
	TransparentBlt(canvas_hdc, (int)(mTankX - BOX_SIZE), (int)(mTankY - BOX_SIZE), BOX_SIZE * 2, 
			BOX_SIZE * 2, GetImageHDC(&image[index_counter++ % 2]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
}

void RingClass::SetShowable()
{
	canshow = true;
	index_counter = 0;
}

/////////////////////////////////////////////////////

PropClass::PropClass()
{
	//bms = _bms;
	leftx = -100;
	topy = -100;
	index_counter = 0;
	can_show = false;

	TCHAR buf[100];
	for (int i = 0; i < 6; i++)
	{
		_stprintf_s(buf, _T("./res/big/prop/p%d.gif"), i);
		loadimage(&image[i], buf);
	}
}

// GameControl ��ѭ�����ú���
void PropClass::ShowProp(const HDC &canvas_hdc)
{
	if (!can_show)
		return;

	if ( (++index_counter / 4) % 2 == 0 )
		TransparentBlt(canvas_hdc, leftx, topy, BOX_SIZE * 2,
			BOX_SIZE * 2, GetImageHDC(&image[prop_kind]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);

	// ����ʱ�� ��ʧ
	if (index_counter > 300)
		StopShowProp();
}

// ���ߵл���������øú���
void PropClass::StartShowProp(int _x, int _y)
{
	leftx = 12 * BOX_SIZE;// (rand() % 25 + 1) * BOX_SIZE;
	topy = 12 * BOX_SIZE; //(rand() % 25 + 1) * BOX_SIZE;
	can_show = true;
	prop_kind = 3;// rand() % 6;		// �������һ������
	index_counter = 0;
	SignPropBox(PROP_SIGN + prop_kind);
}

//
void PropClass::StopShowProp()
{
	can_show = false;
	SignPropBox(_EMPTY);
}

void PropClass::SetBoxMarkStruct(BoxMarkStruct * b)
{
	bms = b;
}

//
void PropClass::SignPropBox(int val)
{
	int i = topy / BOX_SIZE;
	int j = leftx / BOX_SIZE;

	for (int ix = i; ix < i + 2; ix++)
	{
		for (int jy = j; jy < j + 2; jy++)
		{
			bms->prop_8[ix][jy] = val;
		}
	}
}
/*
IMAGE PropClass::image[6];
int PropClass::prop_kind = ADD_PROP;
*/
