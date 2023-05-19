#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "Entity.h"

class World
{
public:
	World();
	~World();
	void CreateWorld(const std::string& worldname);
	void SaveWorld();
	void LoadWorld(const std::string &worldname);
	void UpdateWorld(double deltaTime);
	void CreateEntity(Entity* entity);
	void DestroyEntity(Entity* entity);
	Entity* GetEntity(int x, int y);
public:
	class EntityBuffer
	{
	public:
		EntityBuffer() { 
			entitiesBuf = new Entity * [pWindow->GetScrWidth() * pWindow->GetScrHeight()];
			for (int i = 0; i < pWindow->GetScrWidth() * pWindow->GetScrHeight(); i++)
				entitiesBuf[i] = nullptr;
		}
		~EntityBuffer() { entitiesBuf.reset(); }
		void SetEntityToBuffer(Entity *entity);
		void DeleteEntityFromBuffer(Entity* entity);
		Entity* GetEntityFromBuffer(int x, int y);
	private:
		static SmartPointer<Entity* []> entitiesBuf;
	};
	EntityBuffer entityBuf;
public:
	std::list<SmartPointer<Entity>> entitiesList;
	CFileStream file;
	size_t entitiesCount;
	std::string worldName;
};

extern SmartPointer<World> pWorld;

#endif
