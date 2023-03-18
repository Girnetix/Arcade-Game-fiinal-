#include "Entity.h"
#include "World.h"

uint32_t Entity::counter = 0;

Entity::Entity()
{
	x = y = color = 0;
	delay = speed = time = 0.0;
	eDirection = Direction::Up;
	isAlive = true;
	symbol = L' ';
	name = L"";
	currentWorld = nullptr;
	id = counter++;
}

Entity::Entity(int x, int y, short color, Direction eDirection, double speed, const std::wstring& name) :Entity()
{
	this->x = x;
	this->y = y;
	this->color = color;
	this->eDirection = eDirection;
	this->name = name;
	delay = 1.0 / speed;
}

int Entity::GetX()
{
	return x;
}

int Entity::GetY()
{
	return y;
}

uint32_t Entity::GetId()
{
	return id;
}

wchar_t Entity::GetEntitySymbol()
{
	return symbol;
}

short Entity::GetEntityColor()
{
	return color;
}

std::wstring& Entity::GetName()
{
	return name;
}

bool Entity::IsAlive()
{
	return isAlive;
}

EntityType Entity::GetEntityType()
{
	return eEntityType;
}

Entity::Direction Entity::GetDirection()
{
	return eDirection;
}
