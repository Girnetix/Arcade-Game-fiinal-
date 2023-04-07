#include "Rendering.h"

SmartPointer<Window> pWindow = nullptr;

Window::Window()
{
	screenWidth = 120;
	screenHeight = 50;
	cursX = cursY = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	AppName = L"Unnamed app";
	ConstructWindow(screenWidth, screenHeight, 8, 12, L"Точечные шрифты", AppName);
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

void Window::PrintSymbolAlpha(int x, int y, short ch, short color)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
	{
		screen[y * screenWidth + x].Char.UnicodeChar = ch;
		screen[y * screenWidth + x].Attributes |= color;
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

void Window::ConstructWindow(int width, int height, int fontw, int fonth, const std::wstring& font, const std::wstring& AppName)
{
	screenWidth = width;
	screenHeight = height;
	this->AppName = AppName;

	rectWindow = { 0, 0, 1, 1 };
	COORD coord = { (short)screenWidth, (short)screenHeight };
	SetConsoleScreenBufferSize(hConsole, coord);
	SetConsoleActiveScreenBuffer(hConsole);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, font.c_str());
	SetCurrentConsoleFontEx(hConsole, false, &cfi);

	rectWindow = { 0, 0, (short)(screenWidth - 1), (short)(screenHeight - 1) };
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

	screen = new CHAR_INFO[(int64_t)screenWidth * (int64_t)screenHeight];
	ClearAllWindow();
}

void Window::PrintMsg(int x, int y, short color, const wchar_t* msg, ...)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
	{
		va_list args;
		va_start(args, msg);

		int len = _vscwprintf(msg, args) + 1;
		SmartPointer<wchar_t[]> buffer(new wchar_t[len * sizeof(char)]);
		vswprintf_s(buffer.Get(), len, msg, args);

		va_end(args);

		for (int i = 0; i < len - 1; i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = buffer[i];
			screen[y * screenWidth + x + i].Attributes = color;
		}
	}
}

void Window::PrintMsgAlpha(int x, int y, short color, const wchar_t* msg, ...)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
	{
		va_list args;
		va_start(args, msg);

		int len = _vscwprintf(msg, args) + 1;
		SmartPointer<wchar_t[]> buffer(new wchar_t[len * sizeof(char)]);
		vswprintf_s(buffer.Get(), len, msg, args);

		va_end(args);

		for (int i = 0; i < len - 1; i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = buffer[i];
			screen[y * screenWidth + x + i].Attributes |= color;
		}
	}
}

void Window::PrintMsgLeftSide(int y, short color, const wchar_t* msg, ...)
{
	if (y < screenHeight && y >= 0)
	{
		va_list args;
		va_start(args, msg);

		int len = _vscwprintf(msg, args) + 1;
		SmartPointer<wchar_t[]> buffer(new wchar_t[len * sizeof(char)]);
		vswprintf_s(buffer.Get(), len, msg, args);

		va_end(args);
		int x = 0;
		for (int i = 0; i < len - 1; i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = buffer[i];
			screen[y * screenWidth + x + i].Attributes = color;
		}
	}
}

void Window::PrintMsgLeftSideAlpha(int y, short color, const wchar_t* msg, ...)
{
	if (y < screenHeight && y >= 0)
	{
		va_list args;
		va_start(args, msg);

		int len = _vscwprintf(msg, args) + 1;
		SmartPointer<wchar_t[]> buffer(new wchar_t[len * sizeof(char)]);
		vswprintf_s(buffer.Get(), len, msg, args);

		va_end(args);
		int x = 0;
		for (int i = 0; i < len - 1; i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = buffer[i];
			screen[y * screenWidth + x + i].Attributes |= color;
		}
	}
}

void Window::PrintMsgRightSide(int y, short color, const wchar_t* msg, ...)
{
	if (y < screenHeight && y >= 0)
	{
		va_list args;
		va_start(args, msg);

		int len = _vscwprintf(msg, args) + 1;
		SmartPointer<wchar_t[]> buffer(new wchar_t[len * sizeof(char)]);
		vswprintf_s(buffer.Get(), len, msg, args);

		va_end(args);
		int x = screenWidth - len + 1;
		for (int i = 0; i < len - 1; i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = buffer[i];
			screen[y * screenWidth + x + i].Attributes = color;
		}
	}
}

void Window::PrintMsgRightSideAlpha(int y, short color, const wchar_t* msg, ...)
{
	if (y < screenHeight && y >= 0)
	{
		va_list args;
		va_start(args, msg);

		int len = _vscwprintf(msg, args) + 1;
		SmartPointer<wchar_t[]> buffer(new wchar_t[len * sizeof(char)]);
		vswprintf_s(buffer.Get(), len, msg, args);

		va_end(args);
		int x = screenWidth - len + 1;
		for (int i = 0; i < len - 1; i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = buffer[i];
			screen[y * screenWidth + x + i].Attributes |= color;
		}
	}
}

void Window::PrintMsgInCenter(int y, short color, const wchar_t* msg, ...)
{
	if (y < screenHeight && y >= 0)
	{
		va_list args;
		va_start(args, msg);

		int len = _vscwprintf(msg, args) + 1;
		SmartPointer<wchar_t[]> buffer(new wchar_t[len * sizeof(char)]);
		vswprintf_s(buffer.Get(), len, msg, args);

		va_end(args);
		int x = (screenWidth - len) / 2;
		for (int i = 0; i < len - 1; i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = buffer[i];
			screen[y * screenWidth + x + i].Attributes = color;
		}
	}
}

void Window::PrintMsgInCenterAlpha(int y, short color, const wchar_t* msg, ...)
{
	if (y < screenHeight && y >= 0)
	{
		va_list args;
		va_start(args, msg);

		int len = _vscwprintf(msg, args) + 1;
		SmartPointer<wchar_t[]> buffer(new wchar_t[len * sizeof(char)]);
		vswprintf_s(buffer.Get(), len, msg, args);

		va_end(args);
		int x = (screenWidth - len) / 2;
		for (int i = 0; i < len - 1; i++)
		{
			screen[y * screenWidth + x + i].Char.UnicodeChar = buffer[i];
			screen[y * screenWidth + x + i].Attributes |= color;
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

void Window::PrintSymbolAlpha(int x, int y, const wchar_t msg, short color)
{
	if (x < screenWidth && x >= 0 && y < screenHeight && y >= 0)
	{
		screen[y * screenWidth + x].Char.UnicodeChar = msg;
		screen[y * screenWidth + x].Attributes |= color;
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