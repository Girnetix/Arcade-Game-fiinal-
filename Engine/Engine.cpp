#include "Engine.h"

void CEngine::Start(int iScrWidth, int iScrHeight, int iFontX, int iFontY, const std::wstring& font, const std::wstring& AppName, int maxFPS)
{
	pConsole = new CConsole();
	pWindow = new Window();
	pWindow->ConstructWindow(iScrWidth, iScrHeight, iFontX, iFontY, font, AppName);
	pConsole->CPrintF(L"Engine startup");
	pConsole->CPrintF(L"Created console window: %d x %d, %s", iScrWidth, iScrHeight, AppName.c_str());
	pTimer = new Timer();
	pWorld = new World();
	pNumGenerator = new default_number_generator();
	appName = AppName;
	if(maxFPS > 0)
	{
		this->maxFrameTime = 1000.0 / (double)maxFPS;
		this->maxFPS = maxFPS;		
	}
	cmdBuffer.registerCommand("maxFPS", [&](const std::vector<std::string>& args) {
		if (args.size() < 2)
			return "Usage: maxFPS <value>";
		try
		{
			if (std::stoi(args[1]) > 0) {
				this->maxFPS = std::stoi(args[1]);
				maxFrameTime = 1000.0 / this->maxFPS;
			}
			else
			{
				this->maxFPS = std::stoi(args[1]);
				maxFrameTime = 0.0;
			}
			return args[1].c_str();
		}
		catch (const std::exception& ex)
		{
			return ex.what();
		}

		return "Unknown error";
		});
	//если всё запущено, то устанавливаем флаг о том, что движок запущен
	if (pConsole.Get() && pWindow.Get() && pTimer.Get() && pWorld.Get() && pNumGenerator.Get())
	{
		pConsole->CPrintF(L"Engine started without errors");
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
	bool showProfilling = false;
	cmdBuffer.registerCommand("showProfilling", [&](const std::vector<std::string>& args) {
		if (args.size() < 2)
			return "Usage: showProfilling <value>";
		try
		{
			showProfilling = std::stoi(args[1]);
			return args[1].c_str();
		}
		catch (const std::exception& ex)
		{
			return ex.what();
		}
		
		return "Unknown error";
		});

	std::vector<CProfiling> profilingTimers;
	pWindow->ChangeAppName(appName);
	pTimer->ResetTimer();

	while (bIsRunning)
	{
		/*double totalTime = 0.0;*/

		{
			CProfiling profiling(L"pTimer->UpdateTimer(): % 3.3f ms");
			profiling.Start();

			pTimer->UpdateTimer();

			profiling.Stop();
			profilingTimers.push_back(profiling);
		}

		{
			CProfiling profiling(L"pWindow->ClearAllWindow(): % 3.3f ms");
			profiling.Start();

			pWindow->ClearAllWindow();

			profiling.Stop();
			profilingTimers.push_back(profiling);
		}

		{
			CProfiling profiling(L"UpdateKeyboard(): % 3.3f ms");
			profiling.Start();

			Keyboard::UpdateKeyboard();

			profiling.Stop();
			profilingTimers.push_back(profiling);
		}

		if (showProfilling)
		{
			for (int i = 0; i < profilingTimers.size(); i++) {
				profilingTimers[i].PrintInfo(25 + i);
			}
		}
		profilingTimers.clear();

		/*if (Keyboard::GetKey(EQUALS).bPressed)
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
		}*/

		deltaTime = pTimer->GetDeltaTimeSec();

		

		/*fpsTime += deltaTime;
		ticks++;
		if (fpsTime >= 1.0)
		{
			lastFPS = ticks;
			fpsTime -= 1.0;
			ticks = 0;
			pConsole->CPrintF(L"Real FPS: %d", lastFPS);
		}*/
		

		{
			CProfiling profiling(L"OnUserUpdate(double): % 3.3f ms");
			profiling.Start();

			if (!pConsole->ConsoleIsShow())
				if (!OnUserUpdate(deltaTime))
					bIsRunning = false;

			profiling.Stop();
			profilingTimers.push_back(profiling);
		}

		//pWindow->PrintMsgRightSide(40, FG_WHITE, L"Window is active: %s", pWindow->IsWindowActive() ? L"yes" : L"no");

		pConsole->UpdateConsole(deltaTime);

		{
			CProfiling profiling(L"CalculateFPS(): %3.3f ms");
			profiling.Start();

			CalculateFPS();

			profiling.Stop();
			profilingTimers.push_back(profiling);
		}

		{
			CProfiling profiling(L"LimitFrameTime(): %3.3f ms");
			profiling.Start();

			LimitFrameTime();

			profiling.Stop();
			profilingTimers.push_back(profiling);
		}

		//pWindow->PrintMsgRightSide(29, FG_WHITE, L"Total time: %3.3f ms", totalTime * 1000.0);

		{
			CProfiling profiling(L"pWindow->UpdateWindow(): %3.3f ms");
			profiling.Start();

			pWindow->UpdateWindow();

			profiling.Stop();
			profilingTimers.push_back(profiling);
		}
	}
	OnUserDestroy();
}

double CEngine::LimitFrameTime()
{
	static CTimerValue tpLast = pTimer->GetHighPrecisionTime();
	CTimerValue tpNow = pTimer->GetHighPrecisionTime();

	double currentDelta = (tpNow - tpLast).GetMilliseconds();
	double sleepTime = maxFrameTime - currentDelta;

	/*if (!pWindow->IsWindowActive())
		sleepTime = 1000.0 / 20.0 - currentDelta;*/

	if (sleepTime > 0.0)
	{
		tpLast = tpNow = pTimer->GetHighPrecisionTime();
		while (sleepTime >= (tpNow - tpLast).GetMilliseconds())
			tpNow = pTimer->GetHighPrecisionTime();
		sleepTime = (tpNow - tpLast).GetMilliseconds();
	}

	
#if 1
	tpLast = tpNow;
#else
	tpLast = pTimer->GetHighPrecisionTime();
#endif
	//pConsole->CPrintF(L"LimitFrameTime() - ~%3.3f ms (%3.3f + %3.3f ms)", currentDelta + sleepTime, currentDelta, sleepTime);
	return (currentDelta + sleepTime) / 1000.0;
}

void CEngine::CalculateFPS() {
	static CTimerValue tpLast = pTimer->GetHighPrecisionTime();
	CTimerValue tpNow = pTimer->GetHighPrecisionTime();

	static const int MAX_FRAME_SAMPLES = 60;
	static int currentFrame = 0;

	static double frameTimes[MAX_FRAME_SAMPLES];
	static double avgFPS = 0.0, maxFrameTime = 0.0, minFrameTime = 0.0;
	static double prevAvgFPS = 0.0, prevMaxFrameTime = 0.0, prevMinFrameTime = 0.0;
	static double refreshFPSTimer = 0.0;

	double avgFrameTime = 0.0;
	double deltaTime = (tpNow - tpLast).GetSeconds();
	tpLast = tpNow;

	// Обновляем время для текущего кадра
	frameTimes[currentFrame] = deltaTime;
	currentFrame = (currentFrame + 1) % MAX_FRAME_SAMPLES;

	// Суммируем время всех кадров для получения среднего
	for (int i = 0; i < MAX_FRAME_SAMPLES; ++i) {
		avgFrameTime += frameTimes[i];
	}
	avgFrameTime /= MAX_FRAME_SAMPLES;
	avgFPS = 1.0 / avgFrameTime;

	// Находим минимальное и максимальное время кадра
	maxFrameTime = *std::max_element(frameTimes, frameTimes + MAX_FRAME_SAMPLES);
	minFrameTime = *std::min_element(frameTimes, frameTimes + MAX_FRAME_SAMPLES);

	// Обновляем счетчики каждые 0.5 секунды
	refreshFPSTimer += deltaTime;
	if (refreshFPSTimer >= 0.5) {
		refreshFPSTimer = 0.0;
		prevAvgFPS = avgFPS;
		prevMaxFrameTime = maxFrameTime;
		prevMinFrameTime = minFrameTime;
	}

	// Отображаем FPS
	if (bShowFPS) {
		pWindow->PrintMsgRightSide(5, prevAvgFPS >= 30 ? FG_GREEN : FG_RED, L"FPS: %d (~%3.3f ms)", (int)prevAvgFPS, 1000.0 / prevAvgFPS);
		pWindow->PrintMsgRightSide(6, prevAvgFPS >= 30 ? FG_GREEN : FG_RED, L"max: %3.3f ms | min: %3.3f ms", prevMaxFrameTime * 1000.0, prevMinFrameTime * 1000.0);
	}
}
