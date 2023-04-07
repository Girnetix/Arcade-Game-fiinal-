#include "Input.h"

bool Keyboard::m_keyNewState[]{ 0 };
bool Keyboard::m_keyOldState[]{ 0 };
Keyboard::KeyState Keyboard::m_keys[]{ 0 };

const std::vector<std::tuple<Key, std::wstring, std::wstring, std::wstring, std::wstring>> KeyboardMap =
{
	{Key::Key_A, L"a", L"A", L"ф", L"Ф"}, {Key::Key_B, L"b", L"B", L"и", L"И"}, {Key::Key_C, L"c", L"C", L"с", L"С"}, {Key::Key_D, L"d", L"D", L"в", L"В"}, {Key::Key_E, L"e", L"E", L"у", L"У"},
	{Key::Key_F, L"f", L"F", L"а", L"А"}, {Key::Key_G, L"g", L"G", L"п", L"П"}, {Key::Key_H, L"h", L"H", L"р", L"Р"}, {Key::Key_I, L"i", L"I", L"ш", L"Ш"}, {Key::Key_J, L"j", L"J", L"о", L"О"},
	{Key::Key_K, L"k", L"K", L"л", L"Л"}, {Key::Key_L, L"l", L"L", L"д", L"Д"}, {Key::Key_M, L"m", L"M", L"ь", L"Ь"}, {Key::Key_N, L"n", L"N", L"т", L"Т"}, {Key::Key_O, L"o", L"O", L"щ", L"Щ"},
	{Key::Key_P, L"p", L"P", L"з", L"З"}, {Key::Key_Q, L"q", L"Q", L"й", L"Й"}, {Key::Key_R, L"r", L"R", L"к", L"К"}, {Key::Key_S, L"s", L"S", L"ы", L"Ы"}, {Key::Key_T, L"t", L"T", L"е", L"Е"},
	{Key::Key_U, L"u", L"U", L"г", L"Г"}, {Key::Key_V, L"v", L"V", L"м", L"М"}, {Key::Key_W, L"w", L"W", L"ц", L"Ц"}, {Key::Key_X, L"x", L"X", L"ч", L"Ч"}, {Key::Key_Y, L"y", L"Y", L"н", L"Н"},
	{Key::Key_Z, L"z", L"Z", L"я", L"Я"},

	{Key::K0, L"0", L")", L"0", L")"}, {Key::K1, L"1", L"!", L"1", L"!"}, {Key::K2, L"2", L"@", L"2", L"\""}, {Key::K3, L"3", L"#", L"3", L"№"},	{Key::K4, L"4", L"$", L"4", L";"},
	{Key::K5, L"5", L"%", L"5", L"%"}, {Key::K6, L"6", L"^", L"6", L":"}, {Key::K7, L"7", L"&", L"7", L"?"}, {Key::K8, L"8", L"*", L"8", L"*"},	{Key::K9, L"9", L"(", L"9", L"("},

	{Key::NP0, L"0", L"0", L"0", L"0"}, {Key::NP1, L"1", L"1", L"1", L"1"}, {Key::NP2, L"2", L"2", L"2", L"2"}, {Key::NP3, L"3", L"3", L"3", L"3"},	{Key::NP4, L"4", L"4", L"4", L"4"},
	{Key::NP5, L"5", L"5", L"5", L"5"}, {Key::NP6, L"6", L"6", L"6", L"6"}, {Key::NP7, L"7", L"7", L"7", L"7"}, {Key::NP8, L"8", L"8", L"8", L"8"},	{Key::NP9, L"9", L"9", L"9", L"9"},

	{Key::NP_MUL, L"*", L"*", L"*", L"*"}, {Key::NP_DIV, L"/", L"/", L"/", L"/"}, {Key::NP_ADD, L"+", L"+", L"+", L"+"}, {Key::NP_SUB, L"-", L"-", L"-", L"-"},	{Key::NP_DECIMAL, L".", L".", L".", L"."},
	{Key::PERIOD, L".", L">", L"ю", L"Ю"}, {Key::EQUALS, L"=", L"+", L"=", L"+"}, {Key::COMMA, L",", L"<", L"б", L"Б"},  {Key::MINUS, L"-", L"_", L"-", L"_"},  {Key::SPACE, L" ", L" ", L" ", L" "},
	
	{Key::OEM_1, L";", L":", L"ж", L"Ж"},   {Key::OEM_2, L"/", L"?", L".", L","}, {Key::OEM_3, L"`", L"~", L"ё", L"Ё"}, {Key::OEM_4, L"[", L"{", L"х", L"Х"},
	{Key::OEM_5, L"\\", L"|", L"\\", L"/"}, {Key::OEM_6, L"]", L"}", L"ъ", L"Ъ"}, {Key::OEM_7, L"\'", L"\"", L"э", L"Э"}
};

