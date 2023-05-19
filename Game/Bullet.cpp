#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(int x, int y, short color, Direction eDirection, Entity* owner, double speed):Entity(x, y, color, eDirection, speed, L"Bullet")
{
    eEntityType = EntityType::Bullet;
    this->owner = owner;
    symbol = L'O';
    int testX = x, testY = y;
    switch (eDirection)
    {
        case Direction::Up:    testY--;		break;
        case Direction::Down:  testY++;		break;
        case Direction::Left:  testX--;		break;
        case Direction::Right: testX++;		break;
    }
    Entity* entity = pWorld->GetEntity(testX, testY);
    if (entity != nullptr)
    {
        this->OnCollisionEntity(entity);
        return;
    }
    this->x = testX;
    this->y = testY;
}

Entity* Bullet::GetOwner()
{
    return owner;
}

void Bullet::Update()   
{
    int testX = x, testY = y;
    switch (eDirection)
    {
        case Direction::Up:    testY--;		break;
        case Direction::Down:  testY++;		break;
        case Direction::Left:  testX--;		break;
        case Direction::Right: testX++;		break;
    }
    Entity* entity = pWorld->GetEntity(testX, testY);
    if (entity != nullptr)
    {
        this->OnCollisionEntity(entity);
        return;
    }
    x = testX;
    y = testY;
}

void Bullet::OnCollisionEntity(Entity* target)
{
    switch (target->GetEntityType())
    {
        case EntityType::Player:         
            this->KillEntity();
            target->KillEntity();
            break;
        case EntityType::Runner:
        {
            if (this->GetOwner()->GetEntityType() == EntityType::Player)
            {
                Player* player = (Player*)this->GetOwner();
                player->AddKill();
            }
            this->KillEntity();
            target->KillEntity();
        }
            break;
        case EntityType::Randomer:
        {
            if (this->GetOwner()->GetEntityType() == EntityType::Player)
            {
                Player* player = (Player*)this->GetOwner();
                player->AddKill();
            }
            this->KillEntity();
            target->KillEntity();
        }
            break;
        case EntityType::Cannon:         this->KillEntity();     break;
        case EntityType::MovableEntity:  this->KillEntity();     break;
        case EntityType::CheckPoint:     this->KillEntity();     break;
        case EntityType::Ammo:           this->KillEntity();     break;
        case EntityType::Life:           this->KillEntity();     break;
        case EntityType::Wall:           this->KillEntity();     break;

        default:
                this->KillEntity();
            break;
    }
}
