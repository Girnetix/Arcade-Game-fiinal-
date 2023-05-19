#include "Game.h"

bool CGame::OnUserCreate()
{
	MainMenu.mo[L"Играть"][L"Редактор"][L"Выход"];
	MainMenu.mo[L"Играть"].SetAction([&]() {coreGameState = Game; mainGameState = MainGameState::Starting; }).SetSubtitle(L"Сыграйте в одиночную игру");
	MainMenu.mo[L"Редактор"].SetAction([&]() {coreGameState = Editor;  LoadWorld(); editorState = Editing; }).SetSubtitle(L"Запустите редактор карт");
	MainMenu.mo[L"Выход"].SetAction([&]() {bIsRunning = false; }).SetSubtitle(L"Выход из игры");
	MainMenu.mo.Build();
	MainMenu.mm.Open(&MainMenu.mo);

	coreGameState = Menu;
	return true;
}

bool CGame::OnUserUpdate(double deltaTime)
{
	switch (coreGameState)
	{
		case Menu:		
			MainMenu.DrawMenu();
			pWindow->PrintMsgRightSide(0, FG_WHITE, L"Arrow-man 1.00b");
			pWindow->PrintMsgRightSide(pWindow->GetScrHeight() - 1, FG_WHITE, L"W - вверх, S - вниз, Enter - выбрать");
			break;
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
		case Starting:
			LoadWorld();
			mainGameState = MainGameState::Running;
			break;

		case Running:
			if (Keyboard::GetKey(ESCAPE).bPressed) exitButtonPressedTp1 = pTimer->GetHighPrecisionTime();
			if (Keyboard::GetKey(ESCAPE).bHeld) {
				exitButtonPressedTp2 = pTimer->GetHighPrecisionTime();
				pWindow->PrintMsgRightSide(pWindow->GetScrHeight() - 1, FG_WHITE, L"Удерживайте клавишу Escape для выхода в меню: %d", (int)(3.0 - (exitButtonPressedTp2 - exitButtonPressedTp1).GetSeconds()) + 1);
				if ((exitButtonPressedTp2 - exitButtonPressedTp1).GetSeconds() >= 3.0)
				{
					mainGameState = Finishing;
					break;
				}		
			}
			if (!gameOver && !gameWon)
			{
				gameTime += deltaTime;
				sec = gameTime;
				min = sec / 60;
				hour = min / 60;
				UpdateWorld(deltaTime);
				DrawLegend();
				if (gameTime < 3600.0)
					pWindow->PrintMsgRightSide(0, FG_WHITE, L"Время: %02d:%02d", min % 60, sec % 60);
				else
					pWindow->PrintMsgRightSide(0, FG_WHITE, L"Время: %02d:%02d:%02d", hour, min % 60, sec % 60);
			}
			else
			{
				auto it = std::find_if(pWorld->entitiesList.begin(), pWorld->entitiesList.end(), [&](SmartPointer<Entity>& entity) {return entity->GetEntityType() == EntityType::Player; });
				if (it != pWorld->entitiesList.end())
				{
					Player* player = (Player*)it->Get();
					totalKills = player->Kills();
					totalDeaths = player->Deaths();
				}
				if (gameWon == true)
					mainGameState = GameWon;
				if (gameOver == true)
					mainGameState = GameOver;
				totalScore = (totalKills * 50) - totalDeaths * 30;
				if (totalScore < 0)
					totalScore = 0;
			}
			break;
		case GameWon:
			returningTimeInMenu -= deltaTime;
			if (Keyboard::GetKey(SPACE).bPressed)
				returningTimeInMenu = 0;
			if (returningTimeInMenu > 0.0)
			{
				pWindow->PrintMsgInCenter(20, FG_CYAN, L"ВЫ ПОБЕДИЛИ!");
				pWindow->PrintMsgInCenter(22, FG_WHITE, L"Статистика");
				if (gameTime < 3600.0)
					pWindow->PrintMsgInCenter(23, FG_YELLOW, L"Время: %02d:%02d", min % 60, sec % 60);
				else
					pWindow->PrintMsgInCenter(23, FG_YELLOW, L"Время: %02d:%02d:%02d", hour, min % 60, sec % 60);
				pWindow->PrintMsgInCenter(25, FG_GREEN, L"Убийств: %d       Смертей: %d", totalKills, totalDeaths);
				pWindow->PrintMsgInCenter(26, FG_MAGENTA, L"Ваш счёт: %d", totalScore);
				pWindow->PrintMsgInCenter(40, FG_WHITE, L"Возврат в главное меню через: %d", (int)returningTimeInMenu + 1);
			}
			else
				mainGameState = Finishing;
			
			break;
		case GameOver:		
			returningTimeInMenu -= deltaTime;
			if (Keyboard::GetKey(SPACE).bPressed)
				returningTimeInMenu = 0;
			if (returningTimeInMenu > 0.0)
			{
				pWindow->PrintMsgInCenter(20, FG_RED, L"ВЫ ПРОИГРАЛИ!");
				pWindow->PrintMsgInCenter(22, FG_WHITE, L"Статистика");
				if (gameTime < 3600.0)
					pWindow->PrintMsgInCenter(23, FG_YELLOW, L"Время: %02d:%02d", min % 60, sec % 60);
				else
					pWindow->PrintMsgInCenter(23, FG_YELLOW, L"Время: %02d:%02d:%02d", hour, min % 60, sec % 60);
				pWindow->PrintMsgInCenter(25, FG_GREEN, L"Убийств: %d       Смертей: %d", totalKills, totalDeaths);
				pWindow->PrintMsgInCenter(26, FG_MAGENTA, L"Ваш счёт: %d", totalScore);
				pWindow->PrintMsgInCenter(30, FG_WHITE, L"Не расстраивайтесь! В следующий раз у Вас все получится!");
				pWindow->PrintMsgInCenter(40, FG_WHITE, L"Возврат в главное меню через: %d", (int)returningTimeInMenu + 1);
			}
			else
				mainGameState = Finishing;
			break;
		case Finishing:
				CloseWorld();
				coreGameState = Menu;
				gameTime = 0.0;
				returningTimeInMenu = 10.0;
				gameWon = gameOver = false;
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
				pickedEntity = nullptr;
			}
			if (Keyboard::GetKey(DOWN).bPressed)
			{
				yCurs++;
				if (yCurs > pWindow->GetScrHeight() - 1)
					yCurs = pWindow->GetScrHeight() - 1;
				pickedEntity = nullptr;
			}
			if (Keyboard::GetKey(LEFT).bPressed)
			{
				xCurs--;
				if (xCurs < 0)
					xCurs = 0;
				pickedEntity = nullptr;
			}
			if (Keyboard::GetKey(RIGHT).bPressed)
			{
				xCurs++;
				if (xCurs > pWindow->GetScrWidth() - 1)
					xCurs = pWindow->GetScrWidth() - 1;
				pickedEntity = nullptr;
			}
			if (pickedEntity == nullptr)
			{
				auto it = std::find_if(pWorld->entitiesList.begin(), pWorld->entitiesList.end(), [&](SmartPointer<Entity>& entity) {return entity->GetX() == xCurs && entity->GetY() == yCurs; });
				if (it != pWorld->entitiesList.end())
					pickedEntity = it->Get();
			}
			if (Keyboard::GetKey(DEL).bPressed)
			{
				pickedEntity = nullptr;
				pWorld->entitiesList.remove_if([&](SmartPointer<Entity>& entity) {
					if (entity->GetX() == xCurs && entity->GetY() == yCurs && entity->GetEntityType() == EntityType::Player)
						countOfPlayers--;
					return entity->GetX() == xCurs && entity->GetY() == yCurs; });
			}
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
				pWorld->entitiesList.push_back(new Runner(xCurs, yCurs, FG_GREEN, Entity::Direction::Left, 20.0));
			if (Keyboard::GetKey(Key::K7).bPressed)
			{
				x = xCurs;
				y = yCurs;
				editorState = CreatingRandomer;
			}
				
			if (Keyboard::GetKey(Key::K8).bPressed)
				pWorld->entitiesList.push_back(new Cannon(xCurs, yCurs, FG_RED, Entity::Direction::Up, 0.5));
			if (Keyboard::GetKey(Key::K9).bPressed && countOfPlayers == 0)
			{
				countOfPlayers++;
				pWorld->entitiesList.push_back(new Player(xCurs, yCurs, FG_CYAN, Entity::Direction::Up, 0.0, L"Player"));
			}		
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
				CloseWorld();
				coreGameState = CoreGameState::Menu;
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
			pWindow->PrintMsg(90, 8, FG_WHITE, L"9 - игрок");

			if (pickedEntity != nullptr)
			{
				pWindow->PrintMsgRightSide(20, FG_WHITE, L"Объект: %s", pickedEntity->GetName().c_str());
				pWindow->PrintMsgRightSide(21, FG_WHITE, L"id: %d", pickedEntity->GetId());
				pWindow->PrintMsgRightSide(22, FG_WHITE, L"(x: %d; y: %d)", pickedEntity->GetX(), pickedEntity->GetY());
				if (pickedEntity->IsMovable())
				{
					pWindow->PrintMsgRightSide(23, FG_WHITE, L"Направление: %c", (wchar_t)pickedEntity->GetDirection());
					pWindow->PrintMsgRightSide(24, FG_WHITE, L"Скорость: %3.1f", pickedEntity->GetSpeed());
				}
				if (Keyboard::GetKey(Key::TAB).bPressed && pickedEntity->IsMovable())
				{
					switch (pickedEntity->GetEntityType())
					{	
						case EntityType::Player:
						{
							switch (pickedEntity->GetDirection())
							{
								case Entity::Direction::Up:
									pickedEntity->SetDirection(Entity::Direction::Right);
									break;
								case Entity::Direction::Down:
									pickedEntity->SetDirection(Entity::Direction::Left);
									break;
								case Entity::Direction::Right:
									pickedEntity->SetDirection(Entity::Direction::Down);
									break;
								case Entity::Direction::Left:
									pickedEntity->SetDirection(Entity::Direction::Up);
									break;
							}
						}
							break;
						case EntityType::Cannon:
						{
							switch (pickedEntity->GetDirection())
							{
							case Entity::Direction::Up:
								pickedEntity->SetDirection(Entity::Direction::Right);
								break;
							case Entity::Direction::Down:
								pickedEntity->SetDirection(Entity::Direction::Left);
								break;
							case Entity::Direction::Right:
								pickedEntity->SetDirection(Entity::Direction::Down);
								break;
							case Entity::Direction::Left:
								pickedEntity->SetDirection(Entity::Direction::Up);
								break;
							}
						}
							break;
						case EntityType::Runner:
						{
							switch (pickedEntity->GetDirection())
							{
							case Entity::Direction::Right:
								pickedEntity->SetDirection(Entity::Direction::Left);
								break;
							case Entity::Direction::Left:
								pickedEntity->SetDirection(Entity::Direction::Right);
								break;
							}
						}
							break;
						default:
							break;
					}
				}
				if (Keyboard::GetKey(Key::Key_S).bPressed && pickedEntity->IsMovable())
					editorState = EditingSpeed;
			}
		}
			break;

		case CreatingRandomer:
			CreateRandomer();
			break;

		case EditingSpeed:
		{
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
			pWindow->PrintMsg(90, 8, FG_WHITE, L"9 - игрок");
			pWindow->PrintMsgRightSide(20, FG_WHITE, L"Объект: %s", pickedEntity->GetName().c_str());
			pWindow->PrintMsgRightSide(21, FG_WHITE, L"id: %d", pickedEntity->GetId());
			pWindow->PrintMsgRightSide(22, FG_WHITE, L"(x: %d; y: %d)", pickedEntity->GetX(), pickedEntity->GetY());
			if (pickedEntity->IsMovable())
			{
				pWindow->PrintMsgRightSide(23, FG_WHITE, L"Направление: %c", (wchar_t)pickedEntity->GetDirection());
				pWindow->PrintMsgRightSide(24, FG_WHITE, L"Скорость: %3.1f", pickedEntity->GetSpeed());
			}
			if (Keyboard::GetKey(Key::UP).bPressed)
			{
				if (pickedEntity->GetSpeed() + 0.1 <= 50.0)
					pickedEntity->SetSpeed(pickedEntity->GetSpeed() + 0.1);
			}
			if (Keyboard::GetKey(Key::DOWN).bPressed)
			{
				if (pickedEntity->GetSpeed() - 0.1 >= 0.1)
					pickedEntity->SetSpeed(pickedEntity->GetSpeed() - 0.1);
			}
			if (Keyboard::GetKey(Key::ESCAPE).bPressed)
				editorState = Editing;
		}
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
			{
				pWorld->CreateEntity(new Player(entity.GetX(), entity.GetY(), entity.GetEntityColor(), entity.GetDirection(), entity.GetSpeed(), entity.GetName()));
				countOfPlayers++;
			}				
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
}

