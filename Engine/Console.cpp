#include "Console.h"
#include "Timer.h"
#include "TextInput.h"
#include <cmath>
#include <codecvt>

// ���������� ������� �������, ������� � ����
SmartPointer<CConsole> pConsole = nullptr;
CTimerValue tp1(int64_t(0)), tp2(int64_t(0));
TextInput input {}; // ������ ��� ����� ������

// ��������������, ��� cmdBuffer � ��� ������ ���� CommandProcessor,
// � pWindow � ��������� �� ������ ��������� ���� �������.

// ������� �������� ������ (��� �����������) � ������ ������� �������
std::vector<std::wstring> cmdHistory;
int cmdHistoryIndex = -1;

// ������ ��������� �������������� � ������� ��������� ������
std::vector<std::string> suggestions;
int suggestionIndex = 0;

// ��������� ���������� �������
double scrollOffset = 0.0;
const double scrollSpeed = 5.0; // ����� ����� � �������

CConsole::CConsole()
{
    bShowConsole = false;

    // ��������� ������ ������������ ������� (��������, F1)
    ToggleConsole.SetButtonAction(Key::F1, Key::OEM_3, [&]() { bShowConsole = !bShowConsole; });

    // ��������� �� ������� ������ ������� (���� ��������� ���������)
    ConsoleHistoryUp.SetButtonAction(Key::UP, [&]() {
        if (consoleHistoryIt == consoleHistory.begin())
            return;
        --consoleHistoryIt;
        });
    ConsoleHistoryDown.SetButtonAction(Key::DOWN, [&]() {
        if (consoleHistoryIt == consoleHistory.end())
            return;
        ++consoleHistoryIt;
        });

    // ��������� �� ������� �������� ������ (��� �������������� ���������� ������)
    cmdHistoryUp.SetButtonAction(Key::UP, [&]() {
        if (!cmdHistory.empty()) {
            // ������� ������� �������� � ������ �����
            input.SetText(cmdHistoryIt->c_str());
            // ���� �� �������� ������ ������, ��������� � ����������� ��������
            if (cmdHistoryIt != cmdHistory.begin())
                --cmdHistoryIt;
            input.MoveCursorToEnd();  // ���������� ������ � ����� ������
        }
        });

    cmdHistoryDown.SetButtonAction(Key::DOWN, [&]() {
        if (!cmdHistory.empty()) {
            input.SetText(cmdHistoryIt->c_str());
            // ���� ��������, ��������� � ���������� ��������
            std::list<std::wstring>::iterator nextIt = cmdHistoryIt;
            ++nextIt;
            if (nextIt != cmdHistory.end())
                cmdHistoryIt = nextIt;
            input.MoveCursorToEnd();  // ���������� ������ � ����� ������
        }
        });

    // ����������� ��������� ������ � ������� ������ (cmdBuffer)
    cmdBuffer.registerCommand("GetFrequency()", [&](const std::vector<std::string>& args) {
        return std::to_string(pTimer->GetFrequency());
        });
    cmdBuffer.registerCommand("GetWorkingTime()", [&](const std::vector<std::string>& args) {
        return std::to_string(pTimer->GetWorkingTime())+std::string(" seconds");
        });
    cmdBuffer.registerCommand("GetHighPrecisionTime()", [&](const std::vector<std::string>& args) {
        return std::to_string(pTimer->GetHighPrecisionTime().Count());
        });
    cmdBuffer.registerCommand("clear()", [&](const std::vector<std::string>& args) {
        consoleHistory.clear();
        return "Console history cleared";
        });
    cmdBuffer.registerCommand("CPrintF()", [&](const std::vector<std::string>& args) {
        std::string str;
        for (int i = 1; i < args.size(); i++)
            str += args[i] + " ";
        return str;
        });
}

CConsole::~CConsole()
{
    consoleHistory.clear();
}

