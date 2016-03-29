#pragma once
#include "../Screen.h"

using namespace gameplay;

class AdvertScreen : public Screen {
public:
	AdvertScreen(Vector2 screenSize, Game* game);
	void Update(const float& elapsedTime);
	void Render();
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
	void ShowAds();
	bool AdsShowing();
	~AdvertScreen();
private:
	static bool luaBinded;
};