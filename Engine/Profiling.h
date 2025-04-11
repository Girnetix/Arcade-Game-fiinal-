#pragma once

#include "Timer.h"
#include "Rendering.h"
class CProfiling {
public:
	CProfiling(const std::wstring& title) : title(title) {
		isRunning = false;
	}
	void Start() {
		tpStart = pTimer->GetHighPrecisionTime();
		isRunning = true;
	}
	void Stop() {
		tpEnd = pTimer->GetHighPrecisionTime();
		isRunning = false;
	}
	void PrintInfo(int y)
	{
		pWindow->PrintMsgRightSide(y, FG_WHITE, title.c_str(), (tpEnd - tpStart).GetMilliseconds());
	}
private:
	CTimerValue tpStart, tpEnd;
	std::wstring title;
	bool isRunning;
};