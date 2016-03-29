#pragma once
#include "../Screen.h"

class Opponent;

using namespace gameplay;

class DifficultySet : public Screen {
public:
	DifficultySet(Vector2 screenSize, Opponent* opponent);
	virtual void Update(const float& elapsedTime);
	virtual void Render();
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
	~DifficultySet();
private:
	Opponent* opponent = NULL;

	Font* font;

	float DIFFICULTY_W = 800;
	float DIFFICUTLY_H = 300;
	float DIFFICULTY_Y = 50.0f;
	gameplay::Rectangle difficulty;

	float EASY_W = 800;
	float EASY_H = 200;
	float EASY_Y = 400;
	gameplay::Rectangle easy;

	float MEDIUM_W = 800;
	float MEDIUM_H = 200;
	float MEDIUM_Y = 600;
	gameplay::Rectangle medium;

	float HARD_W = 800;
	float HARD_H = 200;
	float HARD_Y = 800;
	gameplay::Rectangle hard;

	float scale;

	static bool luaBinded;
};