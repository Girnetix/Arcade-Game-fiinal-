#include "Timer.h"
#include "Console.h"

SmartPointer<Timer> pTimer = nullptr;

unsigned int Timer::TimerHandle::counter = 0;

Timer::Timer()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	uint64_t CPUBefore = 0, CPUAfter = 0;
	uint64_t timeAfter = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&timeNow);
	timeBefore = timeNow;
	timeAfter = timeNow + frequency / 4;
	CPUBefore = __rdtsc();
	do
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&timeNow);
	} while (timeNow < timeAfter);
	CPUAfter = __rdtsc();
	CPUSpeed = floor((double)((CPUAfter - CPUBefore) * frequency / (timeNow - timeBefore)) / 1000000.0) * 1000000.0;
	timeBefore = timeNow;
}

Timer::~Timer()
{
	timerList.clear();
}

void Timer::ResetTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&timeNow);
	timeBefore = timeNow;
}

void Timer::UpdateTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&timeNow);
	deltaTime = (double)(timeNow - timeBefore) / frequency;								//получаем время между вызовами этого метода (в секундах)
	workingTime += deltaTime;
	if (deltaTime > 0.3)
		deltaTime = 0.3;
	timeBefore = timeNow;
	for (auto iterator = timerList.begin(); iterator != timerList.end();)
	{
		TimerHandle* currentTimer = (*iterator).Get();
		switch (currentTimer->TimerState)
		{
		case TimerHandleState::Running:										//если таймер запущен
			currentTimer->currentTime += deltaTime;							//увеличиваем время работы таймера на время между вызовами метода
			if (currentTimer->currentTime >= currentTimer->delay)			//если время работы превышает время задержки, то...
			{
				currentTimer->function();									//вызываем функцию
				currentTimer->currentTime -= currentTimer->delay;			//уменьшаем время работы на нашу задержку
				if (currentTimer->countOfRepeat > 0)
					currentTimer->countOfRepeat--;							//уменьшаем кол-во срабатываний
				if (currentTimer->countOfRepeat == 0)
					currentTimer->TimerState = TimerHandleState::Finished;	//устанавливаем флаг, что таймер завершил свою работу
			}
			++iterator;
			break;
		case TimerHandleState::Sleeping:									//если таймер в режиме сна
			currentTimer->sleepingTime -= deltaTime;						//уменьшаем время сна
			if (currentTimer->sleepingTime <= 0.0)							//если время сна закончилось, то переводим таймер в режим работы
				currentTimer->TimerState = TimerHandleState::Running;
			++iterator;
			break;
		case TimerHandleState::Paused:	 ++iterator; break;					//если таймер приостановлен, то переходим к следующему таймеру
		case TimerHandleState::Finished: iterator = timerList.erase(iterator); break;	//если таймер завершил свою работу, то удаляем его
		}
	}
}

bool Timer::PauseTimerHandle(unsigned int id)
{
	TimerHandle* currentTimer = GetTimerHandle(id);
	if (currentTimer != nullptr)
	{
		currentTimer->TimerState = TimerHandleState::Paused;
		return true;
	}
	else
		return false;
}

bool Timer::UnpauseTimerHandle(unsigned int id)
{
	TimerHandle* currentTimer = GetTimerHandle(id);
	if (currentTimer != nullptr)
	{
		currentTimer->TimerState = TimerHandleState::Running;
		return true;
	}
	else
		return false;
}

bool Timer::SleepTimerHandle(unsigned int id, double seconds)
{
	TimerHandle* currentTimer = GetTimerHandle(id);
	if (currentTimer != nullptr)
	{
		currentTimer->TimerState = TimerHandleState::Sleeping;
		currentTimer->sleepingTime = seconds;
		return true;
	}
	else
		return false;
}

bool Timer::StopTimerHandle(unsigned int id)
{
	TimerHandle* currentTimer = GetTimerHandle(id);
	if (currentTimer != nullptr)
	{
		currentTimer->TimerState = TimerHandleState::Finished;
		return true;
	}
	else
		return false;
}

double Timer::TimeLeft(unsigned int id)
{
	TimerHandle* currentTimer = GetTimerHandle(id);
	if (currentTimer != nullptr)
		return currentTimer->delay - currentTimer->currentTime;
	else
		return 0.0;
}

