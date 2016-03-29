#include "AboutScreen.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"

bool AboutScreen::luaBinded = false;

AboutScreen::AboutScreen(Vector2 screenSize, Game* game)
{
	if (!luaBinded) {
		luabridge::getGlobalNamespace(L)
			.deriveClass<AboutScreen, Screen>("AboutScreen")
			.endClass();
	}
	luaBinded = true;

	screenOffsetY = screenYReset = screenSize.y;
	font = Font::create("res/arial.gpb");
	
	AddStatement("Programming and some graphical assets by Aaron Teague");
	AddStatement("Some graphical assets by Chris Boyd");
	AddStatement("Music by Incompetech.com (More details when song plays");
	AddStatement("Framework by gameplay3d (www.gameplay3d.io");
}
void AboutScreen::Update(const float& elapsedTime)
{
	screenOffsetY += -2;
	if (screenOffsetY < 0)
		screenOffsetY = screenYReset;
}
void AboutScreen::Render()
{
	font->start();
	for (unsigned i = 0; i < statementList.size(); ++i) {
		font->drawText(statementList[i].text.c_str()
			, statementList[i].pos.x
			, statementList[i].pos.y + screenOffsetY
			, Vector4(1,1,1,1), font->getSize() * 3);
	}
	font->finish();
}
void AboutScreen::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	if (evt != Touch::TOUCH_PRESS)
		return;
	char luaCommand[] = "GAME:ShowMainMenu()";
	int status = luaL_dostring(L, luaCommand);
	if (status != 0) {
		std::string error = lua_tostring(L, -1);
		GP_ERROR(error.c_str());
	}
}


AboutScreen::~AboutScreen() {}

void AboutScreen::AddStatement(std::string text)
{
	int y = 0;
	if (statementList.size() > 0)
		y = statementList[statementList.size() - 1].pos.y + 100;

	Statement statement;
	statement.pos.set(0, y);
	statement.text = text;
	
	statementList.push_back(statement);
}
