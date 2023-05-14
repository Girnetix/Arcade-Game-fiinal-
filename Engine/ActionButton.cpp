#include "ActionButton.h"

int CButton::counter = 0;

void CButton::SetButtonAction(Key keyVal, std::function<void()> function)
{
	this->function = function;
	actionKey1.SetValues(*KeyNamesMap.find(keyVal));
}

void CButton::SetButtonAction(Key keyVal1, Key keyVal2, std::function<void()> function)
{
	this->function = function;
	actionKey1.SetValues(*KeyNamesMap.find(keyVal1));
	if (keyVal1 != keyVal2)
		actionKey2.SetValues(*KeyNamesMap.find(keyVal2));
}

void CButton::Unbind(Key keyVal)
{
	if (actionKey1.keyVal == keyVal)
	{
		actionKey1 = actionKey2;
		actionKey2.SetValues(*KeyNamesMap.find(Key::NONE));
	}
	if (actionKey2.keyVal = keyVal)
		actionKey2.SetValues(*KeyNamesMap.find(Key::NONE));
}

void CButton::Reset()
{
	actionKey1.SetValues(*KeyNamesMap.find(Key::NONE));
	actionKey2.SetValues(*KeyNamesMap.find(Key::NONE));
}

void CButton::ChangeKey(Key newKey)
{
	if (actionKey1.keyVal == Key::NONE)
		actionKey1.SetValues(*KeyNamesMap.find(newKey));
	else if (actionKey2.keyVal == Key::NONE)
		actionKey2.SetValues(*KeyNamesMap.find(newKey));
	else
	{
		actionKey1.SetValues(*KeyNamesMap.find(newKey));
		actionKey2.SetValues(*KeyNamesMap.find(Key::NONE));
	}
}

std::wstring CButton::GetKeyDescription()
{
	if (actionKey2.keyVal != Key::NONE)
		return std::wstring(actionKey1.keyName + L"  или  " + actionKey2.keyName);
	else
		return std::wstring(actionKey1.keyName);
}
