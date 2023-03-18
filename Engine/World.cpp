#include "World.h"

SmartPointer<Entity* []> World::EntityBuffer::entitiesBuf = nullptr;

World::World()
{
}

World::~World()
{

}

void World::UpdateWorld(double deltaTime)
{
	for (auto& currentEntity : entitiesList)
	{
		currentEntity->Update(deltaTime);
	}
}
