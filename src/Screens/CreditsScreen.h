#pragma once

#include "../Screen.h"
#include "gameplay.h"

using namespace gameplay;

class CreditsScreen : public Screen
{
public:
	CreditsScreen(Vector2 screenSize, const std::string& text);
	virtual void Update(const float& elapsedTime);
	virtual void Render();
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
private:
	Font* font;
	std::string text;
	float timer = 0;
	const float DISPLAY_TIME = 3000; // 3-seconds
	gameplay::Rectangle textBox;
	float fontScale;
};