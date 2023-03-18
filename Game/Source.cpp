#include <Engine/Engine.h>
#include "Bullet.h"

class CGame :public CEngine
{
public:
	bool OnUserCreate() override;
	bool OnUserUpdate(double deltaTime) override;
	void OnUserDestroy() override;
private:
	CTimerValue tp1, tp2;
	double gameTime = 0.0;
	wchar_t msg[40]{};
};

bool CGame::OnUserCreate()
{
	Bullet bullet(0, 0, FG_BLUE, Entity::Direction::Up, nullptr, 5.0);
	Entity* entity = &bullet;
	tp1 = pTimer->GetHighPrecisionTime();
	entity->Update(0.5);
	pTimer->SetTimer(0.0, 5.0, 0.0, [&]() {bIsRunning = false;  });
	return true;
}

bool CGame::OnUserUpdate(double deltaTime)
{
	gameTime += deltaTime;
	int sec = gameTime;
	int min = sec / 60;
	int hour = min / 60;
	swprintf_s(msg, 40, L"Time: %02d:%02d:%02d", hour, min % 60, sec % 60);
	pWindow->PrintMsg(0, 0, msg, FG_CYAN);
	return true;
}

void CGame::OnUserDestroy()
{
	tp2 = pTimer->GetHighPrecisionTime();
	wchar_t buf[40]{};
	swprintf_s(buf, 40, L"Working time: %3.5f msec.", (tp2 - tp1).GetMilliseconds());
	pWindow->PrintMsg(0, 1, buf);
	pWindow->UpdateWindow();
}

int main()
{
	Entity* entity = nullptr;
	CGame game;
	game.Start(120, 50, L"Test");
	return 0;
}