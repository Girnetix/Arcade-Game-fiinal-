#pragma once
#ifndef ENTITIES_H
#define ENTITIES_H

#include <Engine/Entity.h>
#include <Engine/World.h>

enum class EntityType :uint16_t
{
	Player, 
	Bullet, 
	Cannon, 
	Runner, 
	Randomer, 
	Chaser, 
	MovableEntity, 
	CheckPoint, 
	Ammo, 
	Life,
	Wall
};

#endif
