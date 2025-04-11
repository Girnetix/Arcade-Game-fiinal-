#include "World.h"
#include "Console.h"
#include <codecvt>

SmartPointer<World> pWorld;
SmartPointer<Entity* []> World::EntityBuffer::entitiesBuf;

World::World()
{
	entitiesCount = 0;
}

World::~World()
{
	SaveWorld();
}

void World::CreateWorld(const std::string& worldname)
{
	file.Create(worldname);
	file.Close();
	file.Open(worldname);
}

void World::SaveWorld()
{
	file.Close();
	entityBuf.EntityBufferClear();
	entitiesList.clear();
	entitiesCount = 0;
	file.Clear();
}

void World::LoadWorld(const std::string& worldname)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wstr = converter.from_bytes(worldname);

	pConsole->CPrintF(L"Opening file \"%s\"...", wstr.c_str());
	CTimerValue tpBefore = pTimer->GetHighPrecisionTime();

	file.Open(worldname);
	if (!file.isOpen())
	{
		pConsole->CPrintF(L"Cannot open file \"%s\"...", wstr.c_str());
		return;
	}

	CTimerValue tpAfter = pTimer->GetHighPrecisionTime();
	pConsole->CPrintF(L"Opened file \"%s\" in %3.3f seconds", wstr.c_str(), (tpAfter - tpBefore).GetSeconds());
	worldName = worldname;
	file >> entitiesCount;
}

void World::UpdateWorld(double deltaTime)
{
	/*for (auto currentEntity = entitiesList.begin(); currentEntity != entitiesList.end(); currentEntity++)
	{
		currentEntity->Get()->UpdateEntity(deltaTime);
		if (!currentEntity->Get()->IsAlive())
		{
			currentEntity = entitiesList.erase(currentEntity);
			continue;
		}
	}*/
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

Entity* World::FindEntity(EntityType eEntityType)
{
	auto pEntity = std::find_if(entitiesList.begin(), entitiesList.end(), [&](SmartPointer<Entity>& entity) {return entity->GetEntityType() == eEntityType; });
	if (pEntity != entitiesList.end())
		return pEntity->Get();

	return nullptr;
}

void World::EntityBuffer::SetEntityToBuffer(Entity* entity)
{
	int x = entity->GetX();
	int y = entity->GetY();
	Entity* target = GetEntityFromBuffer(x, y);
	if (target != nullptr)
	{
		pConsole->CPrintF(L"Detected enitity collision: (current entity type: %s to target entity type: %s)", entity->GetName().c_str(), target->GetName().c_str());
		entity->OnCollisionEntity(target);
	}
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

void World::EntityBuffer::EntityBufferClear()
{
	for (int i = 0; i < pWindow->GetScrWidth() * pWindow->GetScrHeight(); i++)
		entitiesBuf[i] = nullptr;
}
