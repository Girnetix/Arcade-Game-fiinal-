#include "Game.h"

bool CGame::OnUserCreate()
{
	menu.mo[L"Играть"][L"Редактор"][L"Выход"];
	menu.mo[L"Играть"].SetAction([&]() {coreGameState = Game; }).SetSubtitle(L"Сыграйте в одиночную игру");
	menu.mo[L"Редактор"].SetAction([&]() {coreGameState = Editor; }).SetSubtitle(L"Запустите редактор карт");
	menu.mo[L"Выход"].SetAction([&]() {bIsRunning = false; }).SetSubtitle(L"Выход из игры");
	menu.mo.Build();
	menu.mm.Open(&menu.mo);
	coreGameState = Menu;
	return true;
}

bool CGame::OnUserUpdate(double deltaTime)
{
	switch (coreGameState)
	{
		case Menu: menu.DrawMenu();		break;
		case Game: UpdateGame();		break;
		case Editor:
			break;
	}
	return true;
}

void CGame::OnUserDestroy()
{

}

void CGame::UpdateGame()
{
}

void CGame::UpdateEditor()
{
}
