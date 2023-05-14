#include "Game.h"

bool CGame::OnUserCreate()
{
	menu.mo[L"��������� ����"][L"������� ����"][L"���������"][L"������"][L"�����"];
	menu.mo[L"��������� ����"].SetSubtitle(L"����������� ������������ ���� � ��������")[L"����� ����"][L"��������� ����"][L"�����"];
	menu.mo[L"������� ����"].Enable(false).SetSubtitle(L"���������� � ��������, ������� ���� ������");
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