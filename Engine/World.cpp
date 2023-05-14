#include "World.h"

SmartPointer<World> pWorld = nullptr;
SmartPointer<Entity* []> World::EntityBuffer::entitiesBuf = nullptr;

World::World()
{
	worldWidth = worldHeight = 0;
	entitiesCount = 0;
}

World::~World()
{
	SaveWorld();
}

void World::CreateWorld(const std::string& worldname)
{

}

void World::SaveWorld()
{

	file.Close();
}

void World::LoadWorld(const std::string& worldname)
{
	file.Open(worldname);
	if (!file.isOpen())
		return;
	worldName = worldname;
	file >> worldWidth >> worldHeight;
	file >> entitiesCount;
}

void World::UpdateWorld(double deltaTime)
{
	for (auto& currentEntity : entitiesList)
		currentEntity->UpdateEntity(deltaTime);
	entitiesList.remove_if([](SmartPointer<Entity> &entity) { return !entity->IsAlive(); });
}

void World::CreateEntity(Entity* entity)
{
	entitiesList.push_back(entity);
	entityBuf.SetEntityToBuffer(entity);
}

void World::DestroyEntity(Entity* entity)
{
	entity->KillEntity();
	entityBuf.DeleteEntityFromBuffer(entity);
}

Entity* World::GetEntity(int x, int y)
{
	return entityBuf.GetEntityFromBuffer(x, y);
}

void World::EntityBuffer::SetEntityToBuffer(Entity* entity)
{
	int x = entity->GetX();
	int y = entity->GetY();
	Entity* target = GetEntityFromBuffer(x, y);
	if (target != nullptr)
		entity->OnCollisionEntity(target);
	else
		entitiesBuf[y * pWindow->GetScrWidth() + x] = entity;
}

void World::EntityBuffer::DeleteEntityFromBuffer(Entity* entity)
{
	int x = entity->GetX();
	int y = entity->GetY();
	entitiesBuf[y * pWindow->GetScrWidth() + x] = nullptr;
}

Entity* World::EntityBuffer::GetEntityFromBuffer(int x, int y)
{
	return entitiesBuf[y * pWindow->GetScrWidth() + x];
}
