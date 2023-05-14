#pragma once
#ifndef GAME_H
#define GAME_H

#include "GameMenu.h"

enum CoreGameState
{
	Menu,
	Game,
	Editor,
	Hints
};

class CGame :public CEngine
{
public:
	bool OnUserCreate() override;
	bool OnUserUpdate(double deltaTime) override;
	void OnUserDestroy() override;
private:
	CoreGameState coreGameState;
	CMenu menu;
};


#endif
