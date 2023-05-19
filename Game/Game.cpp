#include "Game.h"

bool CGame::OnUserCreate()
{
	MainMenu.mo[L"Играть"][L"Редактор"][L"Выход"];
	MainMenu.mo[L"Играть"].SetAction([&]() {coreGameState = Game; mainGameState = MainGameState::Starting; }).SetSubtitle(L"Сыграйте в одиночную игру");
	MainMenu.mo[L"Редактор"].SetAction([&]() {coreGameState = Editor;  LoadWorld(); editorState = Editing; }).SetSubtitle(L"Запустите редактор карт");
	MainMenu.mo[L"Выход"].SetAction([&]() {bIsRunning = false; }).SetSubtitle(L"Выход из игры");
	MainMenu.mo.Build();
	MainMenu.mm.Open(&MainMenu.mo);

	/*menuCreatingRunner.mo[L"Влево"][L"Вправо"];
	menuCreatingRunner.mo[L"Влево"].SetSubtitle(L"Выберите начальное направление").SetAction([&]() {entityDirection = Entity::Direction::Left; });
	menuCreatingRunner.mo[L"Вправо"].SetSubtitle(L"Выберите начальное направление").SetAction([&]() {entityDirection = Entity::Direction::Right; });
	menuCreatingRunner.mo.Build();
	menuCreatingRunner.mm.Open(&menuCreatingRunner.mo);*/

	exitButton.SetButtonAction(Key::ESCAPE, [&]() {
		if (exitButtonPressedTp1 < exitButtonPressedTp2)
		{
			exitButtonPressedTp2 = pTimer->GetHighPrecisionTime();
			if ((exitButtonPressedTp2 - exitButtonPressedTp1).GetSeconds() >= 3.0)
			{
				mainGameState = Finishing;
				coreGameState = Menu;
			}
				
		}
		else
			exitButtonPressedTp1 = pTimer->GetHighPrecisionTime(); 
		});
	coreGameState = Menu;
	return true;
}

