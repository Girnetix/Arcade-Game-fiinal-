#include "Runner.h"
#include "Player.h"

Runner::Runner(int x, int y, short color, Direction eDirection, double speed) : Entity(x, y, color, eDirection, speed, L"Runner")
{
	eEntityType = EntityType::Runner;
	symbol = 'S';
}

void Runner::OnCollisionEntity(Entity* target)
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

		case EntityType::Runner:
			break;

		default:
			this->ChangeDirection();
			break;
	}
}

void Runner::Update()
{
	int testX = x;

	switch (eDirection)
	{
		case Direction::Left:  testX--; break;
		case Direction::Right: testX++; break;
	}
	Entity* entity = pWorld->GetEntity(testX, y);
	if (entity != nullptr)
	{
		if (entity->GetEntityType() == EntityType::Wall)
			ChangeDirection();
		else
			x = testX;
	}
	else
		x = testX;
}

void Runner::ChangeDirection()
{
	switch (eDirection)
	{
		case Direction::Left:
			eDirection = Direction::Right;
			x++; 
			break;

		case Direction::Right:
			eDirection = Direction::Left;
			x--;
			break;
	}	
}