void CGame::UpdateWorld(double deltaTime)
{
	for (auto& entity : pWorld->entitiesList)
	{
		entity->UpdateEntity(deltaTime);
		if (entity->GetEntityType() == EntityType::Player)
		{
			Player* player = (Player*)entity.Get();
			player->PrintInfo();
			if (player->Lifes() == 0)
				gameOver = true;
		}
		if (!entity->IsAlive() && entity->GetEntityType() != EntityType::Player)
			pWorld->entityBuf.DeleteEntityFromBuffer(entity.Get());
		if (!entity->IsAlive() && entity->GetEntityType() == EntityType::FinishMarker)
			gameWon = true;
	}
	pWorld->entitiesList.remove_if([](SmartPointer<Entity>& entity) {
		if (!entity->IsAlive() && ((entity->GetEntityType() != EntityType::Player) && (entity->GetEntityType() != EntityType::FinishMarker)))
			pWorld->entityBuf.DeleteEntityFromBuffer(entity.Get());
		return !entity->IsAlive() && ((entity->GetEntityType() != EntityType::Player) && (entity->GetEntityType() != EntityType::FinishMarker));
		});
}

void CGame::CloseWorld()
{
	countOfPlayers = 0;
	pWorld->SaveWorld();
}

void CGame::DrawLegend()
{
	pWindow->PrintMsg(90, 9, FG_WHITE, L"Легенда:");
	pWindow->PrintMsg(90, 11, FG_CYAN, L"%c -> Вы", (wchar_t)Entity::Direction::Up);
	pWindow->PrintMsg(90, 13, FG_GREEN, L"S -> Змея");
	pWindow->PrintMsg(90, 14, FG_GREEN, L"Бегает по горизонтали");
	pWindow->PrintMsg(90, 16, FG_RED, L"- или | -> Пушка");
	pWindow->PrintMsg(90, 17, FG_RED, L"Периодически стреляет,");
	pWindow->PrintMsg(90, 18, FG_RED, L"нельзя уничтожить");
	pWindow->PrintMsg(90, 20, FG_MAGENTA, L"%c -> Рандомер", L'%');
	pWindow->PrintMsg(90, 21, FG_MAGENTA, L"Бегает в случайном");
	pWindow->PrintMsg(90, 22, FG_MAGENTA, L"направлении в заданной");
	pWindow->PrintMsg(90, 23, FG_MAGENTA, L"области");
	pWindow->PrintMsg(90, 25, FG_YELLOW, L"A -> патроны (+5)");
	pWindow->PrintMsg(90, 27, FG_RED, L"+ -> жизнь (+1)");
	pWindow->PrintMsg(90, 29, FG_BLACK | BG_WHITE, L"C");
	pWindow->PrintMsg(92, 29, FG_WHITE, L" -> контрольная точка");
	pWindow->PrintMsg(90, 31, FG_GREEN, L"%c -> конец уровня", (wchar_t)0x2588);

	pWindow->PrintMsg(90, 40, FG_WHITE, L"Управление:");
	pWindow->PrintMsg(90, 42, FG_BLUE, L"W/A/S/D или %c/%c/%c/%c - ", Entity::Direction::Up, Entity::Direction::Left, Entity::Direction::Down, Entity::Direction::Right);
	pWindow->PrintMsg(90, 43, FG_BLUE, L"перемещение игрока");

	pWindow->PrintMsg(90, 45, FG_CYAN, L"Левая кнопка мыши - стрельба");

	pWindow->PrintMsg(90, 47, FG_WHITE, L"Escape - выход в главное меню");
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
