#pragma once
#include "../Screen.h"

using namespace gameplay;

class myCamera;

class AboutScreen : public Screen
{
public:
	AboutScreen(Vector2 screenSize, Game* game);
	void Update(const float& elapsedTime);
	void Render();
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	~AboutScreen();
private:
	struct Statement {
		std::string text;
		Vector2 pos;
	};

	Font* font;
	int screenOffsetY;
	int screenYReset;
	std::vector<Statement> statementList;

	void AddStatement(std::string text);

	static bool luaBinded;
};