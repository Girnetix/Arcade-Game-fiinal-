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

	//получить время в секундах или в миллисекундах (использовать только для временных промежутков)
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
	void UpdateTimer();															//обновление таймера

	bool PauseTimerHandle(unsigned int id);										//приостановить таймер
	bool UnpauseTimerHandle(unsigned int id);									//возобновить таймер
	bool SleepTimerHandle(unsigned int id, double seconds);						//усыпить таймер
	bool StopTimerHandle(unsigned int id);										//полная остановка таймера (без выполения функции)

	double TimeLeft(unsigned int id);											//оставшееся время работы таймера (в секунах)
	double GetDeltaTimeSec();													//возвращает дельта времени в секундах
	double GetDeltaTimeMSec();													//возвращает дельта времени в миллисекундах
	double GetWorkingTime();													//возвращает общее время работы движка(для дебага)

	uint32_t SetTimer(double seconds, double minutes, double hours, std::function<void()> lamdaFunction, bool firstDelay = true, int countOfRepeat = -1);
	uint32_t SetTimer(double seconds, std::function<void()> lamdaFunction, bool firstDelay = true, int countOfRepeat = -1);
	uint32_t SetTimer(int milliseconds, std::function<void()> lamdaFunction, bool firstDelay = true, int countOfRepeat = -1);
	CTimerValue GetHighPrecisionTime();											//возвращает кол-во тактов процессора
	uint64_t GetFrequency();
private:
	class TimerHandle
	{
	public:
		TimerHandleState TimerState = TimerHandleState::Running;				//состояние дескриптора таймера
		std::function<void()> function;											//обертка функции для обратного вызова
		double delay = 0.0, currentTime = 0.0;									//задержка и текущее время дескриптора таймера
		double sleepingTime = 0.0;												//время сна дескриптора таймера
		unsigned int id = 0;													//ид дескриптора таймера
		static unsigned int counter;											//счётчик таймеров (исп. для идентификации дескриптора таймера)
		int countOfRepeat = -1;													//кол-во срабатываний таймера(-1 -- бесконечное кол-во)
	public:
		TimerHandle(double time, std::function<void()> lambda, bool firstDelay, int countOfRepeat);
	};
private:
	double workingTime = 0.0;													//общее время работы движка (для дебага)
	uint64_t timeBefore = 0, timeNow = 0, frequency = 0;						//временные точки и частота (число операций в секунду)
	double deltaTime = 0.0;														//время между двумя временными точками (в секундах)
	double CPUSpeed = 0.0;														//скорость процессора (в Гц)
	std::list<SmartPointer<TimerHandle>> timerList;								//список указателей на все дескрипторы таймеров
private:
	TimerHandle* GetTimerHandle(unsigned int id);								//получить дескриптор таймера по ид
};


//класс для измерения времени выполнения различных функций
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
