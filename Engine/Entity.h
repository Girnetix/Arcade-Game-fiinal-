#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Timer.h"
#include "Rendering.h"
#include "PRNG.h"
#include "Stream.h"

enum class EntityType :uint16_t;
class World;

class Entity
{
public:
	enum class Direction :uint8_t { Up = 24, Down, Left, Right };
public:
	Entity();
	Entity(int x, int y, short color, Direction eDirection, double speed, const std::wstring& name);

	void Create(Entity* entity);
	void Delete(Entity* entity);

	int GetX();
	int GetY();
	uint32_t GetId();

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
	virtual void Update() = 0;
	virtual void OnCollisionEntity(Entity* target) = 0;

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