bool CGame::OnUserUpdate(double deltaTime)
{
	switch (coreGameState)
	{
		case Menu:		MainMenu.DrawMenu();		break;
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
	if (exitButton.ButtonIsHeld()) exitButton.Execute();
	if (exitButton.ButtonIsReleased()) exitButtonPressedTp1 = pTimer->GetHighPrecisionTime();
	switch (mainGameState)
	{
		case Starting:
			LoadWorld();
			mainGameState = MainGameState::Running;
			break;

		case Running:
			UpdateWorld(deltaTime);
			break;

		case Finishing:
			CloseWorld();
			break;
	}
}

void CGame::UpdateEditor(double deltaTime)
{
	switch (editorState)
	{
		case Editing:
		{
			if (Keyboard::GetKey(UP).bPressed)
			{
				yCurs--;
				if (yCurs < 0)
					yCurs = 0;
			}
			if (Keyboard::GetKey(DOWN).bPressed)
			{
				yCurs++;
				if (yCurs > pWindow->GetScrHeight() - 1)
					yCurs = pWindow->GetScrHeight() - 1;
			}
			if (Keyboard::GetKey(LEFT).bPressed)
			{
				xCurs--;
				if (xCurs < 0)
					xCurs = 0;
			}
			if (Keyboard::GetKey(RIGHT).bPressed)
			{
				xCurs++;
				if (xCurs > pWindow->GetScrWidth() - 1)
					xCurs = pWindow->GetScrWidth() - 1;
			}
			if (Keyboard::GetKey(DEL).bPressed)
				pWorld->entitiesList.remove_if([&](SmartPointer<Entity>& entity) {return entity->GetX() == xCurs && entity->GetY() == yCurs; });
			if (Keyboard::GetKey(Key::K1).bPressed)
				pWorld->entitiesList.push_back(new Wall(xCurs, yCurs));
			if (Keyboard::GetKey(Key::K2).bPressed)
				pWorld->entitiesList.push_back(new Life(xCurs, yCurs));
			if (Keyboard::GetKey(Key::K3).bPressed)
				pWorld->entitiesList.push_back(new Ammo(xCurs, yCurs));
			if (Keyboard::GetKey(Key::K4).bPressed)
				pWorld->entitiesList.push_back(new CheckPoint(xCurs, yCurs));
			if (Keyboard::GetKey(Key::K5).bPressed)
				pWorld->entitiesList.push_back(new FinishMarker(xCurs, yCurs));
			if (Keyboard::GetKey(Key::K6).bPressed)
				editorState = CreatingRunner;
			if (Keyboard::GetKey(Key::K7).bPressed)
			{
				x = xCurs;
				y = yCurs;
				editorState = CreatingRandomer;
			}
				
			if (Keyboard::GetKey(Key::K8).bPressed)
				editorState = CreatingCannon;
			if (Keyboard::GetKey(Key::K9).bPressed)
				pWorld->entitiesList.push_back(new Player(xCurs, yCurs, FG_CYAN, Entity::Direction::Up, 0.0, L"Player"));
			if (Keyboard::GetKey(Key::ESCAPE).bPressed)
			{
				pWorld->file.Clear();
				pWorld->file << pWorld->entitiesList.size();
				for (auto& entity : pWorld->entitiesList)
				{
					pWorld->file << *entity;
					if (entity->GetEntityType() == EntityType::Randomer)
					{
						Randomer* randomer = (Randomer*)entity.Get();
						pWorld->file << randomer->GetMinX() << randomer->GetMinY() << randomer->GetMaxX() << randomer->GetMaxY();
					}
				}
					
				pWorld->SaveWorld();
				coreGameState = CoreGameState::Menu;
				pWorld->entitiesList.clear();
				return;
			}

			if (pWorld->entitiesCount != pWorld->entitiesList.size())
				pWorld->entitiesCount = pWorld->entitiesList.size();

			for (auto& entity : pWorld->entitiesList)
				pWindow->PrintSymbol(entity->GetX(), entity->GetY(), entity->GetEntitySymbol(), entity->GetEntityColor());

			pWindow->PrintSymbol(xCurs, yCurs, (wchar_t)0x2588, FG_YELLOW);
			pWindow->PrintMsgLeftSide(49, FG_WHITE, L"(x:%d; y:%d) Создано объектов: %d", xCurs, yCurs, pWorld->entitiesList.size());
			pWindow->PrintMsg(90, 0, FG_WHITE, L"1 - стена");
			pWindow->PrintMsg(90, 1, FG_WHITE, L"2 - жизнь");
			pWindow->PrintMsg(90, 2, FG_WHITE, L"3 - патроны");
			pWindow->PrintMsg(90, 3, FG_WHITE, L"4 - контрольная точка");
			pWindow->PrintMsg(90, 4, FG_WHITE, L"5 - конец уровня");
			pWindow->PrintMsg(90, 5, FG_WHITE, L"6 - змея");
			pWindow->PrintMsg(90, 6, FG_WHITE, L"7 - рандомер");
			pWindow->PrintMsg(90, 7, FG_WHITE, L"8 - пушка");
		}
			break;

		case CreatingRunner: 
			
			break;

		case CreatingRandomer:
			CreateRandomer();
			break;

		case CreatingCannon:
			menuCreatingCannon.DrawMenu();
			break;
	}
	
}

void CGame::LoadWorld()
{
	pWorld->LoadWorld("test.lvl");
	Entity entity;
	for (int i = 0; i < pWorld->entitiesCount; i++)
	{
		pWorld->file >> entity;
		switch (entity.GetEntityType())
		{
			case EntityType::Player:
				pWorld->CreateEntity(new Player(entity.GetX(), entity.GetY(), entity.GetEntityColor(), entity.GetDirection(), entity.GetSpeed(), entity.GetName()));
				break;
			case EntityType::Cannon:
				pWorld->CreateEntity(new Cannon(*(Cannon*)&entity));
				break;
			case EntityType::Runner:
				pWorld->CreateEntity(new Runner(*(Runner*)&entity));
				break;
			case EntityType::Randomer:
			{
				Randomer* randomer = (Randomer*)&entity;
				pWorld->file >> minX >> minY >> maxX >> maxY;
				pWorld->CreateEntity(new Randomer(entity.GetX(), entity.GetY(), minX, minY, maxX, maxY, entity.GetEntityColor(), entity.GetSpeed()));
			}
				break;
			case EntityType::CheckPoint:
				pWorld->CreateEntity(new CheckPoint(*(CheckPoint*)&entity));
				break;
			case EntityType::Ammo:
				pWorld->CreateEntity(new Ammo(*(Ammo*)&entity));
				break;
			case EntityType::Life:
				pWorld->CreateEntity(new Life(*(Life*)&entity));
				break;
			case EntityType::Wall:
				pWorld->CreateEntity(new Wall(*(Wall*)&entity));
				break;
			case EntityType::FinishMarker:
				pWorld->CreateEntity(new FinishMarker(*(FinishMarker*)&entity));
				break;
		}
	}
	//pWorld->entitiesList.push_back(new Player(2, 35, FG_CYAN, Entity::Direction::Up, 0.0, L"Player"));
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
		if (!entity->IsAlive() && entity->GetEntityType() == EntityType::FinishMarker)
			gameWon = true;
	}
	pWorld->entitiesList.remove_if([](SmartPointer<Entity>& entity) {
		return !entity->IsAlive() && entity->GetEntityType() != EntityType::Player;
		});
}

