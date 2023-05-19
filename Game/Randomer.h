#pragma once
#ifndef RANDOMER_H
#define RANDOMER_H

#include <Engine/Entity.h>
#include "EntitiesTypes.h"

class Randomer : public Entity
{
public:
	Randomer(int x, int y, int minX, int minY, int maxX, int maxY, short color, double speed);
	void OnCollisionEntity(Entity* target) override;
	int GetMinX() { return minX; }
	int GetMinY() { return minY; }
	int GetMaxX() { return maxX; }
	int GetMaxY() { return maxY; }
private:
	void Update() override;
	int minX = 0, maxX = 0, minY = 0, maxY = 0;
};

#endif