double Timer::GetDeltaTimeSec()
{
	return deltaTime;
}

double Timer::GetDeltaTimeMSec()
{
	return deltaTime * 1000.0;
}

double Timer::GetWorkingTime()
{
	return workingTime;
}

uint32_t Timer::SetTimer(double seconds, double minutes, double hours, std::function<void()> lamdaFunction, bool firstDelay, int countOfRepeat)
{
	timerList.push_back(new TimerHandle(seconds + minutes * 60.0 + hours * 3600.0, lamdaFunction, firstDelay, countOfRepeat));
	return TimerHandle::counter++;
}

uint32_t Timer::SetTimer(double seconds, std::function<void()> lamdaFunction, bool firstDelay, int countOfRepeat)
{
	timerList.push_back(new TimerHandle(seconds, lamdaFunction, firstDelay, countOfRepeat));
	return TimerHandle::counter++;
}

uint32_t Timer::SetTimer(int milliseconds, std::function<void()> lamdaFunction, bool firstDelay, int countOfRepeat)
{
	timerList.push_back(new TimerHandle(milliseconds / 1000.0, lamdaFunction, firstDelay, countOfRepeat));
	return TimerHandle::counter++;
}

CTimerValue Timer::GetHighPrecisionTime()
{
	int64_t value;
	QueryPerformanceCounter((LARGE_INTEGER*)&value);
	return value;
}

uint64_t Timer::GetFrequency()
{
	return frequency;
}

Timer::TimerHandle* Timer::GetTimerHandle(unsigned int id)
{
	for (auto& currentTimer : timerList)
	{
		if (currentTimer->id == id)
			return currentTimer.Get();
	}
	return nullptr;
}

Timer::TimerHandle::TimerHandle(double time, std::function<void()> lambda, bool firstDelay, int countOfRepeat)
{
	delay = time;
	if (!firstDelay)
		currentTime = delay;
	function = lambda;
	this->countOfRepeat = countOfRepeat;
	id = counter;
}

CTimerValue::CTimerValue()
{
	value = 0;
}

CTimerValue::CTimerValue(double seconds)
{
	value = (int64_t)(seconds * pTimer->GetFrequency());
}

CTimerValue::CTimerValue(int64_t value)
{
	this->value = value;
}

void CTimerValue::Clear()
{
	value = 0;
}

CTimerValue& CTimerValue::operator=(const CTimerValue& other)
{
	value = other.value;
	return *this;
}

CTimerValue& CTimerValue::operator+=(const CTimerValue& other)
{
	value += other.value;
	return *this;
}

CTimerValue& CTimerValue::operator+(const CTimerValue& other)
{
	return CTimerValue(*this) += other;
}

CTimerValue& CTimerValue::operator-=(const CTimerValue& other)
{
	value -= other.value;
	return *this;
}

CTimerValue& CTimerValue::operator-(const CTimerValue& other)
{
	return CTimerValue(*this) -= other;
}

bool CTimerValue::operator>(const CTimerValue& other)
{
	return value > other.value;
}

bool CTimerValue::operator<(const CTimerValue& other)
{
	return value < other.value;
}

bool CTimerValue::operator>=(const CTimerValue& other)
{
	return value >= other.value;;
}

bool CTimerValue::operator<=(const CTimerValue& other)
{
	return value <= other.value;
}

bool CTimerValue::operator==(const CTimerValue& other)
{
	return value == other.value;
}

double CTimerValue::GetSeconds()
{
	return (double)value / pTimer->GetFrequency();
}

double CTimerValue::GetMilliseconds()
{
	return (double)value / pTimer->GetFrequency() * 1000.0;
}

int64_t CTimerValue::Count()
{
	return value;
}

Timing::Timing(std::function<void()> function, const std::wstring& str)
{
	description = str;
	pConsole->CPrintF(L"Измерение времени выполнения ф-ции %s", description.c_str());
	tp1 = pTimer->GetHighPrecisionTime();
	function();
}

Timing::~Timing()
{
	tp2 = pTimer->GetHighPrecisionTime();
	pConsole->CPrintF(L"Выполнение ф-ции %s завершено за %3.3f сек.", description.c_str(), (tp2 - tp1).GetSeconds());
}
