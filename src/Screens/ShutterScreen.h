#pragma once

#include "../Screen.h"
#include "gameplay.h"

using namespace gameplay;

class ShutterScreen : public Screen
{
public:
	ShutterScreen(Vector2 screenSize);

	virtual void Update(const float& elapsedTime);
	virtual void Render();
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
private:
	SpriteBatch* batch;
//	Vector2 screenSize;
	gameplay::Rectangle imgRec;
	gameplay::Rectangle rec;
	float timer = 0;
	float alpha = 1.0f;
};