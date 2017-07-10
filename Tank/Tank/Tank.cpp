
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

BoxMarkStruct Q_boxmark;		// ȫ�ֱ��� Q_ ��ͷ

								////// xy[52][52]
int xy[60][60];											// ʹ��˫�ر�ǣ�һ�ر��̹�ˡ�̹���ӵ����ϰ�����ѡ���ʯ
int _xy[60][60];										// һ�ر��������������ɭ�֡�����(�����ӵ�Ҫ����������Ҫ����һ�ر��)

														/**** ������������ *****/

void Init();			// ��ʼ��̹�ˡ��ӵ�
void Sign(int, int, int, bool _dou_xy = false, bool Twin = false);			// ͬһλ��ʹ�����ֱ��ֵ
void SignBult(int, int);			// ����ӵ���ռ����
bool Check(int, bool Bult = false);			// ���̹�˻��ӵ��ƶ�������
bool Checkposion(int, int);			// ����λ���ӵ��Ƿ�Խ��
void Load();			// ����̹����ͼ
void LoadMap(int);			// �����ϰ���ͼ	
void Enemy();			// �л��ƶ� + �����ӵ�
void Control(DWORD&);			// ��������
void MoveMain(int);			// �����ƶ�
void Kill(int, bool killTurt = false);			// �������ел�
bool Burst(int, bool check = false);			// ����ӵ��ɷ�ը������ը
void ShowNest();			// ��ʾ����
void Hide(int);			// ����̹��
void HideTure(int, bool ture = false);			// �ӵ���Խɭ�ֵ��޲�����
void NextLevel();			// ����Ƿ���Խ�����һ��
void ChangeCol(int);			// ���������иı�������ɫ
								//void Bastion	( int														);			// �ϳ��ְ�������

IMAGE temp_img(600, 600);
//
void main()
{
	// gdi+ init
	GdiplusStartupInput    m_Gdistart;
	ULONG_PTR    m_GdiplusToken;
	GdiplusStartup(&m_GdiplusToken, &m_Gdistart, NULL);
	/////////////////////////////////////////////////////
	srand((unsigned)time(0));

	// ��ɫ����
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOWCONSOLE);
	//setbkcolor(RGB(119,119,119));
	//cleardevice();
	BeginBatchDraw();

	// ���� image ����
	IMAGE canvas_img(CANVAS_WIDTH, CANVAS_HEIGHT);

	// ��ȡ graphics ��ͼ����
	HDC des_hdc = GetImageHDC();
	HDC canvas_hdc = GetImageHDC(&canvas_img);
	Graphics* graphics = Graphics::FromHDC(canvas_hdc);

	SelectPanel* selecter = new SelectPanel(graphics, des_hdc, canvas_hdc);	// ��ʾ��ҹ���ѡ�����
	EnumSelectResult result = selecter->ShowSelectPanel();					// ��ȡ���ѡ����
	GameControl* control = new GameControl(graphics, des_hdc, canvas_hdc, &Q_boxmark);
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
	
	
	
	GdiplusShutdown(m_GdiplusToken);		// �ͷ�gdi+��Դ
	closegraph();
}

// �±��뷽���Ӧ����̹�ˡ�����
void Load()
{
	int i = 0;
	double rora[4] = { 0, 3.14, 1.57, 4.71 };

	/**** ̹�� **/

	for (int n = 0; n <= NUM; n++)
		for (int _r = 0; _r <= 3; _r++)
		{
			if (n == NUM)	// ����
			{
				loadimage(&Tank[n].Bimg[_r], _T("./res/photo/b00.jpg"), 10, 20);
				loadimage(&Tank[n].Timg[_r], _T("./res/photo/master4.jpg"), 40, 40);
			}
			else			// �л�
			{
				loadimage(&Tank[n].Bimg[_r], _T("./res/photo/b10.jpg"), 10, 20);
				loadimage(&Tank[n].Timg[_r], _T("./res/photo/enemy.jpg"), 40, 40);
			}

			// ��ת�洢�ĸ�����̹��
			rotateimage(&Tank[n].Timg[_r], &Tank[n].Timg[_r], rora[_r]);
			rotateimage(&Tank[n].Bimg[_r], &Tank[n].Bimg[_r], rora[_r], BLACK, true);
		}

	/**** ���� **/

	for (i = 0; i <= 19; i++)
	{
		loadimage(&Nest.Nimg[i], _T("./res/photo/nest.jpg"), 40, 40);
		rotateimage(&Nest.Nimg[i], &Nest.Nimg[i], 3.14 * i / 10);
	}
}


