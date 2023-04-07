#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include <vector>
#include <tuple>
#include <string>
#include <map>

enum Key
{
	NONE = 0x00,
	MOUSE_LBUTTON = VK_LBUTTON, MOUSE_RBUTTON = VK_RBUTTON, MOUSE_MBUTTON = VK_MBUTTON, MOUSE_XBUTTON1 = VK_XBUTTON1, MOUSE_XBUTTON2 = VK_XBUTTON2,

	Key_A = 0x41, Key_B = 0x42, Key_C = 0x43, Key_D = 0x44, Key_E = 0x45, Key_F = 0x46, Key_G = 0x47, Key_H = 0x48, Key_I = 0x49, Key_J = 0x4A, Key_K = 0x4B, Key_L = 0x4C, Key_M = 0x4D, Key_N = 0x4E, Key_O = 0x4F, Key_P = 0x50, Key_Q = 0x51, Key_R = 0x52, Key_S = 0x53, Key_T = 0x54, Key_U = 0x55, Key_V = 0x56, Key_W = 0x57, Key_X = 0x58, Key_Y = 0x59, Key_Z = 0x5A,
	K0 = 0x30, K1 = 0x31, K2 = 0x32, K3 = 0x33, K4 = 0x34, K5 = 0x35, K6 = 0x36, K7 = 0x37, K8 = 0x38, K9 = 0x39,

	F1 = VK_F1, F2 = VK_F2, F3 = VK_F3, F4 = VK_F4, F5 = VK_F5, F6 = VK_F6, F7 = VK_F7, F8 = VK_F8, F9 = VK_F9, F10 = VK_F10, F11 = VK_F11, F12 = VK_F12,

	UP = VK_UP, DOWN = VK_DOWN, LEFT = VK_LEFT, RIGHT = VK_RIGHT,

	SPACE = VK_SPACE, TAB = VK_TAB, SHIFT = VK_SHIFT, LSHIFT = VK_LSHIFT, RSHIFT = VK_RSHIFT, CONTROL = VK_CONTROL, LCONTROL = VK_LCONTROL, RCONTROL = VK_RCONTROL,
	INSERT = VK_INSERT, DEL = VK_DELETE, HOME = VK_HOME, END = VK_END, PGUP = VK_PRIOR, PGDN = VK_NEXT, ALT = VK_MENU, LALT = VK_LMENU, RALT = VK_RMENU,
	BACKSPACE = VK_BACK, ESCAPE = VK_ESCAPE, RETURN = VK_RETURN, PAUSE = VK_PAUSE, SCROLL = VK_SCROLL, CAPS_LOCK = VK_CAPITAL,

	NP0 = VK_NUMPAD0, NP1 = VK_NUMPAD1, NP2 = VK_NUMPAD2, NP3 = VK_NUMPAD3, NP4 = VK_NUMPAD4, NP5 = VK_NUMPAD5, NP6 = VK_NUMPAD6, NP7 = VK_NUMPAD7, NP8 = VK_NUMPAD8, NP9 = VK_NUMPAD9,
	NP_MUL = VK_MULTIPLY, NP_DIV = VK_DIVIDE, NP_ADD = VK_ADD, NP_SUB = VK_SUBTRACT, NP_DECIMAL = VK_DECIMAL,

	PERIOD = VK_OEM_PERIOD, EQUALS = VK_OEM_PLUS, COMMA = VK_OEM_COMMA, MINUS = VK_OEM_MINUS,
	OEM_1 = VK_OEM_1, OEM_2 = VK_OEM_2, OEM_3 = VK_OEM_3, OEM_4 = VK_OEM_4, OEM_5 = VK_OEM_5, OEM_6 = VK_OEM_6, OEM_7 = VK_OEM_7
};

class Keyboard
{
private:
	static struct KeyState
	{
		bool bPressed = false;
		bool bReleased = false;
		bool bHeld = false;
	} m_keys[256];
	static bool m_keyNewState[256], m_keyOldState[256];
public:
	static void UpdateKeyboard();
	static KeyState GetKey(Key key);
};

extern const std::vector<std::tuple<Key, std::wstring, std::wstring, std::wstring, std::wstring>> KeyboardMap;
extern const std::map<Key, std::wstring> KeyNamesMap;

#endif