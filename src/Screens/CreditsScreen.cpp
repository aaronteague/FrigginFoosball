#include "CreditsScreen.h"

CreditsScreen::CreditsScreen(Vector2 screenSize, const std::string& text)
:text(text)
, textBox(0, screenSize.y - screenSize.y / 4, screenSize.x, screenSize.y / 4)
, fontScale(screenSize.x / 1000.0f * 3.0f)
{
	font = Font::create("res/arial.gpb");
}
void CreditsScreen::Update(const float& elapsedTime)
{
	timer += elapsedTime;
	if (timer > DISPLAY_TIME)
		isFinished = true;
}
void CreditsScreen::Render()
{
	font->start();
	font->drawText(text.c_str(), textBox, Vector4::one(), font->getSize() * fontScale, Font::Justify::ALIGN_BOTTOM_LEFT);
	font->finish();
}
void CreditsScreen::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	// nope, no touching!!!
}