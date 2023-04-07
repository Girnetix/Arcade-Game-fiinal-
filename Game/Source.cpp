#include <Engine/Engine.h>
#include "Bullet.h"

enum Language { En, Ru };

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
};

bool CGame::OnUserCreate()
{
	return true;
}

bool CGame::OnUserUpdate(double deltaTime)
{
	gameTime += deltaTime;
	int sec = gameTime;
	int min = sec / 60;
	int hour = min / 60;
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

	for (auto& key : KeyNamesMap)
		if (Keyboard::GetKey(key.first).bHeld)
			pWindow->PrintMsg(0, 1, FG_WHITE, L"Нажата кнопка: %s", key.second.c_str());

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