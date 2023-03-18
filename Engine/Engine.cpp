#include "Engine.h"

void CEngine::Start(int iScrWidth, int iScrHeight, std::wstring AppName)
{
	pWindow = new Window();
	pTimer = new Timer();

	pWindow->ConstructWindow(iScrWidth, iScrHeight, AppName);
	if (pWindow.Get() && pTimer.Get())		//если всё запущено, то устанавливаем флаг о том, что движок запущен
	{
		bIsRunning = true;
		GameThread();
	}
}

void CEngine::Stop()
{
	pWindow.reset();
	pTimer.reset();
}

void CEngine::GameThread()
{
	if (!OnUserCreate())
		bIsRunning = false;

	int fps = 0;
	double fpsTime = 0.0;
	pTimer->ResetTimer();

	while (bIsRunning)
	{
		pTimer->UpdateTimer();
		pWindow->ClearAllWindow();

		double deltaTime = pTimer->GetDeltaTimeSec();
		fpsTime += deltaTime;
		fps++;
		if (fpsTime >= 1.0)
		{
			fpsTime -= 1.0;
			pWindow->ChangeAppNme(L"FPS: " + std::to_wstring(fps) + L" FPS(instant): " + std::to_wstring(1.0f / deltaTime));
			fps = 0;
		}

		if (!OnUserUpdate(deltaTime))
			bIsRunning = false;

		pWindow->UpdateWindow();
	}

	OnUserDestroy();
}
