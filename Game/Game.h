#pragma once
#ifndef GAME_H
#define GAME_H

#include "GameMenu.h"
#include "EntitiesTypes.h"
#include "Bullet.h"
#include "Cannon.h"
#include "Player.h"
#include "Runner.h"
#include "Randomer.h"
#include "ExtraEntities.h"

enum CoreGameState
{
	Menu,
	Game,
	Editor
};

enum MainGameState
{
	Starting,
	Running,
	Finishing
};

enum EditorState
{
	Starting,
	Running,
	Finishing
};

class CGame :public CEngine
{
public:
	bool OnUserCreate() override;
	bool OnUserUpdate(double deltaTime) override;
	void OnUserDestroy() override;
	void UpdateGame(double deltaTime);
	void UpdateEditor(double deltaTime);
private:
	void LoadWorld();
	void UpdateWorld(double deltaTime);
	void CloseWorld();
private:
	CoreGameState coreGameState;
	MainGameState mainGameState;
	EditorState editorState;
	CMenu menu;
	bool gameOver = false, gameWon = false;
};


#endif
