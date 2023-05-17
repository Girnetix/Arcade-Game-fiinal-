#include "Game.h"

bool CGame::OnUserCreate()
{
	menu.mo[L"������"][L"��������"][L"�����"];
	menu.mo[L"������"].SetAction([&]() {coreGameState = Game; }).SetSubtitle(L"�������� � ��������� ����");
	menu.mo[L"��������"].SetAction([&]() {coreGameState = Editor; }).SetSubtitle(L"��������� �������� ����");
	menu.mo[L"�����"].SetAction([&]() {bIsRunning = false; }).SetSubtitle(L"����� �� ����");
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
