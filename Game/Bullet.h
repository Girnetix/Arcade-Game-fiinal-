#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <Engine/Entity.h>
#include "EntitiesTypes.h"

class Bullet : public Entity
{
public:
	Bullet(int x, int y, short color, Direction eDirection, Entity* owner, double speed);
	Entity* GetOwner();

	void Update() override;
	void OnCollisionEntity(Entity* target) override;
private:
	Entity* owner;
};

#endif
