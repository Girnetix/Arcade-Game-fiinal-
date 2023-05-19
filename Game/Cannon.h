#pragma once
#ifndef CANNON_H
#define CANNON_H

#include <Engine/Entity.h>
#include "EntitiesTypes.h"
#include "Bullet.h"

class Cannon : public Entity
{
public:
	Cannon(int x, int y, short color, Direction eDirection, double speed);
	void OnCollisionEntity(Entity* target) override;
	void Update() override;
	void SetDirection(Entity::Direction direction) override
	{ 
		this->eDirection = direction; 
		switch (eDirection)
		{
			case Direction::Up:		symbol = '|'; break;
			case Direction::Down:	symbol = '|'; break;
			case Direction::Left:	symbol = '-'; break;
			case Direction::Right:	symbol = '-'; break;
		}
	}
};

#endif