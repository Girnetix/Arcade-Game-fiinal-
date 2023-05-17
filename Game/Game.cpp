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
		case Menu:		menu.DrawMenu();			break;
		case Game:		UpdateGame(deltaTime);		break;
		case Editor:	UpdateEditor(deltaTime);	break;
	}
	return true;
}

void CGame::OnUserDestroy()
{

}

void CGame::UpdateGame(double deltaTime)
{
	switch (mainGameState)
	{
		case Starting:LoadWorld();						break;
		case Running:UpdateWorld(deltaTime);			break;
		case Finishing:CloseWorld();					break;
	}
}

void CGame::UpdateEditor(double deltaTime)
{
	switch (editorState)
	{
		case Starting:
			break;
		case Running:
			break;
		case Finishing:
			break;
	}
}

void CGame::LoadWorld()
{
	pWorld->LoadWorld("lvl_1.lvl");
	Entity* entity = nullptr;
	EntityType et = EntityType::Player;
	for (int i = 0; i < pWorld->entitiesCount; i++)
	{
		pWorld->file >> *entity;
		switch (entity->GetEntityType())
		{
		case EntityType::Player:
			Player* player = (Player*)&entity;
			pWorld->entitiesList.push_back(new Player(*player));
			break;
		case EntityType::Bullet:
			Bullet* bullet = (Bullet*)&entity;
			pWorld->entitiesList.push_back(new Bullet(*bullet));
			break;
		case EntityType::Cannon:
			Cannon* cannon = (Cannon*)&entity;
			pWorld->entitiesList.push_back(new Cannon(*cannon));
			break;
		case EntityType::Runner:
			Runner* runner = (Runner*)&entity;
			pWorld->entitiesList.push_back(new Runner(*runner));
			break;
		case EntityType::Randomer:
			Randomer* randomer = (Randomer*)&entity;
			pWorld->entitiesList.push_back(new Randomer(*randomer));
			break;
		case EntityType::CheckPoint:
			CheckPoint* checkPoint = (CheckPoint*)&entity;
			pWorld->entitiesList.push_back(new CheckPoint(*checkPoint));
			break;
		case EntityType::Ammo:
			Ammo* ammo = (Ammo*)&entity;
			pWorld->entitiesList.push_back(new Ammo(*ammo));
			break;
		case EntityType::Life:
			Life* life = (Life*)&entity;
			pWorld->entitiesList.push_back(new Life(*life));
			break;
		case EntityType::Wall:
			Wall* wall = (Wall*)&entity;
			pWorld->entitiesList.push_back(new Wall(*wall));
			break;
		case EntityType::FinishMarker:
			FinishMarker* finishMarker = (FinishMarker*)&entity;
			pWorld->entitiesList.push_back(new FinishMarker(*finishMarker));
			break;
		}
	}
}

void CGame::UpdateWorld(double deltaTime)
{
	for (auto& entity : pWorld->entitiesList)
	{
		entity->UpdateEntity(deltaTime);
		if (entity->GetEntityType() == EntityType::Player)
		{
			Player* player = (Player*)entity.Get();
			if (player->Lifes() == 0)
				gameOver = true;
		}
		if (!entity->IsAlive() && entity->GetEntityType() != EntityType::Player)
			pWorld->entityBuf.DeleteEntityFromBuffer(entity.Get());
		if (!entity->IsAlive() && entity->GetEntityType() != EntityType::FinishMarker)
			gameWon = true;
	}
	pWorld->entitiesList.remove_if([](SmartPointer<Entity>& entity) {
		return !entity->IsAlive() && entity->GetEntityType() != EntityType::Player;
		});
}

void CGame::CloseWorld()
{

}
