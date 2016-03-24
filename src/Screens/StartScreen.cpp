#include "StartScreen.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"

bool StartScreen::luaBinded = false;

StartScreen::StartScreen(Vector2 screenSize)
:screenSize(screenSize)
{
	splashBatch = SpriteBatch::create("res/Sexy Intro Screen.png");
	

	if (!luaBinded){
		luabridge::getGlobalNamespace(L)
			.deriveClass<StartScreen, Screen>("StartScreen")
			.addFunction("ShowClickToStart", &StartScreen::ShowClickToStart)
			.endClass();
	}
	luaBinded = true;
}

void StartScreen::Update(const float& elapsedTime)
{

}
void StartScreen::Render()
{
	if (splashBatch){ // splash
		splashBatch->start();
		splashBatch->draw(gameplay::Rectangle(0, 0, screenSize.x, screenSize.y), gameplay::Rectangle(0, 0, 2048, 1024));
		splashBatch->finish();
	}

	if (startFont){ // click to start
		startFont->start();
		startFont->drawText("Click To Start", gameplay::Rectangle(screenSize.x / 2 - 200, screenSize.y * 0.75f, 400, 200), Vector4::one(), startFont->getSize() * 5, Font::ALIGN_VCENTER);
		startFont->finish();
	}
}
void StartScreen::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	if (startFont && evt == Touch::TOUCH_PRESS){
		luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
		resume();
	}
}

void StartScreen::ShowClickToStart()
{
	startFont = Font::create("res/arial.gpb");
}