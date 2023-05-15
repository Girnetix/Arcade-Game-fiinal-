#include "Runner.h"

Runner::Runner(int x, int y, short color, Direction eDirection, double speed) : Entity(x, y, color, eDirection, speed, L"Runner")
{
	eEntityType = EntityType::Runner;
	symbol = 'S';
	Create(this);
}

void Runner::OnCollisionEntity(Entity* target)
{
	switch (target->GetEntityType())
	{
		case EntityType::Player:
		case EntityType::Bullet:
		case EntityType::Cannon:
		case EntityType::Runner:
		case EntityType::Randomer:
		case EntityType::Chaser:
		case EntityType::MovableEntity:
		case EntityType::CheckPoint:
		case EntityType::Ammo:
		case EntityType::Life:
		case EntityType::Wall:
		default:
			break;
	}
}

void Runner::Update()
{
	switch (eDirection)
	{
		case Direction::Left:  x--; break;
		case Direction::Right: x++; break;
	}
}