const std::map<Key, std::wstring> KeyNamesMap = {
	{Key::NONE, L"Отсутствует"},	
	{Key::MOUSE_LBUTTON,  L"Левая кнопка мыши"},	{Key::MOUSE_RBUTTON,  L"Правая кнопка мыши"}, 
	{Key::MOUSE_MBUTTON,  L"Кнопка мыши 3"},	{Key::MOUSE_XBUTTON1, L"Кнопка мыши 4"},	{Key::MOUSE_XBUTTON2, L"Кнопка мыши 5"},
	
	{Key::Key_A, L"A"},		{Key::Key_B, L"B"},		{Key::Key_C, L"C"},		{Key::Key_D, L"D"},		{Key::Key_E, L"E"},
	{Key::Key_F, L"F"},		{Key::Key_G, L"G"},		{Key::Key_H, L"H"},		{Key::Key_I, L"I"},		{Key::Key_J, L"J"},
	{Key::Key_K, L"K"},		{Key::Key_L, L"L"},		{Key::Key_M, L"M"},		{Key::Key_N, L"N"},		{Key::Key_O, L"O"},
	{Key::Key_P, L"P"},		{Key::Key_Q, L"Q"},		{Key::Key_R, L"R"},		{Key::Key_S, L"S"},		{Key::Key_T, L"T"},
	{Key::Key_U, L"U"},		{Key::Key_V, L"V"},		{Key::Key_W, L"W"},		{Key::Key_X, L"X"},		{Key::Key_Y, L"Y"},
	{Key::Key_Z, L"Z"},

	{Key::K0, L"0"},	{Key::K1, L"1"},	{Key::K2, L"2"},	{Key::K3, L"3"},	{Key::K4, L"4"},
	{Key::K5, L"5"},	{Key::K6, L"7"},	{Key::K7, L"7"},	{Key::K8, L"8"},	{Key::K9, L"9"},

	{Key::F1, L"F1"},	{Key::F2, L"F2"},	{Key::F3, L"F3"},	{Key::F4, L"F4"},	{Key::F5, L"F5"},	{Key::F6, L"F6"},
	{Key::F7, L"F7"},	{Key::F8, L"F8"},	{Key::F9, L"F9"},	{Key::F10, L"F10"},	{Key::F11, L"F11"},	{Key::F12, L"F12"},

	{Key::UP, L"Стрелка вверх"},	{Key::DOWN, L"Стрелка вниз"},	{Key::LEFT, L"Стрелка влево"},	{Key::RIGHT, L"Стрелка вправо"},

	{Key::SPACE, L"Пробел"},				{Key::TAB, L"Tab"},			{Key::LSHIFT, L"Левый Shift"},	{Key::RSHIFT, L"Правый Shift"},		{Key::LCONTROL, L"Левый Control"},
	{Key::RCONTROL, L"Правый Control"},		{Key::INSERT, L"Insert"},	{Key::DEL, L"Delete"},			{Key::HOME, L"Home"},				{Key::END, L"End"},	
	{Key::PGUP, L"Page Up"},				{Key::PGDN, L"Page Down"},	{Key::LALT, L"Левый Alt"},		{Key::RALT, L"Правый Alt"},			{Key::BACKSPACE, L"Backspace"},	
	{Key::ESCAPE, L"Escape"},				{Key::RETURN, L"Enter"},	{Key::PAUSE, L"Pause"},			{Key::SCROLL, L"Scroll"},			{Key::CAPS_LOCK, L"Caps Lock"},

	{Key::NP0, L"Num 0"},		{Key::NP1, L"Num 1"},		{Key::NP2, L"Num 2"},		{Key::NP3, L"Num 3"},		{Key::NP4, L"Num 4"},
	{Key::NP5, L"Num 5"},		{Key::NP6, L"Num 6"},		{Key::NP7, L"Num 7"},		{Key::NP8, L"Num 8"},		{Key::NP9, L"Num 9"},
	{Key::NP_MUL, L"Num *"},	{Key::NP_DIV, L"Num /"},	{Key::NP_ADD, L"Num +"},	{Key::NP_SUB, L"Num -"},	{Key::NP_DECIMAL, L"Num ."},

	{Key::PERIOD, L"."},	{Key::EQUALS, L"="},	{Key::COMMA, L","},		{Key::MINUS, L"-"},

	{Key::OEM_1, L";"},		{Key::OEM_2, L"/"},		{Key::OEM_3, L"`"},		{Key::OEM_4, L"["},
	{Key::OEM_5, L"\\"},	{Key::OEM_6, L"]"},		{Key::OEM_7, L"\'"}
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
