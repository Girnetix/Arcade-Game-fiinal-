#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "Input.h"
#include <functional>

class CButton
{
	struct ActionKey
	{
		Key keyVal = Key::NONE;
		std::wstring keyName = L"";
		void SetValues(std::pair<Key, std::wstring> values)
		{
			keyVal = values.first;
			keyName = values.second;
		}
	};

public:
	CButton()
	{
		Reset();
		id = counter++;
	}
	void SetButtonAction(Key keyVal, std::function<void()> function);
	void SetButtonAction(Key keyVal1, Key keyVal2, std::function<void()> function);
	void Execute() { function(); }
	void Unbind(Key keyVal);
	void Reset();
	void ChangeKey(Key newKey);

	ActionKey GetFirstActionKey() { return actionKey1; }
	ActionKey GetSecondActionKey() { return actionKey2; }
	std::wstring GetKeyDescription();

	bool ButtonIsPressed() { return Keyboard::GetKey(actionKey1.keyVal).bPressed || Keyboard::GetKey(actionKey2.keyVal).bPressed; }
	bool ButtonIsHeld() { return Keyboard::GetKey(actionKey1.keyVal).bHeld || Keyboard::GetKey(actionKey2.keyVal).bHeld; }
	bool ButtonIsReleased() { return Keyboard::GetKey(actionKey1.keyVal).bReleased || Keyboard::GetKey(actionKey2.keyVal).bReleased; }
private:
	std::function<void()> function;
	ActionKey actionKey1, actionKey2;
	int id;
	static int counter;
};

#endif