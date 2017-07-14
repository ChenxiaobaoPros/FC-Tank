#include "stdafx.h"
#include "GameControl.h"
#include "typeinfo.h"

int GameControl::mCurrentStage = 1;	// [1-35]
GameControl::GameControl( HDC des_hdc, HDC image_hdc/*, BoxMarkStruct* bms*/)
{
	//mGraphics = grap;
	mDes_hdc = des_hdc;
	mImage_hdc = image_hdc;
	mCenterImage.Resize( CENTER_WIDTH, CENTER_HEIGHT );
	mCenter_hdc = GetImageHDC(&mCenterImage);
	mBoxMarkStruct = new BoxMarkStruct();
	Init();
}

GameControl::~GameControl()
{
}

void GameControl::Init()
{
	loadimage( &mBlackBackgroundImage, _T("./res/big/bg_black.gif"		));		// ��ɫ����
	loadimage( &mGrayBackgroundImage , _T("./res/big/bg_gray.gif"		));		// ��ɫ����
	loadimage( &mStoneImage			 , _T("./res/big/stone.gif"			));		// 12*12��ʯͷ
	loadimage( &mForestImage		 , _T("./res/big/forest.gif"		));		// ����
	loadimage( &mIceImage			 , _T("./res/big/ice.gif"			));		// ����
	loadimage( &mRiverImage[0]		 , _T("./res/big/river-0.gif"		));		// ����
	loadimage( &mRiverImage[1]		 , _T("./res/big/river-1.gif"		));		//
	loadimage( &mWallImage			 , _T("./res/big/wall.gif"			));		// ��ǽ
	loadimage( &mCamp[0]			 , _T("./res/big/camp0.gif"			));		// ��Ӫ
	loadimage( &mCamp[1]			 , _T("./res/big/camp1.gif"			));		// 
	loadimage( &mEnemyTankIcoImage	 , _T("./res/big/enemytank-ico.gif"	));		// �л�ͼ��
	loadimage( &mFlagImage			 , _T("./res/big/flag.gif"			));		// ����
	loadimage( &mBlackNumberImage	 , _T("./res/big/black-number.gif"	));		// 0123456789 ��ɫ����

	mActiveEnemyTankNumber = 0;													// �Ѿ������ڵ�ͼ�ϵĵл�����,�����ʾ6��
	mRemainEnemyTankNumber = 20;												// ʣ��δ���ֵĵл�����
	mCampDie = false;															// ��־��Ӫ�Ƿ񱻻���

	mEnemyPause = false;			// �л���ͣ���
	mEnemyPauseCounter = 0;			// �л���ͣ���

	//PlayerList = new ListTable<PlayerBase*>();
}

// �洢��ҽ�����
void GameControl::AddPlayer(int player_num)
{
	for (int i = 0; i < player_num; i++)
		//PlayerList.push_back( new PlayerBase(i, mBoxMarkStruct/*, &mProp*/) );	// �����������
		PlayerList.Add(new PlayerBase(i, mBoxMarkStruct/*, &mProp*/) );
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

	// ��ʼ����Ǹ��ָ���
	int x = 0, y = 0;
	for ( int i = 0; i < 26; i++ )
	{
		for ( int j = 0; j < 26; j++ )
		{
			SignBoxMark( i, j, mMap.buf[i][j] - '0' );		// ��� 26*26 �� 52*52 ����
			mBoxMarkStruct->prop_8[i][j] = 0;
		}
	}

	// ��Ǵ�Ӫ
	for (int i = 24; i < 26; i++)
	{
		for (int j = 12; j < 14; j++)
		{
			mBoxMarkStruct->box_8[i][j] = CAMP_SIGN;
		}
	}

	while (StartGame())
	{
	AddEnemy();
		Sleep(24);
	}
}

