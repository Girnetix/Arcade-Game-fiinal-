#include <Engine/Engine.h>
#include "Bullet.h"

enum Language { En, Ru };

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
	void SetButtonAction(Key keyVal, std::function<void()> function)
	{
		this->function = function;
		actionKey1.SetValues(*KeyNamesMap.find(keyVal));
	}
	void SetButtonAction(Key keyVal1, Key keyVal2, std::function<void()> function)
	{
		this->function = function;
		actionKey1.SetValues(*KeyNamesMap.find(keyVal1));
		if (keyVal1 != keyVal2)
			actionKey2.SetValues(*KeyNamesMap.find(keyVal2));
	}
	void Execute()
	{
		function();
	}
	void Unbind(Key keyVal)
	{
		if (actionKey1.keyVal == keyVal)
		{
			actionKey1 = actionKey2;
			actionKey2.SetValues(*KeyNamesMap.find(Key::NONE));
		}
		if(actionKey2.keyVal = keyVal)
			actionKey2.SetValues(*KeyNamesMap.find(Key::NONE));
	}
	void Reset()
	{
		actionKey1.SetValues(*KeyNamesMap.find(Key::NONE));
		actionKey2.SetValues(*KeyNamesMap.find(Key::NONE));
	}
	void ChangeKey(Key newKey)
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
	ActionKey GetFirstActionKey()
	{
		return actionKey1;
	}
	ActionKey GetSecondActionKey()
	{
		return actionKey2;
	}
	std::wstring GetKeyDescription()
	{
		if (actionKey2.keyVal != Key::NONE)
			return std::wstring(actionKey1.keyName + L"  или  " + actionKey2.keyName);
		else
			return std::wstring(actionKey1.keyName);
	}
	bool ButtonIsPressed()
	{
		return Keyboard::GetKey(actionKey1.keyVal).bPressed || Keyboard::GetKey(actionKey2.keyVal).bPressed;
	}
	bool ButtonIsHeld()
	{
		return Keyboard::GetKey(actionKey1.keyVal).bHeld || Keyboard::GetKey(actionKey2.keyVal).bHeld;
	}
	bool ButtonIsReleased()
	{
		return Keyboard::GetKey(actionKey1.keyVal).bReleased || Keyboard::GetKey(actionKey2.keyVal).bReleased;
	}
private:
	std::function<void()> function;
	ActionKey actionKey1, actionKey2;
	int id;
	static int counter;
};

int CButton::counter = 0;

class CGame :public CEngine
{
public:
	bool OnUserCreate() override;
	bool OnUserUpdate(double deltaTime) override;
	void OnUserDestroy() override;
private:
	double gameTime = 0.0;
	Language lang = Ru;
	std::wstring stringText = L"";
	int stringTextCursor = 0;
	bool changeLayout = false;
	bool bShowTime = true;
	bool bShowInput = true;
	CButton ToggleTimeDisplay, ToggleInputDisplay, StopApplication;
};

bool CGame::OnUserCreate()
{
	StopApplication.SetButtonAction(Key::ESCAPE, [&]() {bIsRunning = !bIsRunning; });
	ToggleInputDisplay.SetButtonAction(Key::SPACE, [&]() {bShowInput = !bShowInput; });
	ToggleTimeDisplay.SetButtonAction(Key::TAB, Key::LSHIFT, [&]() {bShowTime = !bShowTime; });
	return true;
}

