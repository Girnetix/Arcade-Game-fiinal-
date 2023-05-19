#include "Entity.h"
#include "World.h"

uint32_t Entity::counter = 0;

Entity::Entity()
{
	x = y = color = 0;
	delay = speed = time = 0.0;
	eDirection = Direction::Up;
	Alive = Movable = true;
	symbol = L' ';
	name = L"";
	id = counter++;
}

Entity::Entity(int x, int y, short color, Direction eDirection, double speed, const std::wstring& name) :Entity()
{
	this->x = x;
	this->y = y;
	this->color = color;
	this->eDirection = eDirection;
	this->name = name;
	this->speed = speed;
	if (speed > 0.0)
		delay = 1.0 / speed;
}

void Entity::Create(Entity* entity)
{
	if (entity->Alive)
	{
		pWorld->CreateEntity(entity);
		pWindow->PrintSymbol(entity->x, entity->y, entity->symbol, entity->color);
	}
}

void Entity::Delete(Entity* entity)
{
	pWorld->DestroyEntity(entity);
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

double Entity::GetSpeed()
{
	return speed;
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

bool Entity::IsMovable()
{
	return Movable;
}

bool Entity::IsAlive()
{
	return Alive;
}

void Entity::KillEntity()
{
	Alive = false;
}

EntityType Entity::GetEntityType()
{
	return eEntityType;
}

Entity::Direction Entity::GetDirection()
{
	return eDirection;
}

void Entity::UpdateEntity(double deltaTime)
{
	//если объект не двигается и он жив, то просто отрисовываем его
	if (!Movable && Alive)
	{
		pWindow->PrintSymbol(x, y, symbol, color);
		return;
	}
	time += deltaTime;
	if (time < delay)			//если время обновления не прошло, то просто отрисовываем объект
	{
		if (Alive)		//если объект жив, то отрисовываем его после обновления
			pWindow->PrintSymbol(x, y, symbol, color);
		return;
	}
		
	time -= delay;

	//удаляем сущность из всего буфера и обновляем объект
	pWorld->entityBuf.DeleteEntityFromBuffer(this);
	Update();

	//возвращаем объект в буфер
	pWorld->entityBuf.SetEntityToBuffer(this);

	if (Alive)		//если объект жив, то отрисовываем его после обновления
		pWindow->PrintSymbol(x, y, symbol, color);
}

CStream& operator<<(CStream& stream, Entity& entity)
{
	stream.Write(&entity.eEntityType, sizeof(EntityType));
	stream.Write(&entity.eDirection, sizeof(Entity::Direction));

	stream << entity.x << entity.y
		   << entity.speed << entity.delay << entity.time
		   << entity.symbol << entity.color << entity.Alive
		   << entity.Movable << entity.id << entity.name;
	return stream;
}

CStream& operator>>(CStream& stream, Entity& entity)
{
	stream.Read(&entity.eEntityType, sizeof(EntityType));
	stream.Read(&entity.eDirection, sizeof(Entity::Direction));

	stream >> entity.x >> entity.y
		>> entity.speed >> entity.delay >> entity.time
		>> entity.symbol >> entity.color >> entity.Alive
		>> entity.Movable >> entity.id >> entity.name;
	return stream;
}