bool GameControl::StartGame()
{
	// �����ұ���������, ���ж�, ��Ϊ����Ҫ�������� mImage_hdc
	RefreshRightPanel();

	// ����������Ϸ����: mCenter_hdc
	RefreshCenterPanel();

	// �����Ļ���ӡ�������� mImage_hdc ��
	BitBlt( mImage_hdc, CENTER_X, CENTER_Y, CENTER_WIDTH, CENTER_HEIGHT, mCenter_hdc, 0, 0, SRCCOPY );
	
	// ���Ż���������ʾ image ��������
	StretchBlt( mDes_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mImage_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY );
	FlushBatchDraw();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////// ˽�к���,����ʹ�� //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// ���޸�, ��ӵĵл�������Ҫ�޸�
void GameControl::AddEnemy()
{
	//for (int i = 0; i < TOTAL_ENEMY_NUMBER; i++)
	if (EnemyList.size() < 6 && mRemainEnemyTankNumber > 0)
	{
		//printf("%d - %d\n", EnemyList.size(), mRemainEnemyTankNumber);
		//EnemyList.push_back((new BigestTank(TANK_KIND::PROP, mBoxMarkStruct)));
		EnemyList.push_back((new PropTank(2, mBoxMarkStruct)));
		//mRemainEnemyTankNumber--;
		//EnemyList.push_back((new CommonTank(2, mBoxMarkStruct)));
	}
}

// ��� 26*26 �� 52*52 �ĸ���
void GameControl::SignBoxMark(int i, int j, int sign_val)
{
	mBoxMarkStruct->box_8[i][j] = sign_val;	// 26*26
	int temp_i[4] = { 2 * i, 2 * i + 1, 2 * i, 2 * i + 1 };
	int temp_j[4] = { 2 * j, 2 * j, 2 * j + 1, 2 * j + 1 };

	for ( int i = 0; i < 4; i++ )
		mBoxMarkStruct->box_4[ temp_i[i] ][ temp_j[i] ] = sign_val;
}

void GameControl::RefreshRightPanel()
{
	// ��ɫ����
	StretchBlt(mImage_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY);

	// ��ʾ�л�����ͼ��
	int x[2] = {233, 241};
	int n, index;
	for ( int i = 0; i < mRemainEnemyTankNumber; i++ )
	{
		n = i / 2;
		index = i % 2;

		TransparentBlt( mImage_hdc, x[index], 19 + n * 8, ENEMY_TANK_ICO_SIZE, ENEMY_TANK_ICO_SIZE, 
			GetImageHDC(&mEnemyTankIcoImage), 0, 0, ENEMY_TANK_ICO_SIZE, ENEMY_TANK_ICO_SIZE, 0xffffff );	// ע�����ͼ���к�ɫ����
	}
	
	// ���1P\2P\̹��ͼ��\������
	/*for (list<PlayerBase*>::iterator PlayerItor = PlayerList.begin(); PlayerItor != PlayerList.end(); PlayerItor++)
	{
		(*PlayerItor)->DrawPlayerTankIco(mImage_hdc);		// ̹��ͼ��
	}*/
	for (ListNode<PlayerBase*>* p = PlayerList.First(); p != NULL; p = p->pnext)
	{
		p->data->DrawPlayerTankIco(mImage_hdc);
	}

	// ����
	TransparentBlt(mImage_hdc, 232, 177, FLAG_ICO_SIZE_X, FLAG_ICO_SIZE_Y,
		GetImageHDC(&mFlagImage), 0, 0, FLAG_ICO_SIZE_X, FLAG_ICO_SIZE_Y, 0xffffff);	// ע��ͼ�����к�ɫ����

	// �ؿ�
	if (mCurrentStage < 10)
		TransparentBlt(mImage_hdc, 238, 193, 7, 7, GetImageHDC(&mBlackNumberImage),
			BLACK_NUMBER_SIZE * mCurrentStage, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	else	// 10,11,12 .. ˫λ���ؿ�
	{
		TransparentBlt(mImage_hdc, 233, 193, 7, 7, GetImageHDC(&mBlackNumberImage),
			BLACK_NUMBER_SIZE * (mCurrentStage / 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
		TransparentBlt(mImage_hdc, 241, 193, 7, 7, GetImageHDC(&mBlackNumberImage),
			BLACK_NUMBER_SIZE * (mCurrentStage % 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
}

// �����м���Ϸ����
void GameControl::RefreshCenterPanel()
{

	BitBlt(mCenter_hdc, 0, 0, CENTER_WIDTH, CENTER_HEIGHT, GetImageHDC(&mBlackBackgroundImage), 0, 0, SRCCOPY);// ���ĺ�ɫ������Ϸ��
																											  
	// �Ľ�����˸����
	for (list<EnemyBase*>::iterator EnemyItor = EnemyList.begin(); EnemyItor != EnemyList.end(); EnemyItor++)
	{
		// һ���Ľ��Ƕ�����������ִ����һ��
		if ((*EnemyItor)->ShowStar(mCenter_hdc, mRemainEnemyTankNumber) == SHOWING_STAR)
		{
			break;
		}
	}

	/* ��ʼ���������ļ����Ƶ�ͼ
	* ����Ϊ BOX_SIZE x BOX_SIZE �ĸ���
	* x���꣺ j*BOX_SIZE
	* y���꣺ i*BOX_SIZE
	*/
	int x = 0, y = 0;
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			x = j * BOX_SIZE;// +CENTER_X;
			y = i * BOX_SIZE;// +CENTER_Y;
			switch (mBoxMarkStruct->box_8[i][j])
			{
			case _WALL:
				BitBlt(mCenter_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mWallImage), 0, 0, SRCCOPY);
				break;
			//case _FOREST:
			//	BitBlt(mCenter_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mForestImage), 0, 0, SRCCOPY);
				//break;
			case _ICE:
				BitBlt(mCenter_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mIceImage), 0, 0, SRCCOPY);
				break;
			case _RIVER:
				BitBlt(mCenter_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mRiverImage[0]), 0, 0, SRCCOPY);
				break;
			case _STONE:
				BitBlt(mCenter_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mStoneImage), 0, 0, SRCCOPY);
				break;
			default:
				break;
			}
		}
	}

	// ��ⱻ���ٵ��ϰ���, ���ƺ�ɫͼƬ����
	for (int i = 0; i < 52; i++)
	{
		for (int j = 0; j < 52; j++)
		{
			if (mBoxMarkStruct->box_4[i][j] == _CLEAR)
			{
				BitBlt(mCenter_hdc, j * SMALL_BOX_SIZE, i * SMALL_BOX_SIZE, SMALL_BOX_SIZE, SMALL_BOX_SIZE,
					GetImageHDC(&mBlackBackgroundImage), 0, 0, SRCCOPY);
			}
		}
	}

	// �������Ƿ��� 'ʱ��' ��ֹ����
	if (PlayerBase::IsGetTimeProp())
	{
		mEnemyPause = true;
		mEnemyPauseCounter = 0;
	}

	// ��һ�õ��׵���
	if (PlayerBase::IsGetBombProp())
	{
		for (list<EnemyBase*>::iterator EnemyItor = EnemyList.begin(); EnemyItor != EnemyList.end(); EnemyItor++)
		{
			(*EnemyItor)->BeKill();
			//mRemainEnemyTankNumber--;
			mActiveEnemyTankNumber--;
		}
	}

	// ���
	/*for (list<PlayerBase*>::iterator PlayerItor = PlayerList.begin(); PlayerItor != PlayerList.end(); PlayerItor++)
	{
		(*PlayerItor)->ShowStar(mCenter_hdc);
		(*PlayerItor)->DrawPlayerTank(mCenter_hdc);		// ̹��
		(*PlayerItor)->PlayerControl();
		(*PlayerItor)->BulletMoving(mCenter_hdc);
		CheckKillEnemy(PlayerItor);

		if ((*PlayerItor)->IsShootCamp())
		{
			if (mBlast.canBlast == false)
			{
				int index[17] = { 0,0,0,1,1,2,2,3,3,4,4,4,4,3,2,1,0 };
				TransparentBlt(mCenter_hdc, 11 * BOX_SIZE, 23 * BOX_SIZE, BOX_SIZE * 4, BOX_SIZE * 4,
					GetImageHDC(&BlastStruct::image[index[mBlast.counter % 17]]), 0, 0, BOX_SIZE * 4, BOX_SIZE * 4, 0x000000);
				if (mBlast.counter++ == 17)
					mBlast.canBlast = true;
				mCampDie = true;
			}
		}
	}*/
	for (ListNode<PlayerBase*>* p = PlayerList.First(); p != NULL; p = p->pnext)
	{
		p->data->ShowStar(mCenter_hdc);
		p->data->DrawPlayerTank(mCenter_hdc);		// ̹��
		p->data->PlayerControl();
		p->data->BulletMoving(mCenter_hdc);
		CheckKillEnemy(p->data);

		if (p->data->IsShootCamp())
		{
			if (mBlast.canBlast == false)
			{
				int index[17] = { 0,0,0,1,1,2,2,3,3,4,4,4,4,3,2,1,0 };
				TransparentBlt(mCenter_hdc, 11 * BOX_SIZE, 23 * BOX_SIZE, BOX_SIZE * 4, BOX_SIZE * 4,
					GetImageHDC(&BlastStruct::image[index[mBlast.counter % 17]]), 0, 0, BOX_SIZE * 4, BOX_SIZE * 4, 0x000000);
				if (mBlast.counter++ == 17)
					mBlast.canBlast = true;
				mCampDie = true;
			}
		}
	}

	// �л�
	for (list<EnemyBase*>::iterator EnemyItor = EnemyList.begin(); EnemyItor != EnemyList.end(); EnemyItor++)
	{
		(*EnemyItor)->DrawTank(mCenter_hdc);
		(*EnemyItor)->ShootBullet();

		// ����л���ͣ
		if (mEnemyPause == false)
		{
			(*EnemyItor)->TankMoving(mCenter_hdc);
			(*EnemyItor)->BulletMoving(mCenter_hdc);
		}
		else if ( mEnemyPauseCounter++ > 4300 )
		{
			mEnemyPause = false;
			mEnemyPauseCounter = 0;;
		}

		CheckKillPlayer(EnemyItor);
	}

	// ɭ��
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			x = j * BOX_SIZE;// +CENTER_X;
			y = i * BOX_SIZE;// +CENTER_Y;
			if (mBoxMarkStruct->box_8[i][j] == _FOREST)
				BitBlt(mCenter_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mForestImage), 0, 0, SRCCOPY);
		}
	}

	// �л��ӵ�\̹�˱�ըͼ
	for (list<EnemyBase*>::iterator EnemyItor = EnemyList.begin(); EnemyItor != EnemyList.end(); EnemyItor++)
	{
		(*EnemyItor)->Bombing(mCenter_hdc);

		// ��ը���, �Ƴ��л�
		if ((*EnemyItor)->Blasting(mCenter_hdc))
		{
			//EnemyItor = EnemyList.erase(EnemyItor);		// ɾ�����һ�ܵл���ʱ����쳣!!
			EnemyList.erase(EnemyItor);
			break;
		}

		// ����õл����д�Ӫ
		if ((*EnemyItor)->IsShootCamp())
		{
			if (mBlast.canBlast == false)
			{
				int index[17] = { 0,0,0,1,1,2,2,3,3,4,4,4,4,3,2,1,0 };
				TransparentBlt(mCenter_hdc, 11 * BOX_SIZE, 23 * BOX_SIZE, BOX_SIZE * 4, BOX_SIZE * 4,
					GetImageHDC(&BlastStruct::image[index[mBlast.counter % 17]]), 0, 0, BOX_SIZE * 4, BOX_SIZE * 4, 0x000000);
				if (mBlast.counter++ == 17)
					mBlast.canBlast = true;
				mCampDie = true;
			}
		}
	}

	// ����ӵ���ը
	/*for (list<PlayerBase*>::iterator PlayerItor = PlayerList.begin(); PlayerItor != PlayerList.end(); PlayerItor++)
	{
		(*PlayerItor)->Bombing(mCenter_hdc);

		// ��ը��ɺ�
		if ((*PlayerItor)->Blasting(mCenter_hdc))
		{
			//PlayerItor = PlayerList.erase(PlayerItor);	// ���ܸ�ֵ??! ɾ�����һ�����ݵ�ʱ�� bug �쳣!!
			//break;
		}
	}*/
	for (ListNode<PlayerBase*>* p = PlayerList.First(); p != NULL; p = p->pnext)
	{
		p->data->Bombing(mCenter_hdc);

		// ��ը��ɺ�
		if (p->data->Blasting(mCenter_hdc))
		{
			//PlayerItor = PlayerList.erase(PlayerItor);	// ���ܸ�ֵ??! ɾ�����һ�����ݵ�ʱ�� bug �쳣!!
			//break;
		}
	}

	//mProp.ShowProp(mCenter_hdc);
	PlayerBase::ShowProp(mCenter_hdc);

	// ��Ӫ
	if (!mCampDie)		// ���û��ը
	{
		TransparentBlt(mCenter_hdc, BOX_SIZE * 12, BOX_SIZE * 24, BOX_SIZE * 2, BOX_SIZE * 2,
			GetImageHDC(&mCamp[0]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
	}
	else if (mBlast.canBlast)	// �����ը���, ��ʾ���ݻٵ�camp
	{
		TransparentBlt(mCenter_hdc, BOX_SIZE * 12, BOX_SIZE * 24, BOX_SIZE * 2, BOX_SIZE * 2,
			GetImageHDC(&mCamp[1]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
	}
}

// ��ȡPlayerBase �ڵ�����, ����л�
void GameControl::CheckKillEnemy(PlayerBase* pb)
{
	int bullet[2] = {0, 0};
	//(*pb)->GetKillEnemy(bullet[0], bullet[1]);		// ��ȡ��һ��еĵл�id, �洢�� bullet[2] ��
	pb->GetKillEnemy(bullet[0], bullet[1]);		// ��ȡ��һ��еĵл�id, �洢�� bullet[2] ��

	for (int i = 0; i < 2; i++)
	{
		if (bullet[i] >= ENEMY_SIGN && bullet[i] < ENEMY_SIGN + TOTAL_ENEMY_NUMBER)
		{
			for (list<EnemyBase*>::iterator EnemyItor = EnemyList.begin(); EnemyItor != EnemyList.end(); EnemyItor++)
			{
				if ((*EnemyItor)->GetId() + ENEMY_SIGN == bullet[i])
				{
					//delete (EnemyBase*)(&(*EnemyItor));  ????
					(*EnemyItor)->BeKill();
					mActiveEnemyTankNumber--;

					// ������ʾ����
					//if ((*EnemyItor)->GetKind() == TANK_KIND::PROP)
						//mProp.StartShowProp(100, 100);
					//EnemyItor = EnemyList.erase(EnemyItor); //�ŵ���ըͼ��ʾ��ȫ֮���ٵ���
					break;
				}
			}
		}
	}
}

void GameControl::CheckKillPlayer(list<EnemyBase*>::iterator enemyItor)
{
	int id = (*enemyItor)->IsShootToPlayer();
	if (id == 0)
		return;

	/*for (list<PlayerBase*>::iterator itor = PlayerList.begin(); itor != PlayerList.end(); itor++)
	{
		if ((*itor)->GetID() + PLAYER_SIGN == id)
		{
			(*itor)->BeKill();
			break;
		}
	}*/
	for (ListNode<PlayerBase*>* p = PlayerList.First(); p != NULL; p = p->pnext)
	{
		if (p->data->GetID() + PLAYER_SIGN == id)
		{
			p->data->BeKill();
			break;
		}
	}
}

/*
void GameControl::SignBox_8(int iy, int jx, int val)
{
	for (int i = iy; i < iy + 2; i++)
	{
		for (int j = jx; j < jx + 2; j++)
		{
			mBoxMarkStruct->box_8[i][j] = CAMP_SIGN;
		}
	}
}*/
