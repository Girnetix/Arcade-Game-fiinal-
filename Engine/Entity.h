#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Timer.h"
#include "Rendering.h"
#include "Random.h"
#include "Stream.h"
#include "Console.h"

enum class EntityType :uint16_t;
class World;

class Entity
{
public:
	enum class Direction :uint8_t { Up = 24, Down, Right, Left };
public:
	Entity();
	Entity(int x, int y, short color, Direction eDirection, double speed, const std::wstring& name);

	void Create(Entity* entity);
	void Delete(Entity* entity);

	int GetX();
	int GetY();
	uint32_t GetId();
	double GetSpeed();

	void SetX(int x) { this->x = x; }
	void SetY(int y) { this->y = y; }
	void SetSpeed(double speed) { this->speed = speed; if (speed > 0) delay = 1.0 / speed; }
	virtual void SetDirection(Entity::Direction direction) { this->eDirection = direction; }

	wchar_t GetEntitySymbol();
	short GetEntityColor();
	std::wstring& GetName();
	
	bool IsMovable();
	bool IsAlive();
	virtual void KillEntity();

	EntityType GetEntityType();
	Direction GetDirection();

	void UpdateEntity(double deltaTime);

	friend CStream& operator<<(CStream& stream, Entity& entity);
	friend CStream& operator>>(CStream& stream, Entity& entity);

	

public:
	virtual void Update() {};
	virtual void OnCollisionEntity(Entity* target) {};

protected:
	int x, y;
	double speed, delay, time;
	wchar_t symbol;
	short color;
	bool Alive, Movable;
	EntityType eEntityType;
	Direction eDirection;
	uint32_t id;
	std::wstring name;
private:
	static uint32_t counter;
};



#endif