// ��ʼ�����ӡ��������л����л���ʱ���˶�ʱ���Ӫ
void Init()
{
	/**** ��ǰ������Ϣ ***/

	Level.tot_num = LEVEL::base_tot_num;
	Level.cur_num = LEVEL::base_cur_num;
	Level.next = Level.cur_num - 1;

	/**** ���� ***/

	Tank[NUM].X = 28;
	Tank[NUM].Y = 28;
	Tank[NUM].DIR = UP;
	Tank[NUM].kind = MASTER;			// ��־Ϊ����
	Tank[NUM].shoot = false;
	Tank[NUM].live = true;			// ������ʱ�ò��ϸ�ֵ
	Tank[NUM].appear = true;
	TANK::HP = 4;
	Sign(Tank[NUM].X, Tank[NUM].Y, NUM + 1);

	/**** �����ӵ� ***/

	Tank[NUM].b_x = Tank[NUM].X + TANK::T_Bult_of_Cent[Tank[NUM].DIR][0];
	Tank[NUM].b_y = Tank[NUM].Y + TANK::T_Bult_of_Cent[Tank[NUM].DIR][1];
	Tank[NUM].b_r = Tank[NUM].DIR;
	Tank[NUM].b_dt = 25;
	Tank[NUM].b_t1 = timeGetTime();

	/**** �л� ***/
	int i = 0;
	for (i = 0; i <= NUM - 1; i++)
	{
		Tank[i].X = (rand() % 3) * 28;
		Tank[i].Y = 0;
		Tank[i].DIR = rand() % 4;
		Tank[i].kind = ENEMY;
		Tank[i].shoot = true;
		Tank[i].T1 = timeGetTime();
		Tank[i].DRCT = rand() % 20 + 115;		// �л��ƶ�ʱ����
		Tank[i].live = true;
		Tank[i].step = 0;						// һ�����������

		if (i < Level.cur_num)					// ��ʾ��ǰ��ÿ���л�
		{
			Sign(Tank[i].X, Tank[i].Y, i + 1);
			Tank[i].appear = true;
		}
		else
			Tank[i].appear = false;
	}

	/**** �л��ӵ� ***/

	for (i = 0; i <= NUM - 1; i++)
	{
		Tank[i].b_x = Tank[i].X + TANK::T_Bult_of_Cent[Tank[i].DIR][0];
		Tank[i].b_y = Tank[i].Y + TANK::T_Bult_of_Cent[Tank[i].DIR][1];
		Tank[i].b_r = Tank[i].DIR;
		Tank[i].b_dt = Tank[i].DRCT / 4;
		Tank[i].b_t1 = timeGetTime();
		SignBult(i, 1000 + i);

		Tank[i].Ft1 = timeGetTime();
		Tank[i].Freq = rand() % 1100 + 1000;
	}

	/**** ���� ***/

	Nest.Nx = 28;
	Nest.Ny = 56;
	Nest.Nt1 = timeGetTime();
	Nest.Ndt = 50;
	Nest.Np = 0;

	for (int _x = 28; _x <= 31; _x++)
		for (int _y = 56; _y <= 59; _y++)
			xy[_x][_y] = CAMP;
}

