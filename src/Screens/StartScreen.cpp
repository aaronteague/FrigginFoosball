#include "StartScreen.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"
//#include "../advert.h"





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

//	Advert::CallJava(Advert::LOAD_INTERSTITIAL);
}

void StartScreen::Update(const float& elapsedTime)
{
//	if (startFont)
//		Advert::CallJava(Advert::SHOW_AD);
}
void StartScreen::Render()
{
	if (splashBatch){ // splash
		splashBatch->start();
		splashBatch->draw(gameplay::Rectangle(0, 0, screenSize.x, screenSize.y), gameplay::Rectangle(0, 0, 1024, 512));
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
		playSound(0);
		luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
		resume();
	}
}

void StartScreen::ShowClickToStart()
{
	startFont = Font::create("res/arial.gpb");
	loadSound(0, "res/sounds/ping.wav");
}

