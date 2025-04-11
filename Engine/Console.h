#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <list>
#include <string>
#include "Memory.h"
#include "ActionButton.h"
#include "Rendering.h"
#include "CommandProcessor.h"

class CTimerValue;
class Timer;
class TextInput;

constexpr int MAX_CONSOLE_LENGTH_HISTORY = 2500;

class CConsole
{
public:
	CConsole();
	~CConsole();
	void UpdateConsole(double deltaTime);
	void CPrintF(const wchar_t* str, ...);
	bool ConsoleIsShow() { return bShowConsole; }
private:
	bool bShowConsole;
	CButton ToggleConsole, ConsoleHistoryUp, ConsoleHistoryDown, cmdHistoryUp, cmdHistoryDown;
	std::list<std::wstring> consoleHistory;
	std::list<std::wstring> cmdHistory;
	std::list<std::wstring>::iterator consoleHistoryIt;
	std::list<std::wstring>::iterator cmdHistoryIt;
	double scrollOffset = 0.0;       // Текущее положение прокрутки (в строках) с дробной точностью.
	const double scrollSpeed = 20.0; // Скорость прокрутки в строках в секунду.
	std::vector<std::string> suggestions; // Подсказки автодополнения (англ. строки)
};

extern SmartPointer<CConsole> pConsole;
extern CTimerValue tp1, tp2;
extern TextInput input; // объект для ввода текста

#endif