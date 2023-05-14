#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "Input.h"
#include "ActionButton.h"
#include "Console.h"
#include "Stream.h"
#include "Memory.h"
#include "Rendering.h"
#include "PRNG.h"
#include "Timer.h"
#include "Entity.h"
#include "World.h"

class CEngine
{
public:
	~CEngine() { Stop(); }
	void Start(int iScrWidth, int iScrHeight, int iFontX, int iFontY, const std::wstring& font = L"Точечные шрифты", const std::wstring& AppName = L"Default");
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