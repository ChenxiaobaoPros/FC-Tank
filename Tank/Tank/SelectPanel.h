#pragma once

// ָʾ���ѡ��
enum EnumSelectResult
{
	OnePlayer = 1,	// 1->1�����
	TwoPlayer,		// 2->2�����
	Custom,			// 3->�Զ����ͼ
	Error			// 4
};

class SelectPanel
{
public:
	SelectPanel( HDC des_hdc, HDC image_hdc);
	~SelectPanel();

	void Init();
	EnumSelectResult ShowSelectPanel();			// ��ʾ���ѡ�����, �������ѡ��0,1,2, -1����
private:
	//void ShowStage();

private:
	//Graphics* mGraphics;
	HDC mDes_hdc, mImage_hdc;

	// ��������,ѡ����\˫��\�Զ���\��Ϸ.
	//Image* mSelect_player_image;
	IMAGE mSelect_player_image;
	int mSelect_player_image_y;		// ͼƬ�ӵײ���������,��ʼֵ=672, ���ڵײ�

	// ��ʼѡ���1��2����ͼ
	byte mCounter : 1;				// �����л� mSelectTankImage �±�
	//Image* mSelectTankImage[2];		// ָʾ��̹��, �����̹�������ȡͼƬ
	IMAGE mSelectTankImage[2];
	POINT mSelectTankPoint[3];		// ����ѡ������
	int mSelectIndex;				// ѡ����: 0,1,2

	// ѡ����Һ���ʾ�ؿ���Ϣ
	//Image* mGrayBackgroundImage;	// ��Ϸ��ɫ����ͼ
	//Image* mCurrentStageImage;		// ѡ�������ʾ��ǰ�ؿ�
	IMAGE mGrayBackgroundImage;	// ��Ϸ��ɫ����ͼ
	//IMAGE mCurrentStageImage;		// ѡ�������ʾ��ǰ�ؿ�
	IMAGE mBlackNumberImage;		// ���� 1234567890 ͼƬ
};