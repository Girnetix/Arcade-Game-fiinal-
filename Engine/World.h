#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "Entity.h"

class World
{
public:
	World();
	~World();
	void UpdateWorld(double deltaTime);
private:
	class EntityBuffer
	{
	public:

	private:
		static SmartPointer<Entity* []> entitiesBuf;
	};
	std::list<SmartPointer<Entity>> entitiesList;
};

#endif