// ɨ��л��ƶ����л� + �����ӵ��ƶ�
void Enemy()
{
	int i = 0;
	for (i = 0; i <= NUM - 1; i++)
	{
		Tank[i].T2 = timeGetTime();

		if (Tank[i].T2 - Tank[i].T1 > Tank[i].DRCT && Tank[i].live == true/* && Tank[i].besend == false*/ && Tank[i].appear == true)
		{
			/*********/

			putimage(Tank[i].X * LENGTH, Tank[i].Y * LENGTH, &Tank[i].Timg[Tank[i].DIR], SRCINVERT);
			Hide(i);
			Sign(Tank[i].X, Tank[i].Y, 0);

			/*********/

			Tank[i].step++;
			if (Tank[i].step % 36 == 0)
				Tank[i].DIR = rand() % 4;

			/*********/

			if (Check(i))
			{
				Tank[i].X += TANK::DRCXY[Tank[i].DIR][0];
				Tank[i].Y += TANK::DRCXY[Tank[i].DIR][1];
			}
			else
				Tank[i].DIR = rand() % 4;

			putimage(Tank[i].X * LENGTH, Tank[i].Y * LENGTH, &Tank[i].Timg[Tank[i].DIR], SRCINVERT);
			Sign(Tank[i].X, Tank[i].Y, i + 1);

			Hide(i);

			/*********/

			Tank[i].T1 = Tank[i].T2;
		}
	}

	for (i = 0; i <= NUM; i++)	// ע�⣡������������!!! �����ų��������з���!!!
	{
		Tank[i].b_t2 = timeGetTime();

		/*********************/

		if (i != NUM)
		{
			Tank[i].Ft2 = timeGetTime();
			if (Tank[i].Ft2 - Tank[i].Ft1 > Tank[i].Freq && Tank[i].shoot == false && Tank[i].live == true/* && Tank[i].besend == false */)
			{
				Tank[i].shoot = true;
				Tank[i].Ft1 = Tank[i].Ft2;

				Tank[i].b_x = Tank[i].X + TANK::T_Bult_of_Cent[Tank[i].DIR][0];
				Tank[i].b_y = Tank[i].Y + TANK::T_Bult_of_Cent[Tank[i].DIR][1];
				Tank[i].b_r = Tank[i].DIR;

				double ca_x = (Tank[i].b_x + TANK::Cent_Bult_of_LeUp[Tank[i].b_r][0]);
				double ca_y = (Tank[i].b_y + TANK::Cent_Bult_of_LeUp[Tank[i].b_r][1]);

				putimage((int)(ca_x * LENGTH), (int)(ca_y * LENGTH), &Tank[i].Bimg[Tank[i].b_r], SRCINVERT);
			}
		}

		/*********************/

		if (Tank[i].b_t2 - Tank[i].b_t1 > Tank[i].b_dt && Tank[i].shoot == true && Tank[i].live == true && Tank[i].appear == true)
		{
			/**********/

			double ca_x = Tank[i].b_x + TANK::Cent_Bult_of_LeUp[Tank[i].b_r][0];
			double ca_y = Tank[i].b_y + TANK::Cent_Bult_of_LeUp[Tank[i].b_r][1];

			putimage((int)(ca_x * LENGTH), (int)(ca_y * LENGTH), &Tank[i].Bimg[Tank[i].b_r], SRCINVERT);
			HideTure(i);
			SignBult(i, 0);

			/**********/

			if (Check(i, true))
			{
				Tank[i].b_x += TANK::DRCXY[Tank[i].b_r][0];
				Tank[i].b_y += TANK::DRCXY[Tank[i].b_r][1];

				ca_x = Tank[i].b_x + TANK::Cent_Bult_of_LeUp[Tank[i].b_r][0];
				ca_y = Tank[i].b_y + TANK::Cent_Bult_of_LeUp[Tank[i].b_r][1];

				putimage((int)(ca_x * LENGTH), (int)(ca_y * LENGTH), &Tank[i].Bimg[Tank[i].b_r], SRCINVERT);
				SignBult(i, 1000 + i);
			}

			/**********/

			Tank[i].b_t1 = Tank[i].b_t2;
		}

		/*********************/
	}
}

