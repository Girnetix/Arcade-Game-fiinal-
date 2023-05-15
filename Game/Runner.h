#pragma once
#ifndef RUNNER_H
#define RUNNER_H

#include <Engine/Entity.h>
#include "EntitiesTypes.h"

class Runner : public Entity
{
public:
	Runner(int x, int y, short color, Direction eDirection, double speed);
	void OnCollisionEntity(Entity* target) override;
private:
	void Update() override;
};

#endif