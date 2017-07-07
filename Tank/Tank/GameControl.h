#pragma once
#include "PlayerBase.h"

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
	GameControl(Graphics* grap, HDC des_hdc, HDC image_hdc, BoxMarkStruct* );
	~GameControl();
	void Init();
	void AddPlayer(int player_num);					// ������ҽ��������
	void LoadMap();									// ��ȡ .map ��ͼ�ļ�|�������Ƶ�ͼ
	bool StartGame();								// ���»�����Ϸ���ֶ���, ���� false ������Ϸ

private:
	void ShowEnemyTankIco();						// ����ʣ��л�������ʾ�л�ͼ������
	void SignBoxMark(int i, int j, int sign_val);	// ��Ǹ���, LoadMap() �ڵ���
	void RefreshRightPanel();						// ˢ���ұ���Ϣ�������
	void RefreshCenterPanel();						// ˢ���м���Ϸ���� 208 * 208
	int SignValueTrans(char);						// ����ͼ'0''1''2''3''4''5' �ı��ת���ɺ� _WALL, _ICE.. ֮���

public:
	static int mCurrentStage;						// [1-35] ��ǰ�ؿ�, SelectPanel ��ʹ��, ������ʹ��

private:
	// ���ڻ�ͼ�ӿ�
	Graphics* mGraphics;
	IMAGE mCenterImage;								// ���ڱ��� mCenter_hdc ������Ч��.
	HDC mDes_hdc, mImage_hdc, mCenter_hdc;			// ������hdc, 256*224 �� hdc, 208*208 �� hdc, mCenter_hdc ����Ϸ����,�ֿ�,��������,����Ҳ�����趨

	BoxMarkStruct* mBoxMarkStruct;	// �洢���ӱ��
	list<PlayerBase> PlayerList;	// �洢����б�
	list<PlayerBase>::iterator itor;// ���ڱ��� PlayerList

	IMAGE mGrayBackgroundImage;		// ��Ϸ��ɫ����ͼ
	IMAGE mBlackBackgroundImage;	// ��ɫ����ͼ

	Map mMap;						// �洢��ͼ���ݽṹ
	IMAGE mStoneImage;				// ʯͷ
	IMAGE mForestImage;				// ����
	IMAGE mIceImage;				// ����
	IMAGE mRiverImage[2];			// ����
	IMAGE mWallImage;				// ��ǽ
	IMAGE mCamp;					// ��Ӫ

	IMAGE mEnemyTankIcoImage;		// �л�̹��ͼ��
	IMAGE mFlagImage;				// ����
	IMAGE mBlackNumberImage;		// 0123456789 ��ǰ�ؿ���

	// ��ʼ mActiveEnemyTankNumber + mRemainEnemyTankNumber = 20
	int mActiveEnemyTankNumber;		// ��ǰ��ĵл�����, �����ʾ 6 ��
	int mRemainEnemyTankNumber;		// ����δ��ʾ�ĵл�����, ��ʼֵ=20, ����һ�ܵл���ֵ��һ
};