// ���ر��
void Sign(int x, int y, int value, bool _dou_xy, bool Twin)
{
	for (int a = x; a <= x + 3; a++)
	{
		for (int b = y; b <= y + 3; b++)
		{
			if (_dou_xy)
				_xy[a][b] = value;
			/*
			if ( Twin )
			Twinkle.xy[a][b] = value;
			*/
			if (Twin == false && _dou_xy == false)
				xy[a][b] = value;
		}
	}
}

// ����ӵ����ڵ� 2 * 2 �� 10 ��С����
void SignBult(int b_id, int value)
{
	for (int a = Tank[b_id].b_x - 1; a <= Tank[b_id].b_x; a++)
		for (int b = Tank[b_id].b_y - 1; b <= Tank[b_id].b_y; b++)
			xy[a][b] = value;
}

// ���̹���ƶ������ԡ��ӵ��ƶ������(����BLOCK�����磬�����ӵ����������ел�������ʯͷ�����д�Ӫ����������)
// ���Ӽ���������ӵ� Turt
bool Check(int id, bool Bult)
{
	int nextx, nexty;
	/*******/

	if (Bult == false)
	{
		nextx = Tank[id].X + TANK::DRCXY[Tank[id].DIR][0];
		nexty = Tank[id].Y + TANK::DRCXY[Tank[id].DIR][1];

		if (nextx < 0 || nextx > 56 || nexty < 0 || nexty > 56)
			return false;

		for (int x = nextx; x <= nextx + 3; x++)
		{
			for (int y = nexty; y <= nexty + 3; y++)
			{
				if (xy[x][y] != 0 && xy[x][y] != (id + 1) && xy[x][y] < 1000)
					return false;
				if (_xy[x][y] == RIVER)
					return false;
			}
		}
	}

	/*******/

	if (Bult == true)
	{
		nextx = Tank[id].b_x + TANK::DRCXY[Tank[id].b_r][0];
		nexty = Tank[id].b_y + TANK::DRCXY[Tank[id].b_r][1];

		// ע�⣺�����޶���Χ����֤����Գ����߽��δ֪��������ж�!!!
		if (Burst(id, true) && nextx >= 1 && nextx <= 59 && nexty >= 1 && nexty <= 59)
		{
			Burst(id);
			Tank[id].shoot = false;
			return false;
		}

		for (int _x = nextx - 1; _x <= nextx; _x++)
			for (int _y = nexty - 1; _y <= nexty; _y++)
			{
				if (_x < 0 || _x > 59 || _y < 0 || _y > 59)
				{
					Tank[id].shoot = false;
					return false;
				}

				if (xy[_x][_y] >= 1000 && xy[_x][_y] <= 1000 + NUM && xy[_x][_y] != id + 1000)
				{
					int n = xy[_x][_y] - 1000;
					double x1 = Tank[n].b_x + TANK::Cent_Bult_of_LeUp[Tank[n].b_r][0];
					double y1 = Tank[n].b_y + TANK::Cent_Bult_of_LeUp[Tank[n].b_r][1];

					putimage((int)(x1 * LENGTH), (int)(y1 * LENGTH), &Tank[n].Bimg[Tank[n].b_r], SRCINVERT);
					SignBult(n, 0);

					Tank[n].shoot = false;
					Tank[id].shoot = false;
					return false;
				}

				if (xy[_x][_y] >= 1 && xy[_x][_y] <= NUM && id == NUM/* && Tank[xy[_x][_y] - 1].besend == false */)
				{
					int kn = xy[_x][_y] - 1;
					mciSendString(_T("close bomb"), 0, 0, 0);
					mciSendString(_T("open ./res/music/destroy.wav alias bomb"), 0, 0, 0);
					mciSendString(_T("play  bomb"), 0, 0, 0);
					Kill(kn);

					Tank[id].shoot = false;
					return false;
				}

				if (xy[_x][_y] == STONE)
				{
					Tank[id].shoot = false;
					return false;
				}

				if (xy[_x][_y] == CAMP)
				{
					PlaySound(_T("./res/music/bomb.wav"), 0, SND_ASYNC);
					HWND hwnd = GetHWnd();
					if (id != NUM)
						MessageBox(hwnd, _T("ʺ�����ѱ�ը�ˣ�"), _T("GameOver"), MB_OK);
					else
						MessageBox(hwnd, _T("��ˣ���������!"), _T("GameOver"), MB_OK);
					game.replay = true;
					return false;
				}

				if (xy[_x][_y] == NUM + 1 && id != NUM /*&& Tank[NUM].besend == false*/)
				{
					PlaySound(_T("./res/music/bomb.wav"), 0, SND_ASYNC);
					TANK::HP--;
					ChangeCol(TANK::HP);

					if (TANK::HP == 0)
					{
						HWND hwnd = GetHWnd();
						MessageBox(hwnd, _T("�ף��㱻ը����!"), _T("GameOver"), MB_OK);
						game.replay = true;
					}
					Tank[id].shoot = false;
					return false;
				}
			}
	}

	/*******/
	return true;
}

