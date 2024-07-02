#include "Engine.h"

void CEngine::Start(int iScrWidth, int iScrHeight, int iFontX, int iFontY, const std::wstring& font, const std::wstring& AppName, int maxFPS)
{
	pConsole = new CConsole();
	pWindow = new Window();
	pWindow->ConstructWindow(iScrWidth, iScrHeight, iFontX, iFontY, font, AppName);
	pTimer = new Timer();
	pWorld = new World();
	pNumGenerator = new default_number_generator();
	appName = AppName;
	if(maxFPS > 0)
	{
		this->maxFrameTime = 1000.0 / (double)maxFPS;
		this->maxFPS = maxFPS;
	}
	//если всё запущено, то устанавливаем флаг о том, что движок запущен
	if (pConsole.Get() && pWindow.Get() && pTimer.Get() && pWorld.Get() && pNumGenerator.Get())
	{
		bIsRunning = true;
		GameThread();
	}
}

void CEngine::Stop()
{
	pNumGenerator.reset();
	pWorld.reset();
	pTimer.reset();
	pWindow.reset();
	pConsole.reset();	
}

void CEngine::GameThread()
{
	if (!OnUserCreate())
		bIsRunning = false;

	double deltaTime = 0.0;
	int ticks = 0, lastFPS = 0;
	double fpsTime = 0.0, avgFPS = 0.0, avgFrameTime = 0.0;
	double timeFactor = 1.0;

	pWindow->ChangeAppName(appName);
	pTimer->ResetTimer();

	while (bIsRunning)
	{
		pTimer->UpdateTimer();
		pWindow->ClearAllWindow();
		Keyboard::UpdateKeyboard();

		if (Keyboard::GetKey(EQUALS).bPressed)
		{
			timeFactor *= 2;
			if (timeFactor > 128)
				timeFactor = 128;
		}

		if (Keyboard::GetKey(MINUS).bPressed)
		{
			timeFactor /= 2;
			if (timeFactor < 0.25)
				timeFactor = 0.25;
		}

		deltaTime = pTimer->GetDeltaTimeSec() * timeFactor;

		/*fpsTime += deltaTime;
		ticks++;
		if (fpsTime >= 1.0)
		{
			lastFPS = ticks;
			fpsTime -= 1.0;
			ticks = 0;
		}
		pWindow->PrintMsgRightSide(8, FG_WHITE, L" real FPS: %d", lastFPS);*/
		

		if (!OnUserUpdate(deltaTime))
			bIsRunning = false;


		pConsole->UpdateConsole();
		CalculateFPS();
		pWindow->UpdateWindow();
		LimitFrameTime();
	}
	OnUserDestroy();
}

double CEngine::LimitFrameTime()
{
	static CTimerValue tpLast = pTimer->GetHighPrecisionTime();
	CTimerValue tpNow = pTimer->GetHighPrecisionTime();

	double currentDelta = (tpNow - tpLast).GetMilliseconds();
	double sleepTime = maxFrameTime - currentDelta;

	if (sleepTime > 0.0)
	{
		tpLast = tpNow = pTimer->GetHighPrecisionTime();
		while (sleepTime >= (tpNow - tpLast).GetMilliseconds())
			tpNow = pTimer->GetHighPrecisionTime();
		sleepTime = (tpNow - tpLast).GetMilliseconds();
	}

	

	tpLast = tpNow;

	/*if (sleepTime > 0.0)
		Sleep(1);*/
	//pConsole->CPrintF(L"Elapsed time of LimitFrameTime() - ~%3.3f ms", currentDelta + sleepTime);
	return (currentDelta + sleepTime) / 1000.0;

	//static auto tp1 = std::chrono::high_resolution_clock::now();
	//auto tp2 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> duration = tp2 - tp1;

	//double sleepTime = maxFrameTime - duration.count() * 1000.0; // умножаем на 1000 для перевода в миллисекунды

	//if (sleepTime > 0.0) {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(sleepTime)));
	//}

	//tp1 = tp2;

	//return (duration.count() / 1000 + sleepTime) / 1000.0;
}

void CEngine::CalculateFPS()
{
	static CTimerValue tpLast = pTimer->GetHighPrecisionTime();
	CTimerValue tpNow = pTimer->GetHighPrecisionTime();
	
	static const int MAX_FRAME_SAMPLES = 60;
	static int currentFrame = 0;

	static double frameTimes[MAX_FRAME_SAMPLES];
	static double avgFPS = 0.0, maxFrameTime = 0.0, minFrameTime = 0.0;
	static double prevAvgFPS = 0.0, prevMaxFrameTime = 0.0, prevMinFrameTime = 0.0;
	static double refreshFPStime = 0.5;

	double avgFrameTime = 0.0;
	double deltaTime = (tpNow - tpLast).GetSeconds();
	tpLast = tpNow;

	refreshFPStime += deltaTime;

	frameTimes[currentFrame] = deltaTime;

	if (currentFrame + 1 >= MAX_FRAME_SAMPLES)
	{
		maxFrameTime = minFrameTime = 0.0;

		for (int i = 0; i < MAX_FRAME_SAMPLES; i++)
		{
			double currentFrameTime = frameTimes[i];

			if (minFrameTime == 0.0 || currentFrameTime <= minFrameTime)
				minFrameTime = currentFrameTime;

			if (currentFrameTime >= maxFrameTime)
				maxFrameTime = currentFrameTime;

			avgFrameTime += currentFrameTime;
		}
		avgFrameTime /= MAX_FRAME_SAMPLES;
		avgFPS = 1.0 / avgFrameTime;
	}
	currentFrame = (currentFrame + 1) % MAX_FRAME_SAMPLES;
	
	if (refreshFPStime >= 0.0)
	{
		refreshFPStime -= 0.5;
		prevAvgFPS = avgFPS;
		prevMaxFrameTime = maxFrameTime;
		prevMinFrameTime = minFrameTime;
	}

	if(bShowFPS)
	{
		pWindow->PrintMsgRightSide(5, FG_GREEN, L"FPS: %d (~%3.3f ms)", (int)prevAvgFPS, 1000.0 / prevAvgFPS);
		pWindow->PrintMsgRightSide(6, FG_GREEN, L"max: %3.3f ms | min: %3.3f ms", prevMaxFrameTime * 1000.0, prevMinFrameTime * 1000.0);
	}
}



