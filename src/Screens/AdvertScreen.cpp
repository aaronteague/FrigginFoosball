#include "AdvertScreen.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"
#include "../advert.h"


bool AdvertScreen::luaBinded = false;

AdvertScreen::AdvertScreen(Vector2 screenSize, Game* game)
{
	if (!luaBinded)
		luabridge::getGlobalNamespace(L)
		.deriveClass<AdvertScreen, Screen>("AdvertScreen")
		.endClass();
	luaBinded = true;

	Advert::CallJava(Advert::LOAD_INTERSTITIAL);
}

void AdvertScreen::Update(const float & elapsedTime)
{
	// don't think this is actually needed.  it tends to pause when ad shows up anyway
	isFinished = !AdsShowing();
}

void AdvertScreen::Render()
{

}

void AdvertScreen::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	// doesn't actually handle touch
}

void AdvertScreen::ShowAds()
{
	Advert::CallJava(Advert::SHOW_AD);
}

bool AdvertScreen::AdsShowing()
{
	 return Advert::CallJava(Advert::AD_SHOWING);
}

AdvertScreen::~AdvertScreen()
{

}



