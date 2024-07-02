#include "Randomer.h"
#include "Player.h"

Randomer::Randomer(int x, int y, int minX, int minY, int maxX, int maxY, short color, double speed) : Entity(x, y, color, Direction::Up, speed, L"Randomer")
{
	eEntityType = EntityType::Randomer;
	pNumGenerator->SetRange((uint32_t)Direction::Up, (uint32_t)Direction::Left);
	symbol = '%';
	eDirection = (Direction)pNumGenerator->Randomize();
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
}

void Randomer::OnCollisionEntity(Entity* target)
{
	switch (target->GetEntityType())
	{
		case EntityType::Player:
			target->KillEntity();
			break;

		case EntityType::Bullet:
		{
			Bullet* bullet = (Bullet*)target;
			if (bullet->GetOwner()->GetEntityType() == EntityType::Player)
			{
				Player* player = (Player*)bullet->GetOwner();
				player->AddKill();
			}
			this->KillEntity();
			target->KillEntity();
		}
			break;

		case EntityType::Cannon:
			break;
		case EntityType::Randomer:
			break;
		case EntityType::CheckPoint:
			break;
		case EntityType::Ammo:
			break;
		case EntityType::Life:
			break;
		case EntityType::FinishMarker:
			break;

		default:
			this->KillEntity();
			break;
	}
}

void Randomer::Update()
{
	int failedAttempts = 0;
	bool moveIsLegal = false;
	while (!moveIsLegal && failedAttempts != 10)						//на случай, если рандомер случайно где-то застрянет
	{
		eDirection = (Direction)pNumGenerator->Randomize();
		int testX = x, testY = y;				//тестовые координаты, нужны для того, чтобы проверить будущую позицию на наличие или отсутсвие объектов

		switch (eDirection)
		{
			case Entity::Direction::Up: testY--;		break;
			case Entity::Direction::Down:testY++;		break;
			case Entity::Direction::Left:testX--;		break;
			case Entity::Direction::Right:testX++;		break;
		}

		if (testX > minX && testX < maxX && testY > minY && testY < maxY)
			moveIsLegal = true;
		else
			failedAttempts++;

		if (!moveIsLegal)
			continue;
		Entity* entity = pWorld->GetEntity(testX, testY);
		if (entity != nullptr)
		{
			if (entity->GetEntityType() == EntityType::Player && moveIsLegal)
			{
				x = testX;
				y = testY;
			}
			else if (entity->GetEntityType() == EntityType::Bullet && moveIsLegal)
			{
				x = testX;
				y = testY;
			}
			else
			{
				moveIsLegal = false;
				failedAttempts++;
				continue;
			}
		}
		else
		{
			x = testX;
			y = testY;
		}
	}
}
