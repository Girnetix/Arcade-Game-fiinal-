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
	Editing,
	CreatingRunner,
	CreatingRandomer,
	CreatingCannon
};

enum PeekRandomerPositions
{
	First,
	Second
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
	void CreateRunner(int x, int y);
	void CreateRandomer();
	void CreateCannon(int x, int y);
private:
	CoreGameState coreGameState;
	MainGameState mainGameState;
	EditorState editorState;
	PeekRandomerPositions randomerPositions = First;
	CMenu MainMenu;
	CMenu menuCreatingRunner;
	CMenu menuCreatingRandomer;
	CMenu menuCreatingCannon;
	Entity::Direction entityDirection;
	CTimerValue exitButtonPressedTp1, exitButtonPressedTp2;
	CButton exitButton;
	int x, y, minX, minY, maxX, maxY;
	bool gameOver = false, gameWon = false;
private:
	int xCurs = 0, yCurs = 0;

};


#endif
