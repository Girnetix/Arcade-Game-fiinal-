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
	GameWon,
	GameOver,
	Finishing
};

enum EditorState
{
	Editing,
	CreatingRandomer,
	EditingSpeed
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
	void DrawLegend();
private:
	void CreateRandomer();
private:
	CoreGameState coreGameState;
	MainGameState mainGameState;
	EditorState editorState;
	PeekRandomerPositions randomerPositions = First;
	CMenu MainMenu;
	Entity::Direction entityDirection;
	CTimerValue exitButtonPressedTp1, exitButtonPressedTp2;
	int x, y, minX, minY, maxX, maxY;
	int countOfPlayers = 0;
	int totalKills = 0, totalDeaths = 0, totalScore = 0;
	bool gameOver = false, gameWon = false;
	Entity* pickedEntity = nullptr;
	double gameTime = 0.0, returningTimeInMenu = 10.0;
	int sec = 0, min = 0, hour = 0;
private:
	int xCurs = 0, yCurs = 0;
};

#endif
