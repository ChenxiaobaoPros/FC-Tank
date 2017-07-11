#pragma once
#include "TankClass.h"

#define STOP_SHOW_STAR	false		// �Ľ�����ʾ����
#define SHOWING_STAR	true		// ������ʾ�Ľ���

/************** �л�̹�� ************
* һ���л�ʵ����һ������
* �ṩ�л����(�Ƿ��ǵ���̹��), �л�����[0-4]
* Ĭ������ 0-0 �л�, ��0���ĵ���̹��
************************************/
class EnemyBase
{
public:
	EnemyBase(byte kind, int level, BoxMarkStruct*);	// kind[0-1]; level [0-4]
	~EnemyBase();
	bool ShowStar(const HDC& center_hdc, int& total );		// ��ʾ��˸�Ľ���, true-������ʾ, false-��ʾ���
	void TankMoving(const HDC& center_hdc);		// �л��ƶ�

private:
	void SignBox_4(int value);		// ��� 4*4 ��С�ĸ���Ϊ̹��;
	void UnSignBox_4();				// ȡ�����, ̹���ƶ�ǰ��Ҫȡ��, �ƶ����ٱ��
	bool CheckSignBox(int, int);	// ���ĳ�� box �Ƿ���Է���̹��, ������8*8 ���ӵ����ĵ�, ��̹�����������ͬ
	bool CheckMoveable();			// 
	void RejustDirPosition();		// ���¶�λ̹�˷���, ����̹��λ��, �����ڸ�����

private:
	byte mEnemyTankKind;		// �л����, ����̹�˺���̹ͨ������, [0-1]
	int mEnemyTankLevel;		// �л�̹��4������ [0-3]
	TankInfo* mEnemyTank;		// ָ��̹����ϸ��Ϣ
	BoxMarkStruct* bms;			// ָ����ӱ�ǽṹ, �� GameControl ���ݽ���

	int mTankX, mTankY;			// ̹������, ̹�˵����ĵ�
	byte mTankDir : 2;			// ̹�˷���

	IMAGE mStarImage[4];		// �Ľ���ͼƬ
	int mStarIndexDev ;			// �����ı仯��, -1, 1  -1��star��С���, 1 ��star�ɴ��С
	byte mStarIndex : 2;		// �Ľ����±������仯���� 0-1-2-3-2-1-0-1-2-3-...
	int mStarCounter;			// ����,���ٴα��һ��ͼ��
	int mTankOutAfterCounter;	// һ���������֮��, �Ľ��ǿ�ʼ��˸,̹�˳���
	bool mIsOuted;				// �Ľ���Сʱ��̹�˳���, ֹͣ�����Ľ�����˸ͼ
	//static bool isOuting;		// ��ǰ�Ƿ���̹�����ڳ���, �����,����̹�˲��ܳ���,�ȸ�̹�˳�����֮��ſ�ʼ mTankOutAfterCounter

	int mStep;					// ��ǰ�����ƶ��Ĳ���, һ����������������ϰ���任�������¼���;
	static int mDevXY[4][2];				// �ĸ������ƫ����
	int mSpeed[4];							// mSpeed * mDevXY �õ��˶��ٶ�, �±��Ӧ mPlayerTankLevel, ��ͬ�����ٶȲ�һ��

	bool mTankNumberReduce;		// ���Ľ��ǿ�ʼ, true-̹��������һ,Ȼ�����ֵ=false, ֻ��һ��
};