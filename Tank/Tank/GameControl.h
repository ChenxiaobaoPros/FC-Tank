#pragma once
#include "PlayerBase.h"
#include "EnemyBase.h"
#include "List.h"

struct Map
{
	char buf[26][27];
};

/* �๦��:
* �� main ���������ڻ�ͼ�ӿ�
* ����һ�� IMAGE ����, ���Ƶ�ͼ/�л��б���Ϣ/����/�ؿ�
* �洢���ӱ��
* �洢�������
* ������Ϸѭ��
*/

class GameControl
{
public:
	GameControl( HDC des_hdc, HDC image_hdc/*, BoxMarkStruct* */);
	~GameControl();
	void Init();
	void AddPlayer(int player_num);					// ������ҽ��������
	void LoadMap();									// ��ȡ .map ��ͼ�ļ�|�������Ƶ�ͼ
	bool StartGame();								// ���»�����Ϸ���ֶ���, ���� false ������Ϸ

private:
	void AddEnemy();						// ��Ϸ��ʼǰ ���20 �ܵл�
	void SignBoxMark(int i, int j, int sign_val);	// ��Ǹ���, LoadMap() �ڵ���
	void RefreshRightPanel();						// ˢ���ұ���Ϣ�������, �������ݱ仯�ŵ��ø���
	void RefreshCenterPanel();						// ˢ���м���Ϸ���� 208 * 208
	void CheckKillEnemy(PlayerBase*);		// �������Ƿ���ел�, ������ɾ���õл�
	void CheckKillPlayer(list<EnemyBase*>::iterator);					// 
	//void SignBox_8(int i, int j, int val);					// ���ݲ������Ͻ� 8*8 ���ӵ�����, ��� �ĸ� 8*8 ����

public:
	static int mCurrentStage;						// [1-35] ��ǰ�ؿ�, SelectPanel ��ʹ��, ������ʹ��

private:
	// ���ڻ�ͼ�ӿ�
	//Graphics* mGraphics;
	IMAGE mCenterImage;				// ���ڱ��� mCenter_hdc ������Ч��.
	HDC mDes_hdc,					// ������hdc, main ����
		mImage_hdc,					// 256*224 �� hdc, main ����
		mCenter_hdc;				// 208*208 �� hdc, �������, ����Ϸ����,�ֿ�,��������,����Ҳ�����趨

	BoxMarkStruct* mBoxMarkStruct;			// �洢���ӱ��
	//list<PlayerBase*> PlayerList;			// �洢����б�
	ListTable<PlayerBase*> PlayerList;
	//list<PlayerBase*>::iterator PlayerItor;	// ���ڱ��� PlayerList

	list<EnemyBase*> EnemyList;				// �л��б�
	//list<EnemyBase*>::iterator EnemyItor;	// ���ڱ��� PlayerList

	IMAGE mGrayBackgroundImage;		// ��Ϸ��ɫ����ͼ
	IMAGE mBlackBackgroundImage;	// ��ɫ����ͼ

	Map mMap;						// �洢��ͼ���ݽṹ
	IMAGE mStoneImage;				// ʯͷ
	IMAGE mForestImage;				// ����
	IMAGE mIceImage;				// ����
	IMAGE mRiverImage[2];			// ����
	IMAGE mWallImage;				// ��ǽ
	IMAGE mCamp[2];					// ��Ӫ

	IMAGE mEnemyTankIcoImage;		// �л�̹��ͼ��
	IMAGE mFlagImage;				// ����
	IMAGE mBlackNumberImage;		// 0123456789 ��ǰ�ؿ���

	// ��ʼ mActiveEnemyTankNumber + mRemainEnemyTankNumber = 20
	int mActiveEnemyTankNumber;		// ��ǰ��ĵл�����, �����ʾ 6 ��
	int mRemainEnemyTankNumber;		// ����δ��ʾ�ĵл�����, ��ʼֵ=20, ����һ�ܵл���ֵ��һ

	BlastStruct mBlast;				// ��Ӫ��ը
	bool mCampDie;					// ��Ӫ������, ���� Camp ��ͼ 

	//PropClass mProp;			// ������		

	bool mEnemyPause;			// �Ƿ�����ƶ��л�
	int mEnemyPauseCounter;		// �л���ͣ����
};