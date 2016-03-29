#pragma once
#include "../Screen.h"

//#include "gameplay.h"

using namespace gameplay;

void showAds();

class StartScreen : public Screen
{
public:
	StartScreen(Vector2 screenSize);
	void Update(const float& elapsedTime);
	void Render();
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	void ShowClickToStart();

private:
	SpriteBatch* splashBatch = NULL;
	Font* startFont = NULL;
	Vector2 screenSize;
	static bool luaBinded;
};