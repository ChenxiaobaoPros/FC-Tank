#pragma once


/*
* close ָ���ر�ĳ�������� ���� 
* name ���������ļ�����, ����չ��, ����·��,
* type_name ����
*/
//void _PlaySound(const TCHAR* close, const TCHAR* name, const TCHAR* type_name);

#define S_START 0
#define S_SHOOT	1
#define S_BIN	2
#define S_BK	3
#define S_GETPROP	4		// ��õ���
#define S_PROPOUT	5
#define S_ENEMY_BOMB	6		// �л���ը
#define S_CAMP_BOMB	7
#define S_SCOREPANEL_DI	8		// ������� ������
#define S_BOUNS1000		9
#define S_ADDLIFE	10
#define S_WIN	11
#define S_FAIL	12
#define S_PLAYER_BOMB	13
#define S_PLAYER_MOVE	14

class IrrklangSound
{
public:
	static void InitSounds();

	static void _PlaySound(int kind);

	static void PauseBk(bool);		// ��ʼ����ͣ��������
	static void PauseMove(bool);		// ���̹���ƶ�����

private:
	static ISoundEngine* engine;
	static ISound* bk_control;
	static ISound* player_move_control;

	static ISoundSource* start;		// ѡ����ҽ���
	static ISoundSource* shoot;		// ����ӵ�
	static ISoundSource* bin;		// ����ʯͷǽ��,�����Ǹ�̹��
	static ISoundSource* bk;		// ��Ϸ��������
	static ISoundSource* getprop;	// 
	static ISoundSource* propout;	// 
	static ISoundSource* enemy_bomb;	// 
	//static ISoundSource* camp_bomb;	// 
	static ISoundSource* scorepanel_di;	// ��������������
	static ISoundSource* bunds1000;	// 
	static ISoundSource* addlife;	// addlife
	//static ISoundSource* win;	// 
	static ISoundSource* fail;	// 
	static ISoundSource* player_bomb;	// player_bombplayer_move
	static ISoundSource* player_move;	// player_move
};