#include "Console.h"
#include "Timer.h"

SmartPointer<CConsole> pConsole = nullptr;
CTimerValue tp1(int64_t(0)), tp2(int64_t(0));



CConsole::CConsole()
{
	consoleUpdateRate = 1 / 10.0;
	bShowConsole = false;
	ToggleConsole.SetButtonAction(Key::F1, Key::OEM_3, [&]() { bShowConsole = !bShowConsole; });
	ConsoleHistoryUp.SetButtonAction(Key::UP,
		[&]() {
			if (consoleHistoryIt == consoleHistory.begin())
				return;
			--consoleHistoryIt;
		});
	ConsoleHistoryDown.SetButtonAction(Key::DOWN,
		[&]() {
			if (consoleHistoryIt == consoleHistory.end())
				return;
			++consoleHistoryIt;
		});
}

CConsole::~CConsole()
{
	consoleHistory.clear();
}

void CConsole::UpdateConsole()
{
	tp2 = pTimer->GetHighPrecisionTime();

	if (ToggleConsole.ButtonIsPressed())
		ToggleConsole.Execute();

	if (!bShowConsole)
		return;

	pWindow->FillWindow(0, 0, pWindow->GetScrWidth() - 1, pWindow->GetScrHeight() - 1, BG_DARK_BLUE);
	pWindow->PrintMsgInCenterAlpha(0, FG_YELLOW, L"Консоль");
	int iLine = 1;

	if (consoleHistory.size())
		for (; consoleHistoryIt != consoleHistory.end() && iLine != pWindow->GetScrHeight() - 1; ++consoleHistoryIt)
		{
			pWindow->PrintMsgLeftSideAlpha(iLine, FG_YELLOW, consoleHistoryIt->c_str());
			iLine++;
		}
		

	if ((tp2 - tp1).GetSeconds() <= consoleUpdateRate)
		return;
	tp1 = tp2;

	if (ConsoleHistoryUp.ButtonIsHeld())
		ConsoleHistoryUp.Execute();

	if (ConsoleHistoryDown.ButtonIsHeld())
		ConsoleHistoryDown.Execute();
}

void CConsole::CPrintF(const wchar_t* str, ...)
{
	va_list args;
	va_start(args, str);

	int len = _vscwprintf(str, args) + 1;
	SmartPointer<wchar_t[]> buffer(new wchar_t[len]);
	vswprintf_s(buffer.Get(), len, str, args);

	if (consoleHistory.size() == MAX_CONSOLE_LENGTH_HISTORY)
		consoleHistory.pop_front();

	consoleHistory.emplace_back(buffer.Get());

	if (consoleHistory.size() == 1)
		consoleHistoryIt = consoleHistory.begin();

	if (consoleHistory.size() > (size_t)(pWindow->GetScrHeight() - 2))
		std::advance(consoleHistoryIt, consoleHistory.size() - (size_t)(pWindow->GetScrHeight() - 2));

	va_end(args);
}
