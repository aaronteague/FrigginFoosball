#pragma once
#include "../Screen.h"

class MyCamera;
class Table;

using namespace gameplay;

class Pause : public Screen 
{
public:
	Pause(Vector2 screenSize, int score1, int score2, Table* table, MyCamera* cam);
	virtual void Update(const float& elapsedTime);
	virtual void Render();
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
	~Pause();



private:

	Font* font;
	float PAUSE_W = 800;
	float PAUSE_H = 300;
	float PAUSE_Y = 50.0f;
	gameplay::Rectangle pause;

	float RESUME_W = 800;
	float RESUME_H = 300;
	float RESUME_Y = 400;
	gameplay::Rectangle resume;

	float MAIN_W = 800;
	float MAIN_H = 300;
	float MAIN_Y = 700;
	gameplay::Rectangle mainMenu;

	gameplay::Rectangle scoreRec1;

	gameplay::Rectangle scoreRec2;

	int score1;
	int score2;

	float scale;

	Table* table;
	MyCamera* cam;
};