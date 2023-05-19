#include "Cannon.h"

Cannon::Cannon(int x, int y, short color, Direction eDirection, double speed) :Entity(x, y, color, eDirection, speed, L"Cannon")
{
	eEntityType = EntityType::Cannon;
	switch (eDirection)
	{
		case Direction::Up:		symbol = '|'; break;
		case Direction::Down:	symbol = '|'; break;
		case Direction::Left:	symbol = '-'; break;
		case Direction::Right:	symbol = '-'; break;
	}
}

void Cannon::OnCollisionEntity(Entity* target)
{
	return;
}

void Cannon::Update()
{
	Create(new Bullet(x, y, color, eDirection, this, 20.0));
}

