#include "Input.h"

bool Keyboard::m_keyNewState[]{ 0 };
bool Keyboard::m_keyOldState[]{ 0 };
Keyboard::KeyState Keyboard::m_keys[]{ 0 };

const std::vector<std::tuple<Key, std::wstring, std::wstring, std::wstring, std::wstring>> KeyboardMap =
{
	{Key::Key_A, L"a", L"A", L"�", L"�"}, {Key::Key_B, L"b", L"B", L"�", L"�"}, {Key::Key_C, L"c", L"C", L"�", L"�"}, {Key::Key_D, L"d", L"D", L"�", L"�"}, {Key::Key_E, L"e", L"E", L"�", L"�"},
	{Key::Key_F, L"f", L"F", L"�", L"�"}, {Key::Key_G, L"g", L"G", L"�", L"�"}, {Key::Key_H, L"h", L"H", L"�", L"�"}, {Key::Key_I, L"i", L"I", L"�", L"�"}, {Key::Key_J, L"j", L"J", L"�", L"�"},
	{Key::Key_K, L"k", L"K", L"�", L"�"}, {Key::Key_L, L"l", L"L", L"�", L"�"}, {Key::Key_M, L"m", L"M", L"�", L"�"}, {Key::Key_N, L"n", L"N", L"�", L"�"}, {Key::Key_O, L"o", L"O", L"�", L"�"},
	{Key::Key_P, L"p", L"P", L"�", L"�"}, {Key::Key_Q, L"q", L"Q", L"�", L"�"}, {Key::Key_R, L"r", L"R", L"�", L"�"}, {Key::Key_S, L"s", L"S", L"�", L"�"}, {Key::Key_T, L"t", L"T", L"�", L"�"},
	{Key::Key_U, L"u", L"U", L"�", L"�"}, {Key::Key_V, L"v", L"V", L"�", L"�"}, {Key::Key_W, L"w", L"W", L"�", L"�"}, {Key::Key_X, L"x", L"X", L"�", L"�"}, {Key::Key_Y, L"y", L"Y", L"�", L"�"},
	{Key::Key_Z, L"z", L"Z", L"�", L"�"},

	{Key::K0, L"0", L")", L"0", L")"}, {Key::K1, L"1", L"!", L"1", L"!"}, {Key::K2, L"2", L"@", L"2", L"\""}, {Key::K3, L"3", L"#", L"3", L"�"},	{Key::K4, L"4", L"$", L"4", L";"},
	{Key::K5, L"5", L"%", L"5", L"%"}, {Key::K6, L"6", L"^", L"6", L":"}, {Key::K7, L"7", L"&", L"7", L"?"}, {Key::K8, L"8", L"*", L"8", L"*"},	{Key::K9, L"9", L"(", L"9", L"("},

	{Key::NP0, L"0", L"0", L"0", L"0"}, {Key::NP1, L"1", L"1", L"1", L"1"}, {Key::NP2, L"2", L"2", L"2", L"2"}, {Key::NP3, L"3", L"3", L"3", L"3"},	{Key::NP4, L"4", L"4", L"4", L"4"},
	{Key::NP5, L"5", L"5", L"5", L"5"}, {Key::NP6, L"6", L"6", L"6", L"6"}, {Key::NP7, L"7", L"7", L"7", L"7"}, {Key::NP8, L"8", L"8", L"8", L"8"},	{Key::NP9, L"9", L"9", L"9", L"9"},

	{Key::NP_MUL, L"*", L"*", L"*", L"*"}, {Key::NP_DIV, L"/", L"/", L"/", L"/"}, {Key::NP_ADD, L"+", L"+", L"+", L"+"}, {Key::NP_SUB, L"-", L"-", L"-", L"-"},	{Key::NP_DECIMAL, L".", L".", L".", L"."},
	{Key::PERIOD, L".", L">", L"�", L"�"}, {Key::EQUALS, L"=", L"+", L"=", L"+"}, {Key::COMMA, L",", L"<", L"�", L"�"},  {Key::MINUS, L"-", L"_", L"-", L"_"},  {Key::SPACE, L" ", L" ", L" ", L" "},
	
	{Key::OEM_1, L";", L":", L"�", L"�"},   {Key::OEM_2, L"/", L"?", L".", L","}, {Key::OEM_3, L"`", L"~", L"�", L"�"}, {Key::OEM_4, L"[", L"{", L"�", L"�"},
	{Key::OEM_5, L"\\", L"|", L"\\", L"/"}, {Key::OEM_6, L"]", L"}", L"�", L"�"}, {Key::OEM_7, L"\'", L"\"", L"�", L"�"}
};

void Keyboard::UpdateKeyboard()
{
	for (int i = 0; i < 256; i++)
	{
		m_keyNewState[i] = GetAsyncKeyState(i);

		m_keys[i].bPressed = false;
		m_keys[i].bReleased = false;

		if (m_keyNewState[i] != m_keyOldState[i])
		{
			if (m_keyNewState[i])
			{
				m_keys[i].bPressed = !m_keys[i].bHeld;
				m_keys[i].bHeld = true;
			}
			else
			{
				m_keys[i].bReleased = true;
				m_keys[i].bHeld = false;
			}
		}

		m_keyOldState[i] = m_keyNewState[i];
	}
}

Keyboard::KeyState Keyboard::GetKey(Key key)
{
	return m_keys[key];
}