// ���ӵ�����λ�ü���ӵ��Ƿ�Խ��
bool Checkposion(int x, int y)
{
	for (int a = x - 1; a <= x; a++)
		for (int b = y - 1; b <= y; b++)
		{
			if (a < 0 || a > 59 || b < 0 || b > 59)
				return false;

			if (xy[a][b] != 0)
				return false;
		}
	return true;
}

// �����ƶ��������ӵ�
void Control(DWORD& click_t1)
{
	DWORD click_t2 = timeGetTime(), click_speed = 70;

	if (click_t2 - click_t1 > click_speed /*&& Tank[NUM].besend == false */)
	{
		if (GetAsyncKeyState('W') & 0x8000)		MoveMain(UP);
		else if (GetAsyncKeyState('S') & 0x8000)		MoveMain(DOWN);
		else if (GetAsyncKeyState('A') & 0x8000)		MoveMain(LEFT);
		else if (GetAsyncKeyState('D') & 0x8000)		MoveMain(RIGHT);

		click_t1 = click_t2;

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)		game.start = false;
		if (GetAsyncKeyState('J') & 0x8000)
		{
			if (Tank[NUM].shoot == false)
			{
				PlaySound(_T("./res/music/shoot2.wav"), NULL, SND_ASYNC);

				Tank[NUM].b_x = Tank[NUM].X + TANK::T_Bult_of_Cent[Tank[NUM].DIR][0];
				Tank[NUM].b_y = Tank[NUM].Y + TANK::T_Bult_of_Cent[Tank[NUM].DIR][1];
				Tank[NUM].b_r = Tank[NUM].DIR;
				Tank[NUM].shoot = true;

				double mx = Tank[NUM].b_x + TANK::Cent_Bult_of_LeUp[Tank[NUM].DIR][0];
				double my = Tank[NUM].b_y + TANK::Cent_Bult_of_LeUp[Tank[NUM].DIR][1];

				putimage((int)(mx * LENGTH), (int)(my * LENGTH), &Tank[NUM].Bimg[Tank[NUM].b_r], SRCINVERT);
			}
		}
	}
}

// �ƶ�����������������ԭ������һ��ֻת��
void MoveMain(int _Dir)
{
	putimage(Tank[NUM].X * LENGTH, Tank[NUM].Y * LENGTH, &Tank[NUM].Timg[Tank[NUM].DIR], SRCINVERT);
	Hide(NUM);
	Sign(Tank[NUM].X, Tank[NUM].Y, 0);

	int temp = Tank[NUM].DIR;

	if (_Dir != temp)
		Tank[NUM].DIR = _Dir;

	else if (Check(NUM))
	{
		Tank[NUM].X += TANK::DRCXY[Tank[NUM].DIR][0];
		Tank[NUM].Y += TANK::DRCXY[Tank[NUM].DIR][1];
	}

	putimage(Tank[NUM].X * LENGTH, Tank[NUM].Y * LENGTH, &Tank[NUM].Timg[Tank[NUM].DIR], SRCINVERT);
	Hide(NUM);
	Sign(Tank[NUM].X, Tank[NUM].Y, NUM + 1);
}

