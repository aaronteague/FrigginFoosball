#include "DifficultySet.h"
#include "../Opponent.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"


bool DifficultySet::luaBinded = false;

DifficultySet::DifficultySet(Vector2 screenSize, Opponent* opponent)
	:opponent(opponent)
{
	if (!luaBinded) {
		luabridge::getGlobalNamespace(L)
			.deriveClass<DifficultySet, Screen>("DifficultySet")
			.endClass();
	}
	luaBinded = true;

	scale = screenSize.y / 1000.0f;

	font = Font::create("res/arial.gpb");

	isFinished = false;

	DIFFICULTY_W *= scale; DIFFICUTLY_H *= scale; DIFFICULTY_Y *= scale;
	difficulty.set(screenSize.x / 2 - DIFFICULTY_W / 2,
		DIFFICULTY_Y, DIFFICULTY_W, DIFFICUTLY_H);

	EASY_W *= scale; EASY_H *= scale; EASY_Y *= scale;
	easy.set(screenSize.x / 2 - EASY_W / 2,
		EASY_Y, EASY_W, EASY_H);

	MEDIUM_W *= scale; MEDIUM_H *= scale; MEDIUM_Y *= scale;
	medium.set(screenSize.x / 2 - MEDIUM_W / 2,
		MEDIUM_Y, MEDIUM_W, MEDIUM_H);

	HARD_W *= scale; HARD_H *= scale; HARD_Y *= scale;
	hard.set(screenSize.x / 2 - HARD_W / 2,
		HARD_Y, HARD_W, HARD_H);


}

void DifficultySet::Update(const float& elapsedTime)
{

}

void DifficultySet::Render()
{
	Vector4 red(1, 0, 0, 1);
	Vector4 white(1, 1, 1, 1);
	float size = font->getSize() * 12 * scale;

	font->start();

	font->drawText("Choose your difficulty", difficulty, red, size, Font::ALIGN_HCENTER);
	font->drawText("Easy", easy, white, size, Font::ALIGN_HCENTER);
	font->drawText("Medium", medium, white, size, Font::ALIGN_HCENTER);
	font->drawText("Hard", hard, white, size, Font::ALIGN_HCENTER);


	font->finish();
}

void DifficultySet::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	if (evt != Touch::TouchEvent::TOUCH_PRESS)
		return;

	bool difSet = false;

	if (easy.contains(x, y)) {
		opponent->SetDifficulty(0.5f);
		difSet = true;
	}
	else if (medium.contains(x, y)) {
		opponent->SetDifficulty(1.0f);
		difSet = true;
	}
	else if (hard.contains(x, y)) {
		opponent->SetDifficulty(1.8f);
		difSet = true;
	}

	if (difSet) {
		char* file = FileSystem::readAll("res/quickGame/level.lua");
		int status = luaL_dostring(L, file);
		if (status != 0) {
			std::string error = lua_tostring(L, -1);
			GP_ERROR(error.c_str());
		}
	}
}

DifficultySet::~DifficultySet()
{

}