bool CGame::OnUserUpdate(double deltaTime)
{
	gameTime += deltaTime;
	int sec = (int)gameTime;
	int min = sec / 60;
	int hour = min / 60;
	if (bShowTime)
		if (sec >= 60 * 60)
			pWindow->PrintMsgInCenter(0, FG_CYAN, L"Time: %2d:%02d:%02d", hour, min % 60, sec % 60);
		else
			pWindow->PrintMsgInCenter(0, FG_CYAN, L"Time: %2d:%02d", min % 60, sec % 60);

	/*if (Keyboard::GetKey(SHIFT).bHeld && Keyboard::GetKey(ALT).bHeld && !changeLayout)
		changeLayout = true;

	if (changeLayout)
		if (Keyboard::GetKey(SHIFT).bReleased || Keyboard::GetKey(ALT).bReleased)
		{
			if (lang == En)
				lang = Ru;
			else
				lang = En;
			changeLayout = false;
		}

	for (auto& key : KeyboardMap)
		if (Keyboard::GetKey(std::get<0>(key)).bPressed)
		{
			stringText.insert(stringTextCursor, lang == En ? (Keyboard::GetKey(SHIFT).bHeld ? std::get<2>(key) : std::get<1>(key)) : (Keyboard::GetKey(SHIFT).bHeld ? std::get<4>(key) : std::get<3>(key)));
			stringTextCursor++;
		}

	if (Keyboard::GetKey(Key::LEFT).bPressed)
		stringTextCursor = (0 > (stringTextCursor - 1)) ? 0 : (stringTextCursor - 1);
	if (Keyboard::GetKey(Key::RIGHT).bPressed)
		stringTextCursor = ((int32_t)stringText.size()) < (stringTextCursor + 1) ? (int32_t)stringText.size() : (stringTextCursor + 1);
	if (Keyboard::GetKey(Key::BACKSPACE).bPressed && stringTextCursor > 0)
	{
		stringText.erase(stringTextCursor - 1, 1);
		stringTextCursor = (0 > (stringTextCursor - 1)) ? 0 : (stringTextCursor - 1);
	}
	if (Keyboard::GetKey(Key::DEL).bPressed && stringTextCursor < stringText.size())
		stringText.erase(stringTextCursor, 1);

	if (lang == En)
		pWindow->PrintMsg(0, 0, FG_WHITE, L"English");
	else
		pWindow->PrintMsg(0, 0, FG_WHITE, L"Russian");

	pWindow->PrintSymbolAlpha(11 + stringTextCursor, 1, L' ', BG_WHITE);
	pWindow->PrintMsgAlpha(0, 1, FG_CYAN, L"Your text: %s", stringText.c_str());*/
	if(bShowInput)
	for (auto& key : KeyNamesMap)
		if (Keyboard::GetKey(key.first).bHeld)
			pWindow->PrintMsg(0, 1, FG_WHITE, L"Нажата кнопка: %s", key.second.c_str());

	if (StopApplication.ButtonIsPressed())
		StopApplication.Execute();

	if(ToggleTimeDisplay.ButtonIsPressed())
		ToggleTimeDisplay.Execute();

	if(ToggleInputDisplay.ButtonIsPressed())
		ToggleInputDisplay.Execute();

	if (bShowInput)
		pWindow->PrintMsgLeftSide(pWindow->GetScrHeight() - 3, FG_WHITE, L"Не отображать нажатия клавиш - %s", ToggleInputDisplay.GetKeyDescription().c_str());
	else
		pWindow->PrintMsgLeftSide(pWindow->GetScrHeight() - 3, FG_WHITE, L"Отображать нажатия клавиш - %s", ToggleInputDisplay.GetKeyDescription().c_str());

	if (bShowTime)
		pWindow->PrintMsgLeftSide(pWindow->GetScrHeight() - 2, FG_WHITE, L"Не отображать таймер - %s", ToggleTimeDisplay.GetKeyDescription().c_str());
	else
		pWindow->PrintMsgLeftSide(pWindow->GetScrHeight() - 2, FG_WHITE, L"Отображать таймер - %s", ToggleTimeDisplay.GetKeyDescription().c_str());

	pWindow->PrintMsgLeftSide(pWindow->GetScrHeight() - 1, FG_WHITE, L"Выход - %s", StopApplication.GetKeyDescription().c_str());

	return true;
}

void CGame::OnUserDestroy()
{

}

int main()
{
	Entity* entity = nullptr;
	CGame game;
	game.Start(120, 50, 12, 16, L"Test");
	return 0;
}