// ���� n �ŵл��������л������ӵ�
// �������� n ������
void Kill(int n, bool killTurt)
{
	if (killTurt == false)
	{
		Tank[n].live = false;
		/**********/

		putimage(Tank[n].X * LENGTH, Tank[n].Y * LENGTH, &Tank[n].Timg[Tank[n].DIR], SRCINVERT);
		Hide(n);
		Sign(Tank[n].X, Tank[n].Y, 0);

		/**********/

		double k_x = Tank[n].b_x + TANK::Cent_Bult_of_LeUp[Tank[n].b_r][0];
		double k_y = Tank[n].b_y + TANK::Cent_Bult_of_LeUp[Tank[n].b_r][1];

		if (Tank[n].shoot == true)
		{
			putimage((int)(k_x * LENGTH), (int)(k_y * LENGTH), &Tank[n].Bimg[Tank[n].b_r], SRCINVERT);
			HideTure(n);
		}
		SignBult(n, 0);

		/**********/

		if (Level.next < Level.tot_num - 1)
		{
			Level.next++;
			Tank[Level.next].appear = true;

			putimage(Tank[Level.next].X * LENGTH, Tank[Level.next].Y * LENGTH, &Tank[Level.next].Timg[Tank[Level.next].DIR], SRCINVERT);
			k_x = Tank[Level.next].b_x + TANK::Cent_Bult_of_LeUp[Tank[Level.next].b_r][0];
			k_y = Tank[Level.next].b_y + TANK::Cent_Bult_of_LeUp[Tank[Level.next].b_r][1];
			putimage((int)(k_x * LENGTH), (int)(k_y * LENGTH), &Tank[Level.next].Bimg[Tank[Level.next].b_r], SRCINVERT);
		}
	}
}

// true Ϊ���ɷ�ը��false Ϊ��ը
bool Burst(int n, bool check)
{
	/**** �����ĸ�����ı�ը��Χ ****/

	int dir_x[4] = { -2, -2, -4, 1 };
	int dir_y[4] = { -4, 1, -2, -2 };
	int may, a, b;

	/**** ������ �� ��һ�еĲ������� ****/

	if (n == NUM /*&& Twinkle.effect[0] == true */)
		may = 2;
	else
	{
		dir_x[2] = -2;
		dir_y[0] = -2;
		may = 0;
	}

	/**** 0 �ŵ���Ч�������޼ӳ�Ч�� ***/

	if (!check)
	{
		for (int x = 0; x <= 3; x++)
		{
			for (int y = 0; y <= may; y++)
			{
				int _x[4] = { x, x, y, y };
				int _y[4] = { y, y, x, x };
				a = Tank[n].b_x + (_x[Tank[n].b_r] + dir_x[Tank[n].b_r]);
				b = Tank[n].b_y + (_y[Tank[n].b_r] + dir_y[Tank[n].b_r]);

				if (a >= 0 && a <= 59 && b >= 0 && b <= 59)
				{
					if (xy[a][b] == BLOCK)
					{
						putimage(a * LENGTH, b * LENGTH, &StaPic.block, SRCINVERT);
						xy[a][b] = 0;
					}
				}
			}
		}
	}

	/**** һ�ŵ���Ч����ը����ʯ **/
	/*
	if ( !check )
	{
	if ( n == NUM && Twinkle.effect[1] == true )
	{
	setfillstyle( BLACK );
	dir_x[2] = -2;
	dir_y[0] = -2;
	for ( int x = 0; x <= 3; x++ )
	{
	int _x[4] = { x, x, 0, 0 };
	int _y[4] = { 0, 0, x, x };
	a = Tank[n].b_x + ( _x[Tank[n].b_r] + dir_x[Tank[n].b_r] );
	b = Tank[n].b_y + ( _y[Tank[n].b_r] + dir_y[Tank[n].b_r] );

	if ( a >= 0 && a <= 59 && b >= 0 && b <= 59 )	// ��ֹԽ�籬ը!!!
	{
	if ( xy[a][b] == BLOCK )
	{
	putimage( a * LENGTH, b * LENGTH, &StaPic.block, SRCINVERT );
	xy[a][b] = 0;
	}

	if ( xy[a][b] == STONE )
	{
	putimage( a * LENGTH, b * LENGTH, &StaPic.stone, SRCINVERT );
	xy[a][b] = 0;
	}
	}
	}
	}
	}*/

	/**** ����Ƿ�����ש�� �� ʯͷ ****/

	if (check)
	{
		dir_x[2] = -2;
		dir_y[0] = -2;
		for (int x = 0; x <= 3; x++)
		{
			int _x[4] = { x, x, 0, 0 };
			int _y[4] = { 0, 0, x, x };

			a = Tank[n].b_x + (_x[Tank[n].b_r] + dir_x[Tank[n].b_r]);
			b = Tank[n].b_y + (_y[Tank[n].b_r] + dir_y[Tank[n].b_r]);

			if (a >= 0 && a <= 59 && b >= 0 && b <= 59)		// ע�⣺����Խ���ж�!!!
				if (xy[a][b] == BLOCK || xy[a][b] == STONE /*&& Twinkle.effect[1] == true */)
					return true;
		}
	}
	return false;
}

