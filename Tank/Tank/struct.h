#pragma once

#define TOTAL_ENEMY_NUMBER	20

struct BulletStruct
{
	int x, y;					// �ӵ�����, ���ݲ�ͬ����ָ����ͬ��λ�ô��� x,y, ָ�� x=SHOOTABLE_X ��ʱ����Է����ӵ�
	int dir;					// �ӵ�����
	int speed[4];				// �ӵ��ٶ�, ����̹�˼�����費ͬ�ٶ�ϵ��. ÿ���ƶ����ܳ���4�����ص�!! ��Ȼ���Խ 4*4 �ĸ���!!���bug

	/* �����һ��ел�,���ø�ֵΪ�л���id, GameControl ѭ���ڼ���ֵ, Ȼ��ɾ���õл�
	* ����ǵл��������, ���ø�ֵΪ���id, ��Ϸѭ�����ټ���ֵ�����������*/
	int mKillId;				// ��¼�ӵ����еĵл�/��� id

	static IMAGE mBulletImage[4];		// ͼƬ
	static int mBulletSize[4][2];		// {{4,3},{3,4},{4,3},{3,4}} �ߴ�: ����-3*4 / ����-4*3
	static int devto_tank[4][2];		// �涨�ӵ������������tank���ĵ��ƫ����
	static int devto_head[4][2];		// �涨�ӵ�ͼƬ���Ͻ�����ڵ�ͷ�����ƫ����;���·���ͷ�����ڵ�ͷ���ұ�;�������ڵ�ͷ�������Ǹ���
	static int bomb_center_dev[4][2];	// ��ը����������ӵ�ͷ��ƫ����
};

struct BombStruct
{
	static IMAGE mBombImage[3];				// �ӵ���ըͼ
	int mBombX, mBombY;						// ��ը����������
	bool canBomb;							// �Ƿ�ʼ��ʾ��ըͼƬ flag
	int counter;						// ȡģ������, ���ٴ�ѭ������һ��ͼƬ
};