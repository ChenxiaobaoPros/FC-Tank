
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "graphics.h"
#include <list>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <Mmsystem.h>
#pragma comment ( lib, "Winmm.lib" )
using namespace std;

// GDI+ 

#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#include <comdef.h>
#include "./gdi+/Includes/GdiPlus.h"
#include "./gdi+/Includes/GdiPlusInit.h"
using namespace Gdiplus;
#pragma comment ( lib, "./gdi+/Lib/GdiPlus.lib" )
#endif

#define WINDOW_WIDTH 256		// ���ڴ�С������������޸ģ�
#define WINDOW_HEIGHT 224

#define CANVAS_WIDTH 256		// ���� image ��С������ı䣬���һ�ͼ���� image �ϲ�����Ȼ��һ���Ի��Ƶ�������
#define CANVAS_HEIGHT 224
#define CENTER_WIDTH 208		// �м��ɫ��Ϸ����
#define CENTER_HEIGHT 208

#define CENTER_X 16		// ��ɫ��Ϸ����������Ͻǵ�����
#define CENTER_Y 9

#define _WALL		'1'
#define _FOREST		'2'
#define _ICE		'3'
#define _RIVER		'4'
#define _STONE		'5'

#define BOX_SIZE 8			// 26*26 �ĸ���

#define ENEMY_TANK_ICO_SIZE		7		// �л�ͼ���С
#define PLAYER_TANK_ICO_X		7		// ���ͼ���С
#define PLAYER_TANK_ICO_Y		8
#define PLAYER_12_ICO_X			15		// 1P\2P ͼ���С
#define PLAYER_12_ICO_Y			8
#define FLAG_ICO_SIZE			17		// ���Ӵ�С
#define PLAYER_LIFE_NUMBER_SIZE 7		// ����������ִ�С

#define DIR_LEFT	0
#define DIR_UP		1
#define DIR_RIGHT	2
#define DIR_DOWN	3
