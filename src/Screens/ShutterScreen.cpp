#include "ShutterScreen.h"

ShutterScreen::ShutterScreen(Vector2 screenSize)
:rec(0, 0, screenSize.x, screenSize.y), imgRec(0,0,1024, 1024 * screenSize.y / screenSize.x)
{
	batch = SpriteBatch::create("res/Shutter.png");
}

void ShutterScreen::Update(const float& elapsedTime)
{
	const float FADE_TIME = 1500;
	const float KILL_TIME = 2000;
	const float SCALE_RATE = 120.0;

	if (elapsedTime < 100) // skip load pauses
		timer += elapsedTime;

	if (timer > FADE_TIME)
		alpha -= 0.05f;

	if (timer > KILL_TIME)
		isFinished = true;

	rec.set(rec.x - SCALE_RATE, rec.y - SCALE_RATE, rec.width + SCALE_RATE * 2, rec.height + SCALE_RATE * 2);
}
void ShutterScreen::Render()
{
	batch->start();
	batch->draw(rec, imgRec, Vector4(1,1,1, alpha));
	batch->finish();
}
void ShutterScreen::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	// no touchy!!!
}