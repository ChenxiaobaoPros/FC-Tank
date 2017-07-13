
#include "stdafx.h"
#include "struct.h"
#include "SelectPanel.h"
#include "GameControl.h"

#define ONE_PLAYER	1		// �����޸ĸ�ֵ!! �Ѿ����������±�!!
#define TWO_PLAYER	2

/***
* TankClass.PlayerBase ��
- �ö����� GameControl ��ʵ����,�洢

* TankClass.EnemyTank ��

* SelectPanel ��
- ������ʾѡ�����

* GameConrol ��
- ���� main ���ݽ���������, �洢 PlayerBase ��������
- �� Graphics* ��-> PlayerBase ��������л���

* ������ main()
- ���� SelectPanel ����, ��ʾ���ѡ�����
- ���� Gamecontrol ����, �������ѡ����
***/

//BoxMarkStruct Q_boxmark;		// ȫ�ֱ��� Q_ ��ͷ

IMAGE temp_img(600, 600);

//
void main()
{
	srand((unsigned)time(0));

	// ��ɫ����
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOWCONSOLE);
	BeginBatchDraw();

	// ���� image ����
	IMAGE canvas_img(CANVAS_WIDTH, CANVAS_HEIGHT);

	// ��ȡ graphics ��ͼ����
	HDC des_hdc = GetImageHDC();
	HDC canvas_hdc = GetImageHDC(&canvas_img);

	SelectPanel* selecter = new SelectPanel(des_hdc, canvas_hdc);	// ��ʾ��ҹ���ѡ�����
	EnumSelectResult result = selecter->ShowSelectPanel();					// ��ȡ���ѡ����
	GameControl* control = new GameControl(des_hdc, canvas_hdc/*, &Q_boxmark*/);
	switch (result)
	{
	case OnePlayer:
		control->AddPlayer(ONE_PLAYER);		// һ�����
		break;
	case TwoPlayer:
		control->AddPlayer(TWO_PLAYER);		// �������
		break;
	case Custom:					// ����Զ����ͼ
		break;
	default:
		return;
	}

	control->LoadMap();
	closegraph();
}