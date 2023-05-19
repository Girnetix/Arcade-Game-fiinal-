#include "ExtraEntities.h"

Ammo::Ammo(int x, int y, short color):Entity(x, y, color, Direction::Up, 0.0, L"Ammo")
{
	symbol = L'A';
	Movable = false;
	eEntityType = EntityType::Ammo;
}

void Ammo::Update()
{
}

void Ammo::OnCollisionEntity(Entity* target)
{
}

Life::Life(int x, int y, short color) :Entity(x, y, color, Direction::Up, 0.0, L"Life")
{
	symbol = L'+';
	Movable = false;
	eEntityType = EntityType::Life;
}

void Life::Update()
{
}

void Life::OnCollisionEntity(Entity* target)
{
}

CheckPoint::CheckPoint(int x, int y, short color) :Entity(x, y, color, Direction::Up, 0.0, L"CheckPoint")
{
	symbol = L'C';
	Movable = false;
	eEntityType = EntityType::CheckPoint;
}

void CheckPoint::Update()
{
}

void CheckPoint::OnCollisionEntity(Entity* target)
{
}

FinishMarker::FinishMarker(int x, int y, short color) :Entity(x, y, color, Direction::Up, 0.0, L"FinishMarker")
{
	symbol = 0x2588;
	Movable = false;
	eEntityType = EntityType::FinishMarker;
}

void FinishMarker::Update()
{
}

void FinishMarker::OnCollisionEntity(Entity* target)
{
}

Wall::Wall(int x, int y, short color) :Entity(x, y, color, Direction::Up, 0.0, L"Wall")
{
	symbol = L'#';
	Movable = false;
	eEntityType = EntityType::Wall;
}

void Wall::Update()
{
}

void Wall::OnCollisionEntity(Entity* target)
{
}

