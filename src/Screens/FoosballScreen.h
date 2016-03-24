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

class FoosballScreen : public Screen
{
public:
	FoosballScreen(Vector2 screenSize, Game* game);
	virtual void Update(const float& elapsedTime);
	virtual void Render();
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	void keyEvent(Keyboard::KeyEvent evt, int key);

	void StartRound();
	void SwoopLogo(int milliseconds);
private:
	Opponent* opponent;
	Player* player;
	Table* table;
	MyCamera* camera;
	Announcer* announcer;

	LogoSwoop* logoSwoop = NULL;

	//	bool gameRunning = false;
	bool roundRunning = false;
	
	Vector2 screenSize;
	Game* game;

	Scene* _scene;


	static bool luaBinded;

//	void updateLogoSwoop(const float& elapsedTime);
	bool drawScene(Node* node);
};