#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Timer.h"
#include "Rendering.h"
#include "PRNG.h"

enum class EntityType;
class World;

class Entity
{
public:
	enum class Direction :uint8_t { Up = 24, Down, Left, Right };
public:
	Entity();
	Entity(int x, int y, short color, Direction eDirection, double speed, const std::wstring& name = L"");

	int GetX();
	int GetY();
	uint32_t GetId();

	wchar_t GetEntitySymbol();
	short GetEntityColor();
	std::wstring& GetName();
	
	bool IsAlive();

	EntityType GetEntityType();
	Direction GetDirection();

	virtual void Update(double deltaTime) = 0;
	virtual void OnCollisionEntity(Entity* target) = 0;

protected:
	int x, y;
	double speed, delay, time;
	World* currentWorld;
	wchar_t symbol;
	short color;
	bool isAlive;
	EntityType eEntityType;
	Direction eDirection;
	uint32_t id;
	std::wstring name;
private:
	static uint32_t counter;
};



#endif
