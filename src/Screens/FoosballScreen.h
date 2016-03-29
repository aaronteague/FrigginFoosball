#pragma once

#include "../Screen.h"

using namespace gameplay;

class Opponent;
class Player;
class Table;
class MyCurve;
class Announcer;
class MyCamera;
class LogoSwoop;
class AdvertScreen;
class Pause;

class FoosballScreen : public Screen
{
public:
	FoosballScreen(Vector2 screenSize, Game* game, Table* table, Opponent* opponent, Player* player);
	~FoosballScreen();
	virtual void Update(const float& elapsedTime);
	virtual void Render();
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	void keyEvent(Keyboard::KeyEvent evt, int key);

	void StartRound();
	void SwoopLogo(int milliseconds);
	void ShowAds();
	void PauseGame();
	void ResumeGame();
private:
	Opponent* opponent;
	Player* player;
	Table* table;
	MyCamera* camera;
	Announcer* announcer;

	AdvertScreen* advertScreen = NULL;
	Pause* pauseScreen = NULL;
	Screen* childScreen;

	//	bool gameRunning = false;
	bool roundRunning = false;
	
	Vector2 screenSize;
	Game* game;

	Scene* _scene;
	Vector3 pauseVelocity;

	static bool luaBinded;

//	void updateLogoSwoop(const float& elapsedTime);
	bool drawScene(Node* node);
};