#include "Engine.h"

void CEngine::Start(int iScrWidth, int iScrHeight, int iFontX, int iFontY, const std::wstring& font, const std::wstring& AppName)
{
	pConsole = new CConsole();
	pWindow = new Window();
	pTimer = new Timer();
	pWorld = new World();
	pNumGenerator = new default_number_generator();
	pWindow->ConstructWindow(iScrWidth, iScrHeight, iFontX, iFontY, font, AppName);
	//если всё запущено, то устанавливаем флаг о том, что движок запущен
	if (pConsole.Get() && pWindow.Get() && pTimer.Get() && pWorld.Get() && pNumGenerator.Get())
	{
		bIsRunning = true;
		GameThread();
	}
}

void CEngine::Stop()
{
	pWindow.reset();
	pTimer.reset();
	pConsole.reset();
	pWorld.reset();
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
		Keyboard::UpdateKeyboard();

		double deltaTime = pTimer->GetDeltaTimeSec();
		fpsTime += deltaTime;
		fps++;
		if (fpsTime >= 1.0)
		{
			fpsTime -= 1.0;
			wchar_t buf[256]{};
			swprintf_s(buf, 256, L"FPS: %d FPS(instant): %3.2f", fps, 1.0 / deltaTime);
			pWindow->ChangeAppNme(buf);
			fps = 0;
		}

		if (!OnUserUpdate(deltaTime))
			bIsRunning = false;
		
		pWindow->UpdateWindow();
	}

	OnUserDestroy();
}
