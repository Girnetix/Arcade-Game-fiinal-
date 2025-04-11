#include "Player.h"

Player::Player(int x, int y, short color, Direction eDirection, double speed, const std::wstring& name) :Entity(x, y, color, eDirection, speed, name)
{
	xRespawn = x;
	yRespawn = y;
	symbol = (wchar_t)eDirection;
	directionRespawn = eDirection;
	eEntityType = EntityType::Player;
	this->speed = 0.0;
	this->delay = 0.0;
	moveSpeedTp1 = moveSpeedTp2 = shootSpeedTp1 = shootSpeedTp2 = pTimer->GetHighPrecisionTime();
	this->name = name;
}

void Player::Update()
{
	if (!Respawning)
	{
		moveSpeedTp1 = shootSpeedTp1 = pTimer->GetHighPrecisionTime();
		if (Keyboard::GetKey(MOUSE_LBUTTON).bHeld)
			if ((shootSpeedTp1 - shootSpeedTp2).GetSeconds() >= shootingSpeed)
			{
				Shoot();
				shootSpeedTp2 = shootSpeedTp1;
			}
			
		if (Keyboard::GetKey(Key_W).bHeld || Keyboard::GetKey(UP).bHeld)
			if ((moveSpeedTp1 - moveSpeedTp2).GetSeconds() >= movementSpeed)
			{
				MoveUp();
				moveSpeedTp2 = moveSpeedTp1;
			}
		if (Keyboard::GetKey(Key_S).bHeld || Keyboard::GetKey(DOWN).bHeld)
			if ((moveSpeedTp1 - moveSpeedTp2).GetSeconds() >= movementSpeed)
			{
				MoveDown();
				moveSpeedTp2 = moveSpeedTp1;
			}
		if (Keyboard::GetKey(Key_A).bHeld || Keyboard::GetKey(LEFT).bHeld)	
			if ((moveSpeedTp1 - moveSpeedTp2).GetSeconds() >= movementSpeed)
			{
				MoveLeft();
				moveSpeedTp2 = moveSpeedTp1;
			}
		if (Keyboard::GetKey(Key_D).bHeld || Keyboard::GetKey(RIGHT).bHeld)	
			if ((moveSpeedTp1 - moveSpeedTp2).GetSeconds() >= movementSpeed)
			{
				MoveRight();
				moveSpeedTp2 = moveSpeedTp1;
			}

		symbol = (char)eDirection;
	}
	else
	{
		timeForRespawning -= pTimer->GetDeltaTimeSec();
		pWindow->PrintMsgRightSide(1, FG_CYAN, Translate(L"Respawning in: %d"), (int)(timeForRespawning + 1));
		if (timeForRespawning <= 0.0)
		{
			color = FG_CYAN;
			Respawning = false;
			x = xRespawn;
			y = yRespawn;
			eDirection = directionRespawn;
		}
	}
}

void Player::OnCollisionEntity(Entity* target)
{
	pConsole->CPrintF(L"Detected enitity collision: (current entity type: %s to target entity type: %s)", this->GetName().c_str(), target->GetName().c_str());
	switch (target->GetEntityType())
	{
		case EntityType::Bullet:
		{
			Bullet* bullet = (Bullet*)target;
			if ((Entity*)this != bullet->GetOwner())
			{
				this->KillEntity();
				target->KillEntity();
			}
		}	
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
			if (countOfBullets > maxCountOfBullets)
				countOfBullets = maxCountOfBullets;
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
	if (Respawning)
		return;
	deaths++;
	color = FG_RED;
	Respawning = true;
	timeForRespawning = timeRespawn;
	lifes--;
}

void Player::PrintInfo()
{
	pWindow->PrintMsg(90, 3, FG_RED, Translate(L"Lives: %d"), lifes);
	pWindow->PrintMsg(90, 4, FG_YELLOW, Translate(L"Ammo: %d/%d"), countOfBullets, maxCountOfBullets);
	pWindow->PrintMsg(0, 43, FG_CYAN, Translate(L"Current position: (x: %d; y: %d)"), x, y);
	pWindow->PrintMsg(0, 44, FG_CYAN, Translate(L"Respawning position: (x: %d; y: %d)"), xRespawn, yRespawn);
}

void Player::Shoot()
{
	if (countOfBullets > 0)
	{
		countOfBullets--;
		Create(new Bullet(x, y, color, eDirection, this, 15.0));
	}
}

void Player::MoveUp()
{
	eDirection = Direction::Up;
	Entity* entity = pWorld->GetEntity(x, y - 1);
	if (entity != nullptr)
	{
		EntityType entTy = entity->GetEntityType();
		if (entTy == EntityType::Cannon || entTy == EntityType::Wall)
			return;
	}	
	y--;
}

void Player::MoveDown()
{
	eDirection = Direction::Down;
	Entity* entity = pWorld->GetEntity(x, y + 1);
	if (entity != nullptr)
	{
		EntityType entTy = entity->GetEntityType();
		if (entTy == EntityType::Cannon || entTy == EntityType::Wall)
			return;
	}
	y++;
}

void Player::MoveLeft()
{
	eDirection = Direction::Left;
	Entity* entity = pWorld->GetEntity(x - 1, y);
	if (entity != nullptr)
	{
		EntityType entTy = entity->GetEntityType();
		if (entTy == EntityType::Cannon || entTy == EntityType::Wall)
			return;
	}
	x--;
}

void Player::MoveRight()
{
	eDirection = Direction::Right;
	Entity* entity = pWorld->GetEntity(x + 1, y);
	if (entity != nullptr)
	{
		EntityType entTy = entity->GetEntityType();
		if (entTy == EntityType::Cannon || entTy == EntityType::Wall)
			return;
	}
	x++;
}