// ��ʾ��ת����
void ShowNest()
{
	Nest.Nt2 = timeGetTime();

	if (Nest.Nt2 - Nest.Nt1 > Nest.Ndt)
	{
		putimage(Nest.Nx * LENGTH, Nest.Ny * LENGTH, &Nest.Nimg[Nest.Np]);
		Nest.Np++;

		if (Nest.Np > 19)
			Nest.Np = 0;

		Nest.Nt1 = Nest.Nt2;
	}
}

void LoadMap(int id)
{
	/**** ̹�� **/

	int i = 0;
	for (i = 0; i <= NUM; i++)
	{
		if (Tank[i].appear == true)
			putimage(Tank[i].X * LENGTH, Tank[i].Y * LENGTH, &Tank[i].Timg[Tank[i].DIR], SRCINVERT);
	}

	/**** �л��ӵ� **/

	for (i = 0; i <= NUM - 1; i++)
	{
		if (Tank[i].appear == true)
		{
			double a = Tank[i].b_x + TANK::Cent_Bult_of_LeUp[Tank[i].b_r][0];
			double b = Tank[i].b_y + TANK::Cent_Bult_of_LeUp[Tank[i].b_r][1];

			putimage((int)(a * LENGTH), (int)(b * LENGTH), &Tank[i].Bimg[Tank[i].b_r], SRCINVERT);
		}
	}


	int n = 0, m = 0;
	IMAGE map1, map2;

	loadimage(&map1, _T("./res/photo/map.jpg"));
	SetWorkingImage(&map1);
	getimage(&map2, id * 60, 0, 60, 60);

	loadimage(&StaPic.block, _T("./res/photo/block.jpg"), 10, 10);
	loadimage(&StaPic.stone, _T("./res/photo/stone.jpg"), 10, 10);
	loadimage(&StaPic.forest, _T("./res/photo/forest.jpg"), 10, 10);
	loadimage(&StaPic.river, _T("./res/photo/river.jpg"), 10, 10);

	int save[60][60];
	SetWorkingImage(&map2);

	for (int x = 0; x <= 59; x++)
	{
		for (int y = 0; y <= 59; y++)
		{
			save[x][y] = getpixel(x, y);
		}
	}

	SetWorkingImage();
	SetWorkingImage(&temp_img);
	for (int x2 = 0; x2 <= 59; x2++)
	{
		for (int y2 = 0; y2 <= 59; y2++)
		{
			switch (save[x2][y2])
			{
			case 0xfcfcfc:		// �ϰ���
				xy[x2][y2] = BLOCK;
				putimage(x2 * LENGTH, y2 * LENGTH, &StaPic.block, SRCINVERT);
				break;

			case 0x00ff00:		// ��ʯ
				xy[x2][y2] = STONE;
				putimage(x2 * LENGTH, y2 * LENGTH, &StaPic.stone);
				break;

			case 0x0000ff:		// ɭ��
				_xy[x2][y2] = FOREST;
				putimage(x2 * LENGTH, y2 * LENGTH, &StaPic.forest);
				break;

			case 0xff0000:		// ����
				_xy[x2][y2] = RIVER;
				putimage(x2 * LENGTH, y2 * LENGTH, &StaPic.river);
				break;
			default: break;
			}
		}
	}
}

