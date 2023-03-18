#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "Memory.h"
#include "Rendering.h"
#include "PRNG.h"
#include "Timer.h"
#include "Entity.h"

class CEngine
{
public:
	~CEngine() { Stop(); }
	void Start(int iScrWidth, int iScrHeight, std::wstring AppName = L"Default");
	void Stop();
private:
	void GameThread();
protected:
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(double deltaTime) = 0;
	virtual void OnUserDestroy() = 0;
protected:
	bool bIsRunning = false;
	bool bShowFPS = false;
	bool bShowPing = false;
};

#endif
