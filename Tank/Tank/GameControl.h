#pragma once
#include "PlayerBase.h"

struct Map
{
	char buf[26][27];
};

class GameControl
{
public:
	GameControl(Graphics* grap, HDC des_hdc, HDC image_hdc, BoxMarkStruct* );
	~GameControl();
	void Init();
	void AddPlayer(int player_num);					// ������ҽ��������
	void LoadMap();									// ��ȡ .map ��ͼ�ļ�|�������Ƶ�ͼ
	bool StartGame();								// ���»�����Ϸ���ֶ���, ���� false ������Ϸ

private:
	void ShowEnemyTankIco();						// ����ʣ��л�������ʾ�л�ͼ������
	void SignBoxMark(int i, int j, char sign_val);	// ��Ǹ���, LoadMap() �ڵ���

public:
	static int mCurrentStage;						// [1-35] ��ǰ�ؿ�, SelectPanel ��ʹ��, ������ʹ��

private:
	// ���ڻ�ͼ�ӿ�
	Graphics* mGraphics;
	HDC mDes_hdc, mImage_hdc;

	BoxMarkStruct* mBoxMarkStruct;	// �洢���ӱ��
	list<PlayerBase> PlayerList;	// �洢����б�

	Image* mGrayBackgroundImage;	// ��Ϸ��ɫ����ͼ
	Image* mBlackBackgroundImage;	// ��ɫ����ͼ

	Map mMap;						// �洢��ͼ���ݽṹ
	Image *mStoneImage;				// ʯͷ
	Image *mForestImage;			// ����
	Image *mIceImage;				// ����
	Image *mRiverImage[2];			// ����
	Image *mWallImage;				// ��ǽ

	// ��ʼ mActiveEnemyTankNumber + mRemainEnemyTankNumber = 20
	int mActiveEnemyTankNumber;		// ��ǰ��ĵл�����, �����ʾ 6 ��
	int mRemainEnemyTankNumber;		// ����δ��ʾ�ĵл�����, ��ʼֵ=20, ����һ�ܵл���ֵ��һ

	Image* mEnemyTankIcoImage;		// �л�̹��ͼ��

	Image* mFlagImage;				// ����
	Image* mBlackNumberImage;		// 0123456789 ��ǰ�ؿ���
};