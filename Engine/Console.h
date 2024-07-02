#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <list>
#include <string>
#include "Memory.h"
#include "ActionButton.h"
#include "Rendering.h"

class CTimerValue;
class Timer;

constexpr int MAX_CONSOLE_LENGTH_HISTORY = 2500;

class CConsole
{
public:
	CConsole();
	~CConsole();
	void UpdateConsole();
	void CPrintF(const wchar_t* str, ...);
	bool ConsoleIsShow() { return bShowConsole; }
private:
	double consoleUpdateRate;
	bool bShowConsole;
	CButton ToggleConsole, ConsoleHistoryUp, ConsoleHistoryDown;
	std::list<std::wstring> consoleHistory;
	std::list<std::wstring>::iterator consoleHistoryIt;
};

extern SmartPointer<CConsole> pConsole;
extern CTimerValue tp1, tp2;

#endif