void CConsole::UpdateConsole(double deltaTime)
{
    // ��������� ������� ����� ��� ���������
    tp2 = pTimer->GetHighPrecisionTime();

    // ������������ ������� (��������, �� F1)
    if (ToggleConsole.ButtonIsPressed())
        ToggleConsole.Execute();

    if (!bShowConsole)
        return;

    // ��������� ���� ������� � ���������
    pWindow->FillWindow(0, 0, pWindow->GetScrWidth() - 1, pWindow->GetScrHeight() - 1, BG_DARK_BLUE);
    pWindow->PrintMsgInCenterAlpha(0, FG_YELLOW, L"�������");

    // ��������� �������� ���������� ������� ������� (���� ������������ �������)
    if (ConsoleHistoryUp.ButtonIsHeld())
        scrollOffset -= scrollSpeed * deltaTime;
    if (ConsoleHistoryDown.ButtonIsHeld())
        scrollOffset += scrollSpeed * deltaTime;

    if (cmdHistoryUp.ButtonIsPressed())
        cmdHistoryUp.Execute();
    if (cmdHistoryDown.ButtonIsPressed())
        cmdHistoryDown.Execute();

    // ������������ scrollOffset � �������� ������� �������
    int totalLines = static_cast<int>(consoleHistory.size());
    int visibleLines = pWindow->GetScrHeight() - 2; // �� ������� ������ ��������� � ���� �����
    double maxOffset = (totalLines > visibleLines ? totalLines - visibleLines : 0);
    if (scrollOffset < 0.0)
        scrollOffset = 0.0;
    if (scrollOffset > maxOffset)
        scrollOffset = maxOffset;

    // ���������, � ����� ������ �������� ����������� �������
    int startLineIndex = static_cast<int>(std::floor(scrollOffset));
    auto displayIt = consoleHistory.begin();
    std::advance(displayIt, (startLineIndex < totalLines ? startLineIndex : totalLines));

    // ��������� ������� ����� ������� �������
    int iLine = 1; // ������ ������ ����� ���������
    for (auto it = displayIt; it != consoleHistory.end() && iLine < visibleLines; ++it, ++iLine)
    {
        pWindow->PrintMsgLeftSideAlpha(iLine, FG_YELLOW, it->c_str());
    }

    // ��������� ����������� ���������� ������
    if (totalLines > visibleLines) {
        double scrollRatio = scrollOffset / static_cast<double>(totalLines - visibleLines);
        int calculatedHeight = static_cast<int>(visibleLines * (static_cast<double>(visibleLines) / totalLines));
        int scrollbarHeight = (calculatedHeight < 1 ? 1 : calculatedHeight);
        int scrollbarY = static_cast<int>(scrollRatio * (visibleLines - scrollbarHeight));
        int scrollbarX1 = pWindow->GetScrWidth() - 2;
        int scrollbarX2 = pWindow->GetScrWidth() - 1;
        int scrollbarY1 = 1 + scrollbarY;
        int scrollbarY2 = scrollbarY1 + scrollbarHeight - 1;
        pWindow->FillWindow(scrollbarX1, 1, scrollbarX2, visibleLines, BG_GREY);
        pWindow->FillWindow(scrollbarX1, scrollbarY1, scrollbarX2, scrollbarY2, FG_CYAN);
    }
    else {
        int scrollbarX1 = pWindow->GetScrWidth() - 2;
        int scrollbarX2 = pWindow->GetScrWidth() - 1;
        pWindow->FillWindow(scrollbarX1, 1, scrollbarX2, visibleLines, FG_CYAN);
    }

    // ��������� ����� ������� (��� ������� RETURN)
    if (Keyboard::GetKey(RETURN).bPressed) {
        std::wstring command = input.getText();
        if (!command.empty()) {
            cmdHistory.push_back(command);
            cmdHistoryIndex = static_cast<int>(cmdHistory.size()) - 1;
            cmdHistoryIt = cmdHistory.begin();
            CPrintF(L"=> %s", command.c_str());
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converterToString;
            std::string cmdResult = cmdBuffer.executeCommand(converterToString.to_bytes(command));
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converterToWideString;
            CPrintF(converterToWideString.from_bytes(cmdResult).c_str());
            input.clear();
        }
    }

    // ��������� ���������� �������� � ���� �����: ����������� �����/������
    if (Keyboard::GetKey(Key::LEFT).bPressed)
        input.MoveCursorLeft();
    if (Keyboard::GetKey(Key::RIGHT).bPressed)
        input.MoveCursorRight();

    // ���������� ��������� ����� (�������������� ������)
    input.update(deltaTime);

    // ��������� �������� ����� ��� ��������������
    std::wstring currentInput = input.getText();
    // ����� ���� ����� (�� ��������� ������)
    pWindow->PrintMsgLeftSide(pWindow->GetScrHeight() - 1, FG_WHITE, L"=> %s", input.getDisplayText().c_str());

    if (!currentInput.empty())
    {
        std::string currentInputStr(currentInput.begin(), currentInput.end());
        suggestions = cmdBuffer.getSuggestions(currentInputStr);
        if (suggestions.empty())
            return;
        if (suggestions.size() > 5)
            suggestions.resize(5);

        // ��������� �� ������ ����������� (�������� �� ������� ������)
        if (Keyboard::GetKey(Key::F3).bPressed && suggestionIndex > 0)
            suggestionIndex--;
        if (Keyboard::GetKey(Key::F4).bPressed && suggestionIndex < static_cast<int>(suggestions.size()) - 1)
            suggestionIndex++;

        // ��� ������� TAB ��������������� ��������� ���������
        if (Keyboard::GetKey(Key::TAB).bPressed && !suggestions.empty()) {
            std::wstring suggestionW(suggestions[suggestionIndex].begin(), suggestions[suggestionIndex].end());
            input.SetText(suggestionW);
            input.MoveCursorToEnd();  // ���������� ������ � ����� ������
        }

        // ��������� ������ ��������� ��������������� ��� ����� ����� (����� ����)
        int suggestionAreaHeight = static_cast<int>(suggestions.size());
        int suggestionStartY = pWindow->GetScrHeight() - 1 - suggestionAreaHeight;
        // �������� ��� ������� ���������
        pWindow->FillWindow(0, suggestionStartY, pWindow->GetScrWidth() - 1, pWindow->GetScrHeight() - 2, BG_YELLOW);
        for (int i = 0; i < suggestionAreaHeight; i++) {
            std::wstring line = std::wstring(suggestions[i].begin(), suggestions[i].end());
            int color = (i == suggestionIndex) ? FG_RED : FG_GREEN;
            pWindow->PrintMsgLeftSideAlpha(suggestionStartY + i, color, line.c_str());
        }
    }
    else {
        suggestionIndex = 0;
        suggestions.clear();
    }

    tp1 = tp2;
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

    consoleHistory.push_back(buffer.Get());

    if (consoleHistory.size() == 1)
        consoleHistoryIt = consoleHistory.begin();

    if (consoleHistory.size() > (size_t)(pWindow->GetScrHeight() - 2))
    {
        consoleHistoryIt = consoleHistory.begin();
        size_t offset = consoleHistory.size() - (size_t)(pWindow->GetScrHeight() - 2);
        std::advance(consoleHistoryIt, offset);
    }

    va_end(args);
}
