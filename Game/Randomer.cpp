#include "Randomer.h"

Randomer::Randomer(int x, int y, int minX, int minY, int maxX, int maxY, short color, double speed) : Entity(x, y, color, Direction::Up, speed, L"Randomer")
{
	eEntityType = EntityType::Randomer;
	pNumGenerator->SetRange((uint32_t)Direction::Up, (uint32_t)Direction::Right);
	symbol = '%';
	eDirection = (Direction)pNumGenerator->Randomize();
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	Create(this);
}

void Randomer::OnCollisionEntity(Entity* target)
{
	switch (target->GetEntityType())
	{
		case EntityType::Player:
			target->KillEntity();
			break;

		case EntityType::Bullet:
			target->KillEntity();
			this->KillEntity();
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
	while (!moveIsLegal || failedAttempts != 10)						//на случай, если рандомер случайно где-то застрянет
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

		if (pWorld->GetEntity(testX, testY)->GetEntityType() == EntityType::Wall && moveIsLegal)
		{
			moveIsLegal = false;
			failedAttempts++;
		}
		else
		{
			x = testX;
			y = testY;
		}
	}
}