void CGame::CloseWorld()
{
	pWorld->SaveWorld();
}

void CGame::CreateRunner(int x, int y)
{
	menuCreatingRunner.DrawMenu();
}

void CGame::CreateRandomer()
{
	if (Keyboard::GetKey(UP).bPressed)
	{
		yCurs--;
		if (yCurs < 0)
			yCurs = 0;
	}
	if (Keyboard::GetKey(DOWN).bPressed)
	{
		yCurs++;
		if (yCurs > pWindow->GetScrHeight() - 1)
			yCurs = pWindow->GetScrHeight() - 1;
	}
	if (Keyboard::GetKey(LEFT).bPressed)
	{
		xCurs--;
		if (xCurs < 0)
			xCurs = 0;
	}
	if (Keyboard::GetKey(RIGHT).bPressed)
	{
		xCurs++;
		if (xCurs > pWindow->GetScrWidth() - 1)
			xCurs = pWindow->GetScrWidth() - 1;
	}

	for (auto& entity : pWorld->entitiesList)
		pWindow->PrintSymbol(entity->GetX(), entity->GetY(), entity->GetEntitySymbol(), entity->GetEntityColor());

	pWindow->PrintSymbol(xCurs, yCurs, (wchar_t)0x2588, FG_YELLOW);
	pWindow->PrintMsgLeftSide(49, FG_WHITE, L"(x:%d; y:%d)", xCurs, yCurs);

	switch (randomerPositions)
	{
		case First:
			pWindow->PrintMsgRightSide(49, FG_WHITE, L"Выберите первую позицию");
			if (Keyboard::GetKey(RETURN).bPressed)
			{
				minX = xCurs;
				minY = yCurs;
				randomerPositions = Second;
			}
			break;
		case Second:
			pWindow->PrintMsgRightSide(49, FG_WHITE, L"Выберите вторую позицию");
			if (Keyboard::GetKey(RETURN).bPressed)
			{
				maxX = xCurs;
				maxY = yCurs;
				pWorld->CreateEntity(new Randomer(x, y, minX, minY, maxX, maxY, FG_MAGENTA, 10.0));
				randomerPositions = First;
				editorState = Editing;
			}
			break;
	}
}

void CGame::CreateCannon(int x, int y)
{

}
