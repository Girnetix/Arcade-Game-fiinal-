#include "Game.h"

bool CGame::OnUserCreate()
{
	menu.mo[L"Одиночная игра"][L"Сетевая игра"][L"Настройки"][L"Прочее"][L"Выход"];
	menu.mo[L"Одиночная игра"].SetSubtitle(L"Насладитесь прохождением игры в одиночку")[L"Новая игра"][L"Загрузить игру"][L"Назад"];
	menu.mo[L"Сетевая игра"].Enable(false).SetSubtitle(L"Веселитесь с друзьями, проходя игру вместе");
	menu.mo.Build();
	menu.mm.Open(&menu.mo);
	coreGameState = Menu;
	return true;
}

bool CGame::OnUserUpdate(double deltaTime)
{
	switch (coreGameState)
	{
		case Menu: menu.DrawMenu();	break;
		case Game:
			break;
		case Editor:
			break;
		case Hints:
			break;
	}
	return true;
}

void CGame::OnUserDestroy()
{

}