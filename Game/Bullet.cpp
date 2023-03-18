#include "Bullet.h"

Bullet::Bullet(int x, int y, short color, Direction eDirection, Entity* owner, double speed):Entity(x, y, color, eDirection, speed)
{
    eEntityType = EntityType::Bullet;
    this->owner = owner;
}

Entity* Bullet::GetOwner()
{
    return owner;
}

void Bullet::Update(double deltaTime)
{

}

void Bullet::OnCollisionEntity(Entity* target)
{

}
