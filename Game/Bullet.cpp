#include "Bullet.h"

Bullet::Bullet(int x, int y, short color, Direction eDirection, Entity* owner, double speed):Entity(x, y, color, eDirection, speed, L"Bullet")
{
    eEntityType = EntityType::Bullet;
    this->owner = owner;
    symbol = L'O';

    switch (eDirection)
    {
        case Direction::Up:    this->y--;		break;
        case Direction::Down:  this->y++;		break;
        case Direction::Left:  this->x--;		break;
        case Direction::Right: this->x++;		break;
    }

    Create(this);
}

Entity* Bullet::GetOwner()
{
    return owner;
}

void Bullet::Update()   
{
   //меняем координаты в зависимости от направления
    switch (eDirection)
    {
        case Direction::Up:		y--; break;
        case Direction::Down:	y++; break;
        case Direction::Left:	x--; break;
        case Direction::Right:	x++; break;
    }

}

void Bullet::OnCollisionEntity(Entity* target)
{
    switch (target->GetEntityType())
    {
        case EntityType::Cannon:         this->KillEntity();     break;
        case EntityType::MovableEntity:  this->KillEntity();     break;
        case EntityType::CheckPoint:     this->KillEntity();     break;
        case EntityType::Ammo:           this->KillEntity();     break;
        case EntityType::Life:           this->KillEntity();     break;
        case EntityType::Wall:           this->KillEntity();     break;

        default:
            this->KillEntity();
            target->KillEntity();
            break;
    }
}
