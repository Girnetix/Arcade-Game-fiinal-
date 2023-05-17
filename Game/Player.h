#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <Engine/Engine.h>
#include "EntitiesTypes.h"
#include "Bullet.h"

class Player :public Entity
{
public:
	Player(int x, int y, short color, Direction eDirection, double speed, const std::wstring& name);
	void Update() override;
	void OnCollisionEntity(Entity* target) override;
	void KillEntity() override;
	int Lifes() { return lifes; }
private:
	void Shoot();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
private:
	bool Respawning = false;
	double timeRespawn = 5.0;					//время возрождения в секундах
	double timeForRespawning = 0.0;
	int countOfBullets = 10;
	int maxCountOfBullets = 30;
	int lifes = 3;
	int xRespawn, yRespawn;
	Direction directionRespawn;
};

#endif
