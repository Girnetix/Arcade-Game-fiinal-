#include "Player.h"

Player::Player(int x, int y, short color, Direction eDirection, double speed, const std::wstring& name) :Entity(x, y, color, eDirection, speed, name)
{
	xRespawn = x;
	yRespawn = y;
	symbol = (char)eDirection;
	directionRespawn = eDirection;
	eEntityType = EntityType::Player;
	this->name = name;
	Create(this);
}

void Player::Update()
{
	if (!Respawning)
	{
		if (Keyboard::GetKey(MOUSE_LBUTTON).bPressed)
			Shoot();
		if (Keyboard::GetKey(Key_W).bPressed || Keyboard::GetKey(UP).bPressed)
			MoveUp();
		if (Keyboard::GetKey(Key_S).bPressed || Keyboard::GetKey(DOWN).bPressed)
			MoveDown();
		if (Keyboard::GetKey(Key_A).bPressed || Keyboard::GetKey(LEFT).bPressed)
			MoveLeft();
		if (Keyboard::GetKey(Key_D).bPressed || Keyboard::GetKey(UP).bPressed)
			MoveRight();
		symbol = (char)eDirection;
	}
	else
	{
		timeForRespawning -= pTimer->GetDeltaTimeSec();
		pWindow->PrintMsgRightSide(10, FG_WHITE, L"Возрождение через: %3.1f", timeForRespawning);
		if (timeForRespawning <= 0.0)
		{
			Respawning = false;
			x = xRespawn;
			y = yRespawn;
			eDirection = directionRespawn;
		}
	}
}

void Player::OnCollisionEntity(Entity* target)
{
	switch (target->GetEntityType())
	{
		case EntityType::Bullet:
			this->KillEntity();
			target->KillEntity();
			break;

		case EntityType::Runner:
			this->KillEntity();
			break;

		case EntityType::Randomer:
			this->KillEntity();
			break;

		case EntityType::Chaser:
			this->KillEntity();
			break;

		case EntityType::CheckPoint:
			target->KillEntity();
			xRespawn = x;
			yRespawn = y;
			directionRespawn = eDirection;
			break;

		case EntityType::Ammo:
			target->KillEntity();
			countOfBullets += 5;
			break;

		case EntityType::Life:
			target->KillEntity();
			lifes++;
			break;

		case EntityType::FinishMarker:
			target->KillEntity();
			break;

		default:
			this->KillEntity();
			break;
	}
}

void Player::KillEntity()
{
	Respawning = true;
	timeForRespawning = timeRespawn;
	lifes--;
}

void Player::Shoot()
{
	if (countOfBullets > 0)
	{
		countOfBullets--;
		Create(new Bullet(x, y, color, eDirection, this, 10.0));
	}
}

void Player::MoveUp()
{
	eDirection = Direction::Up;
	EntityType entTy = pWorld->GetEntity(x, y - 1)->GetEntityType();
	if (entTy == EntityType::Cannon || entTy == EntityType::Wall)
		return;
	y--;
}

void Player::MoveDown()
{
	eDirection = Direction::Down;
	EntityType entTy = pWorld->GetEntity(x, y + 1)->GetEntityType();
	if (entTy == EntityType::Cannon || entTy == EntityType::Wall)
		return;
	y++;
}

void Player::MoveLeft()
{
	eDirection = Direction::Left;
	EntityType entTy = pWorld->GetEntity(x - 1, y)->GetEntityType();
	if (entTy == EntityType::Cannon || entTy == EntityType::Wall)
		return;
	x--;
}

void Player::MoveRight()
{
	eDirection = Direction::Right;
	EntityType entTy = pWorld->GetEntity(x + 1, y)->GetEntityType();
	if (entTy == EntityType::Cannon || entTy == EntityType::Wall)
		return;
	x++;
}
