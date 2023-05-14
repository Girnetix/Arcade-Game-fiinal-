#include "Bullet.h"

Bullet::Bullet(int x, int y, short color, Direction eDirection, Entity* owner, double speed):Entity(x, y, color, eDirection, speed, L"Bullet")
{
    eEntityType = EntityType::Bullet;
    this->owner = owner;
}

Entity* Bullet::GetOwner()
{
    return owner;
}

void Bullet::Update()
{
    switch (eDirection)
    {
        case Entity::Direction::Up:

            break;
        case Entity::Direction::Down:

            break;
        case Entity::Direction::Left:

            break;
        case Entity::Direction::Right:

            break;
    }
}

void Bullet::OnCollisionEntity(Entity* target)
{
    switch (target->GetEntityType())
    {
        case EntityType::Player:
            break;
        case EntityType::Bullet:
            break;
        case EntityType::Cannon:
            break;
        case EntityType::Runner:
            break;
        case EntityType::Randomer:
            break;
        case EntityType::Chaser:
            break;
        case EntityType::MovableEntity:
            break;
        case EntityType::CheckPoint:
            break;
        case EntityType::Ammo:
            break;
        case EntityType::Life:
            break;
        case EntityType::Wall:
            break;
        default:
            break;
    }
}
