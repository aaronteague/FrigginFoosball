#include "LogoSwoop.h"
#include "../MyCurve.h"
#include "../Dependencies/LuaBridge/LuaBridge.h"

LogoSwoop::LogoSwoop(Vector2 screenSize, int milliseconds)
:screenSize(screenSize)
{
	logoBatch = SpriteBatch::create("res/Logo.png");
	starBackBatch = SpriteBatch::create("res/stars.png");

	logoTop = new MyCurve(Curve::LINEAR);
	logoBottom = new MyCurve(Curve::LINEAR);

	logoTop->addPoint(0, -1 * (float)screenSize.x);
	logoTop->addPoint(milliseconds / 3, -0.25f * (float)screenSize.x);
	logoTop->addPoint(milliseconds / 3, (float)screenSize.x / 4);
	logoTop->addPoint(milliseconds / 3, screenSize.x);

	logoBottom->addPoint(0, screenSize.x);
	logoBottom->addPoint(milliseconds / 3, (float)screenSize.x / 4);
	logoBottom->addPoint(milliseconds / 3, -0.25f * (float)screenSize.x);
	logoBottom->addPoint(milliseconds / 3, -1 * (float)screenSize.x);

	starAlpha = 0.1f;
	starOffset.set(-512, -512);
	
}
void LogoSwoop::Update(const float& elapsedTime)
{
	if (logoTop && logoBottom){
		//	gameplay::Rectangle topRec(0, 0, getWidth(), getHeight());
		//	gameplay::Rectangle bottomRec(0, getHeight() / 2, getWidth(), getHeight());
		logoTop->evaluate(elapsedTime, topX);
		logoBottom->evaluate(elapsedTime, bottomX);
		starAlpha += 0.1f;
		if (starAlpha > 1)
			starAlpha = 1.0f;

		if (logoTop->isComplete() || logoBottom->isComplete()){
			delete(logoTop);
			delete(logoBottom);
			logoTop = NULL;
			logoBottom = NULL;
		}
	}
	else{
		starAlpha -= 0.1f;
		if (starAlpha <= 0.0f){
			isFinished = true;
			luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
			resume();
		}
	}
	starOffset += Vector2(5, 5);
	if (starOffset.x > 0)
		starOffset.set(-512, -512);
}
void LogoSwoop::Render()
{
	if (starAlpha > 0){
		starBackBatch->start();
		for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			starBackBatch->draw(gameplay::Rectangle(starOffset.x + 512 * i, starOffset.y + 512 * j, 512, 512), gameplay::Rectangle(0, 0, 512, 512), Vector4(1, 1, 1, starAlpha));

		starBackBatch->finish();

		logoBatch->start();
		logoBatch->draw(gameplay::Rectangle(topX, 0, screenSize.x, screenSize.y / 2), gameplay::Rectangle(0, 0, 1917, 528));
		logoBatch->draw(gameplay::Rectangle(bottomX, screenSize.y / 2, screenSize.x, screenSize.y / 2), gameplay::Rectangle(0, 555, 1917, 527));
		logoBatch->finish();
	}
}
void LogoSwoop::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
}