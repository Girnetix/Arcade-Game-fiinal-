#include "Rendering.h"

SmartPointer<Window> pWindow = nullptr;

Window::Window()
{
	screenWidth = 120;
	screenHeight = 50;
	cursX = cursY = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	AppName = L"Unnamed app";
	ConstructWindow(screenWidth, screenHeight, AppName);
}

Window::~Window()
{
	CloseWindow();
}

void Window::PrintSymbol(int x, int y, short ch, short color)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
	{
		screen[y * screenWidth + x].Char.UnicodeChar = ch;
		screen[y * screenWidth + x].Attributes = color;
	}
}

void Window::ClearSymbol(int x, int y, const wchar_t msg)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
	{
		if (msg == screen[y * screenWidth + x].Char.UnicodeChar)
		{
			screen[y * screenWidth + x].Char.UnicodeChar = ' ';
			screen[y * screenWidth + x].Attributes = FG_BLACK | BG_BLACK;
		}
	}
}

void Window::ConstructWindow(int width, int height, const std::wstring& AppName)
{
	screenWidth = width;
	screenHeight = height;
	this->AppName = AppName;

	rectWindow = { 0, 0, 1, 1 };
	COORD coord = { (short)screenWidth, (short)screenHeight };
	SetConsoleScreenBufferSize(hConsole, coord);
	SetConsoleActiveScreenBuffer(hConsole);
	rectWindow = { 0, 0, (short)(screenWidth - 1), (short)(screenHeight - 1) };
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

	screen = new CHAR_INFO[(int64_t)screenWidth * (int64_t)screenHeight];
	ClearAllWindow();
}

void Window::PrintMsg(int x, int y, const std::wstring& msg, short color)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
	{
		for (int i = 0; i < msg.length(); i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = msg[i];
			screen[y * screenWidth + x + i].Attributes = color;
		}
	}
}

void Window::PrintMsgInCenter(int y, const std::wstring& msg, short color)
{
	if (y < screenHeight && y >= 0)
	{
		int x = screenWidth / 2 - (int)msg.length() / 2;
		for (int i = 0; i < (int)msg.length(); i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = msg[i];
			screen[y * screenWidth + x + i].Attributes = color;
		}
	}
}

void Window::PrintSymbol(int x, int y, const wchar_t msg, short color)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
	{
		screen[y * screenWidth + x].Char.UnicodeChar = msg;
		screen[y * screenWidth + x].Attributes = color;
	}
}

void Window::ClearMsg(int x, int y, const std::wstring& msg)
{
	if (x <= screenWidth && x >= 0 && y <= screenHeight && y >= 0)
	{
		for (int i = 0; i < (int)msg.length(); i++)
			if (msg[i] == screen[y * screenWidth + x + i].Char.UnicodeChar)
			{
				screen[y * screenWidth + x + i].Char.UnicodeChar = ' ';
				screen[y * screenWidth + x + i].Attributes = FG_BLACK | BG_BLACK;
			}
	}
}

void Window::ClearMsgInCenter(int y, const std::wstring& msg)
{
	if (y <= screenHeight && y >= 0)
	{
		int x = screenWidth / 2 - (int)msg.length() / 2;
		for (int i = 0; i < (int)msg.length(); i++)
			if (msg[i] == screen[y * screenWidth + x + i].Char.UnicodeChar)
			{
				screen[y * screenWidth + x + i].Char.UnicodeChar = ' ';
				screen[y * screenWidth + x + i].Attributes = FG_BLACK | BG_BLACK;
			}
	}
}


void Window::ChangeAppNme(const std::wstring& AppName)
{
	this->AppName = AppName;
	SetConsoleTitle(AppName.c_str());
}

void Window::AddToAppName(const std::wstring& str)
{
	ChangeAppNme(AppName + L" " + str);
}

void Window::UpdateWindow()
{
	WriteConsoleOutput(hConsole, screen.Get(), {(short)screenWidth, (short)screenHeight}, {0, 0}, &rectWindow);
}

void Window::CloseWindow()
{
	screen.reset();
}

void Window::ClearAllWindow()
{
	for (int i = 0; i < screenHeight * screenWidth; i++)
	{
		screen[i].Char.UnicodeChar = ' ';
		screen[i].Attributes = FG_BLACK | BG_BLACK;
	}
}

int Window::GetScrWidth()
{
	return screenWidth;
}

int Window::GetScrHeight()
{
	return screenHeight;
}

wchar_t Window::GetChar(int x, int y)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
		return screen[y * screenWidth + x].Char.UnicodeChar;
	return '\0';
}