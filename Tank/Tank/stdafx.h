
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "./easyx/graphics.h"
#include <list>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <Mmsystem.h>
#pragma comment ( lib, "Winmm.lib" )
using namespace std;

// GDI
#include <wingdi.h>
#pragma comment (lib, "msimg32.lib")

// GDI+ 
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#include <comdef.h>
#include "./gdi+/Includes/GdiPlus.h"
#include "./gdi+/Includes/GdiPlusInit.h"
using namespace Gdiplus;
#pragma comment ( lib, "./gdi+/Lib/GdiPlus.lib" )
#endif

#define WINDOW_WIDTH	512		// ���ڴ�С������������޸ģ�
#define WINDOW_HEIGHT	448
#define CANVAS_WIDTH	256		// ���� image ��С������ı䣬���һ�ͼ���� image �ϲ�����Ȼ��һ���Ի��Ƶ�������
#define CANVAS_HEIGHT	224
#define CENTER_WIDTH	208		// �м��ɫ��Ϸ����
#define CENTER_HEIGHT	208
#define CENTER_X		16		// ��ɫ��Ϸ����������Ͻǵ�����
#define CENTER_Y		9

// ���, ���� markxx[26][26]
#define _CLEAR		-1		// ����ϰ��ﱻ�����ĸ���, ����ɨ��ʱ�� 0 ���ֿ�, Ȼ����ƺ�ɫͼƬ, ����� 0 Ҳ���ж������
#define _EMPTY		0		// �յ�
#define _FOREST		1		// ɭ��
#define _ICE		2		// ��
							// ------ ���� < 3 ̹�˿��Դ���
#define _WALL		3		// ǽ
#define _RIVER		4		// ����
#define _STONE		5		// ʯͷ

#define BOX_SIZE					8		// 26*26 �ĸ���
#define SMALL_BOX_SIZE				4		// 52*52 ���Ӵ�С
#define BLACK_NUMBER_SIZE			7		// ��ɫ���ִ�С

#define ENEMY_TANK_ICO_SIZE			7		// �л�ͼ���С
#define PLAYER_TANK_ICO_SIZE_X		7		// ���ͼ���С
#define PLAYER_TANK_ICO_SIZE_Y		8
#define PLAYER_12_ICO_SIZE_X		14		// 1P\2P ͼ���С
#define PLAYER_12_ICO_SIZE_Y		7
#define FLAG_ICO_SIZE_X				16		// ���Ӵ�С
#define FLAG_ICO_SIZE_Y				15
#define PLAYER_LIFE_NUMBER_SIZE		7		// ����������ִ�С

#define DIR_LEFT	0
#define DIR_UP		1
#define DIR_RIGHT	2
#define DIR_DOWN	3

// 4*4 �� 8*8 ���ӱ��, ���ϵл����� id�ٱ��
#define ENEMY_SIGN	1000

#define PLAYER_SIGN	100


#define SHOOTABLE_X		-100	// �涨�ӵ����� x = -100 �ӵ����Է���


/* ��Ӧ��ֵ
* 	#defien _EMPTY		0
#define _WALL		3
#define _FOREST		1
#define _ICE		2
#define _RIVER		4
#define _STONE		5
-------------------------------
* ��Ӧ�����ϵ
box[0][0-25] : ��һ��( y=0; x=[0-25] )
box[1][0-25] : �ڶ���( y=1; x=[0-25] )
...
box[y/BOX_SIZE][x/BOX_SIZE]
-------------------------------
*/
struct BoxMarkStruct
{
	int box_8[26][26];			// 8*8 ���ӵı��, ̹���ƶ�,�����øø��Ӽ��
	int box_4[52][52];			// 4*4 ���ӵı��, ǽ�������øñ�Ǽ��
};
