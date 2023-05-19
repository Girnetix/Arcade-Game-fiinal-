#pragma once
#ifndef EXTRA_ENTITIES_H
#define EXTRA_ENTITIES_H

#include <Engine/Entity.h>
#include "EntitiesTypes.h"

class Ammo : public Entity
{
public:
	Ammo(int x, int y, short color = FG_YELLOW);
	void Update() override;
	void OnCollisionEntity(Entity* target) override;
};

class Life : public Entity
{
public:
	Life(int x, int y, short color = FG_RED);
	void Update() override;
	void OnCollisionEntity(Entity* target) override;
};
class CheckPoint : public Entity
{
public:
	CheckPoint(int x, int y, short color = FG_BLACK | BG_WHITE);
	void Update() override;
	void OnCollisionEntity(Entity* target) override;
};
class FinishMarker : public Entity
{
public:
	FinishMarker(int x, int y, short color = FG_GREEN);
	void Update() override;
	void OnCollisionEntity(Entity* target) override;
};

class Wall : public Entity
{
public:
	Wall(int x, int y, short color = FG_WHITE);
	void Update() override;
	void OnCollisionEntity(Entity* target) override;
};
#endif
