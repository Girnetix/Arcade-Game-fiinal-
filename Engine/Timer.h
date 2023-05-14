#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>
#include <list>
#include <string>
#include <functional>
#include "Memory.h"

class CTimerValue
{
	int64_t value = 0;
public:
	CTimerValue();
	CTimerValue(double seconds);
	CTimerValue(int64_t value);
	void Clear();

	CTimerValue& operator=(const CTimerValue& other);

	CTimerValue& operator+=(const CTimerValue& other);
	CTimerValue& operator+(const CTimerValue& other);

	CTimerValue& operator-=(const CTimerValue& other);
	CTimerValue& operator-(const CTimerValue& other);

	bool operator >(const CTimerValue& other);
	bool operator <(const CTimerValue& other);

	bool operator >=(const CTimerValue& other);
	bool operator <=(const CTimerValue& other);

	bool operator ==(const CTimerValue& other);

	//�������� ����� � �������� ��� � ������������� (������������ ������ ��� ��������� �����������)
	double GetSeconds();
	double GetMilliseconds();

	int64_t Count();
};

class Timer
{
public:
	enum class TimerHandleState { Running, Sleeping, Paused, Finished };
public:
	Timer();
	~Timer();
	void ResetTimer();
	void UpdateTimer();															//���������� �������

	bool PauseTimerHandle(unsigned int id);										//������������� ������
	bool UnpauseTimerHandle(unsigned int id);									//����������� ������
	bool SleepTimerHandle(unsigned int id, double seconds);						//������� ������
	bool StopTimerHandle(unsigned int id);										//������ ��������� ������� (��� ��������� �������)

	double TimeLeft(unsigned int id);											//���������� ����� ������ ������� (� �������)
	double GetDeltaTimeSec();													//���������� ������ ������� � ��������
	double GetDeltaTimeMSec();													//���������� ������ ������� � �������������
	double GetWorkingTime();													//���������� ����� ����� ������ ������(��� ������)

	uint32_t SetTimer(double seconds, double minutes, double hours, std::function<void()> lamdaFunction, bool firstDelay = true, int countOfRepeat = -1);
	uint32_t SetTimer(double seconds, std::function<void()> lamdaFunction, bool firstDelay = true, int countOfRepeat = -1);
	uint32_t SetTimer(int milliseconds, std::function<void()> lamdaFunction, bool firstDelay = true, int countOfRepeat = -1);
	CTimerValue GetHighPrecisionTime();											//���������� ���-�� ������ ����������
	uint64_t GetFrequency();
private:
	class TimerHandle
	{
	public:
		TimerHandleState TimerState = TimerHandleState::Running;				//��������� ����������� �������
		std::function<void()> function;											//������� ������� ��� ��������� ������
		double delay = 0.0, currentTime = 0.0;									//�������� � ������� ����� ����������� �������
		double sleepingTime = 0.0;												//����� ��� ����������� �������
		unsigned int id = 0;													//�� ����������� �������
		static unsigned int counter;											//������� �������� (���. ��� ������������� ����������� �������)
		int countOfRepeat = -1;													//���-�� ������������ �������(-1 -- ����������� ���-��)
	public:
		TimerHandle(double time, std::function<void()> lambda, bool firstDelay, int countOfRepeat);
	};
private:
	double workingTime = 0.0;													//����� ����� ������ ������ (��� ������)
	uint64_t timeBefore = 0, timeNow = 0, frequency = 0;						//��������� ����� � ������� (����� �������� � �������)
	double deltaTime = 0.0;														//����� ����� ����� ���������� ������� (� ��������)
	double CPUSpeed = 0.0;														//�������� ���������� (� ��)
	std::list<SmartPointer<TimerHandle>> timerList;								//������ ���������� �� ��� ����������� ��������
private:
	TimerHandle* GetTimerHandle(unsigned int id);								//�������� ���������� ������� �� ��
};


//����� ��� ��������� ������� ���������� ��������� �������
class Timing
{
public:
	Timing(std::function<void()> function, const std::wstring& str);
	~Timing();
private:
	std::wstring description;
	CTimerValue tp1, tp2;
};

extern SmartPointer<Timer> pTimer;

#endif
