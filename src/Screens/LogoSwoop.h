#pragma once

#include "../Screen.h"

using namespace gameplay;

class MyCurve;

class LogoSwoop : public Screen{
public:
	LogoSwoop(Vector2 screenSize, int milliseconds);
	void Update(const float& elapsedTime);
	void Render();
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	bool isFinished(){ return !running; }
private:
	MyCurve* logoTop = NULL;
	MyCurve* logoBottom = NULL;
	SpriteBatch* logoBatch = NULL;
	SpriteBatch* starBackBatch = NULL;
	float starAlpha = 0;
	Vector2 starOffset;
	float topX;
	float bottomX;

	Vector2 screenSize;
	bool running;
};