#pragma once


class TimeClock
{
public:
	TimeClock();
	~TimeClock() {}

	/*�����˶�ʱ���*/
	void SetDrtTime(double);

	void Timer1();
	void Timer2();

	/*�Ƚ�������ʱʱ���, ���� true false ������ʱ���*/
	bool IsTimeOut();		// ʱ��

	LARGE_INTEGER litmp;		// ����ýṹ���һ������
	LONGLONG QPart1, QPart2;	// ���ڻ�ȡ�ṹ������� QuadPart ��Ա
	double dfFreq;				// ʱ��Ƶ��
	double drtTime;				// ����ʱ���, ���������˶��ٶ�
};