// ̹���߽�ɭ�ֵ��˶�������ʽ�е㲻һ��
void Hide(int n)
{
	for (int x = Tank[n].X; x <= Tank[n].X + 3; x++)
		for (int y = Tank[n].Y; y <= Tank[n].Y + 3; y++)
			if (_xy[x][y] == FOREST)
				putimage(x * LENGTH, y * LENGTH, &StaPic.forest);
}

// ̹���ӵ��������ӵ���ɭ������Ĳ������������ӵ�
void HideTure(int n, bool ture)
{
	if (!ture)
	{
		for (int x = Tank[n].b_x - 1; x <= Tank[n].b_x; x++)
			for (int y = Tank[n].b_y - 1; y <= Tank[n].b_y; y++)
				if (_xy[x][y] == FOREST)
					putimage(x * LENGTH, y * LENGTH, &StaPic.forest);
	}
}

// ����Ƿ���������̹�� �� ����
// �ʵ����ӵл������� �� ͬʱ���ֵ�����
void NextLevel()
{
	int sum = 0;
	for (int i = 0; i < Level.tot_num; i++)
		if (Tank[i].live == true)
			sum++;
	/*
	if ( Turret[0].life == true || Turret[1].life == true )
	sum++;
	*/
	if (sum == 0)
	{
		LEVEL::base_tot_num += 2;
		if (LEVEL::base_tot_num % 6 == 0)
			LEVEL::base_cur_num++;

		MessageBox(GetHWnd(), _T("��ϲ����"), _T("Next"), MB_OK);
		ChangeCol(4);
		game.level++;
		game.replay = true;
	}
}

// ����������ǰ����ֵ�ı���ɫ
void ChangeCol(int hp)
{
	double rora[4] = { 0, 3.14, 1.57, 4.71 };
	TCHAR c[50];

	if (hp == 0)
		hp = 4;
	_stprintf_s(c, _T("./res/photo/master%d.jpg"), hp);

	putimage(Tank[NUM].X * LENGTH, Tank[NUM].Y * LENGTH, &Tank[NUM].Timg[Tank[NUM].DIR], SRCINVERT);
	for (int r = 0; r < 4; r++)
	{
		loadimage(&Tank[NUM].Timg[r], c, 40, 40);
		rotateimage(&Tank[NUM].Timg[r], &Tank[NUM].Timg[r], rora[r]);
	}
	putimage(Tank[NUM].X * LENGTH, Tank[NUM].Y * LENGTH, &Tank[NUM].Timg[Tank[NUM].DIR], SRCINVERT);
	Hide(NUM);
}
/*
// 3 �ŵ���Ч�� (0 ������1 ����) �ϳ�
void Bastion( int f_t )
{
int sign[2] = { BLOCK, STONE };
IMAGE* p[2];

p[0] = &StaPic.block;
p[1] = &StaPic.stone;

for ( int i = 26; i <= 27; i++ )
{
for ( int j = 0; j <= 15; j++ )
{
int _x[16] = { 26, 27, 28, 29, 30, 31, 32, 33, i,  i,  i,  i,  i + 6, i + 6, i + 6, i + 6 };
int _y[16] = { i + 28, i + 28, i + 28, i + 28, i + 28, i + 28, i + 28, i + 28, 56, 57, 58, 59, 56, 57, 58, 59 };

int a = _x[j], b = _y[j];
xy[a][b] = sign[f_t];

setfillstyle( 0 );
bar( a * LENGTH, b * LENGTH, (a + 1) *LENGTH - 1, (b + 1) * LENGTH - 1 );
putimage( a * LENGTH, b * LENGTH, p[f_t], SRCINVERT );
}
}
}*/