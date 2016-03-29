#include "Pause.h"
#include "../MyCamera.h"
//#include "FoosballScreen.h"
#include "../FSM.h"
#include "../Table.h"

Pause::Pause(Vector2 screenSize, int score1, int score2, Table* table, MyCamera* cam)
	:table(table), cam(cam)
{
	scale = screenSize.y / 1000.0f;

	font = Font::create("res/arial.gpb");
	isFinished = false;

	Pause::score1 = score1;
	Pause::score2 = score2;

	PAUSE_W *= scale; PAUSE_H *= scale; PAUSE_Y *= scale;
	pause.set(screenSize.x / 2 - PAUSE_W / 2,
		PAUSE_Y, PAUSE_W, PAUSE_H);

	RESUME_W *= scale; RESUME_H *= scale; RESUME_Y *= scale;
	resume.set(screenSize.x / 2 - RESUME_W / 2,
		RESUME_Y, RESUME_W, RESUME_H);

	MAIN_W *= scale; MAIN_H *= scale; MAIN_Y *= scale;
	mainMenu.set(screenSize.x / 2 - MAIN_W / 2,
		MAIN_Y, MAIN_W, MAIN_H);

//	Bundle* bund = Bundle::create("res/corkboard.gpb");
//	_scene = bund->loadScene();

//	cam = new MyCamera(_scene, screenSize.x, screenSize.y);
//	cam->SetState("Overmap");

	//for (int i = 0; i < 5; ++i)
	//	for (int j = 0; j < 5; ++j) {
	//		Node* node = _scene->findNode("Board")->clone();
	//		Material* corkMat = Entity::buildMaterial(_scene, Texture::create("res/cork-diffuse.png"), Entity::TEXTURED_NORMAL, false, -1);
	//		Entity::applyNormalMap(corkMat, Texture::create("res/cork-normal.png", true));
	//		corkMat->getParameter("u_specularExponent")->setValue(10.0f);
	//		corkMat->getParameter("u_ambientColor")->setValue(Vector3(0.3f, 0.3f, 0.3f));
	//		((Model*)node->getDrawable())->setMaterial(corkMat);
	//		node->setTranslation(i * 6, j * 6, 0);
	//		_scene->addNode(node);
	//	}
}



void Pause::Update(const float& elapsedTime)
{

}

void Pause::Render()
{
//	_scene->findNode("Board")->getDrawable()->draw();
	
	Vector4 red(1, 0, 0, 1);
	Vector4 white(1, 1, 1, 1);
	float size = font->getSize() * 12 * scale;

	font->start();

	font->drawText("Pause Menu", pause, red, size, Font::ALIGN_HCENTER);
	font->drawText("Resume", resume, white, size, Font::ALIGN_HCENTER);
	font->drawText("Main Menu", mainMenu, white, size, Font::ALIGN_HCENTER);

	font->finish();
}

void Pause::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	if (resume.contains(x, y)) {
		isFinished = true;
	}
	else if (mainMenu.contains(x, y)) { // close out of this shit nigga
		int status = luaL_dostring(L, "GAME:ShowMainMenu()");
		if (status != 0) {
			MessageDispatcher::Instance()->dispatchMessage(0, NULL, table, FSM::GAME_OVER, NULL);
			MessageDispatcher::Instance()->dispatchMessage(0, NULL, cam, FSM::GAME_OVER, NULL);

			lua_getglobal(L, "notifyGameEnd");
			lua_pushnumber(L, 2);
			lua_call(L, 1, 0);
			lua_pop(L, 0);

			std::string error = lua_tostring(L, -1);
			GP_ERROR(error.c_str());
		}
	}
}

Pause::~Pause()
{
//	delete cam;
	SAFE_RELEASE(font);
}

