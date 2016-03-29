#include "MainMenu.h"
#include "../Entity.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"
#include "../MyCamera.h"
#include "gameplay.h"

bool MainMenu::luaBinded = false;

MainMenu::MainMenu(Vector2 screenSize, Game* game)
{
	if (!luaBinded){
		luabridge::getGlobalNamespace(L)
			.deriveClass<MainMenu, Screen>("MainMenu")
			.endClass();
	}
	luaBinded = true;

	Bundle* bundle = Bundle::create("res/mainMenu/main_menu.gpb");

	_scene = bundle->loadScene();

	

	camera = new MyCamera(_scene, screenSize.x, screenSize.y);
	_scene->getActiveCamera()->getNode()->setRotation(Matrix::identity());
	_scene->getActiveCamera()->getNode()->setTranslation(0, 3.3f, 7);

	Texture* wallTex = Texture::create("res/mainMenu/brick.png", true);
	Texture* wallTexNorm = Texture::create("res/mainMenu/brick-normal.png", true);
	Texture* aboutTex = Texture::create("res/mainMenu/aboutGame.png");
	Texture* campaignTex = Texture::create("res/mainMenu/campaign.png");
	Texture* corkboardTex = Texture::create("res/mainMenu/corkboard.png");
	Texture* quickTex = Texture::create("res/mainMenu/quickGame.png");

	wall = _scene->findNode("Wall");
	Model* wallModel = (Model*)wall->getDrawable();
	Material* mat = Entity::buildMaterial(_scene, wallTex, Entity::TEXTURED_NORMAL, false, -1);
	wallModel->setMaterial(mat);
	Entity::applyNormalMap(mat, wallTexNorm);

	board = _scene->findNode("CorkBoard");
	Model* boardModel = (Model*)board->getDrawable();
	mat = Entity::buildMaterial(_scene, corkboardTex, Entity::TEXTURED, false, -1);
	boardModel->setMaterial(mat);

	campaign = _scene->findNode("Campaign");
	mat = Entity::buildMaterial(_scene, campaignTex, Entity::TEXTURED, false, -1);
	Model* model = (Model*)campaign->getDrawable();
	model->setMaterial(mat);
	BoundingBox campaignBox(Vector3(-0.65f, 0.48f, -0.5f), Vector3(0.65f, -0.48f, 0.5f));
	model->getMesh()->setBoundingBox(campaignBox);

	quickGame = _scene->findNode("Quick_Game");
	mat = Entity::buildMaterial(_scene, quickTex, Entity::TEXTURED, false, -1);
	model = (Model*)quickGame->getDrawable();
	model->setMaterial(mat);
	BoundingBox quickBox(Vector3(-0.65f, 0.48f, -0.5f), Vector3(0.65f, -0.48f, 0.5f));
	model->getMesh()->setBoundingBox(quickBox);

	about = _scene->findNode("About_Game");
	mat = Entity::buildMaterial(_scene, aboutTex, Entity::TEXTURED, false, -1);
	 model = (Model*)about->getDrawable();
	model->setMaterial(mat);
	BoundingBox aboutBox(Vector3(-0.48f, 0.65f, -0.5f), Vector3(0.48f, -0.65f, 0.5f));
	model->getMesh()->setBoundingBox(aboutBox);

	// may need to change the camera's position from here
	
}
void MainMenu::Update(const float& elapsedTime)
{

}
void MainMenu::Render()
{
	wall->getDrawable()->draw();
	board->getDrawable()->draw();
	campaign->getDrawable()->draw();
	quickGame->getDrawable()->draw();
	about->getDrawable()->draw();
}
void MainMenu::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	struct Touch{
		static bool test(Node* nodeToTest, MyCamera* camera, int x, int y){
			Ray* pickRay = camera->getPickRay(x, y);

			BoundingBox originalBox = ((Model*)nodeToTest->getDrawable())->getMesh()->getBoundingBox();
			BoundingBox box(originalBox); // copy constructor
			Matrix mat;
			mat.translate(nodeToTest->getTranslation());
			box.transform(mat);
			if (pickRay->intersects(box) != Ray::INTERSECTS_NONE){
				return true;
			}
			return false;
		};
	};

	if (evt != gameplay::Touch::TOUCH_PRESS)
		return;
	
	if (Touch::test(campaign, camera, x, y)){
		char luaCommand[] = "GAME:ShowOverMap()";
		int status = luaL_dostring(L, luaCommand);
		if (status != 0) {
			std::string error = lua_tostring(L, -1);
			GP_ERROR(error.c_str());
		}
	}
	else if (Touch::test(quickGame, camera, x, y)){
		//char* file = FileSystem::readAll("res/quickGame/level.lua");
		//int status = luaL_dostring(L, file);
		//if (status != 0) {
		//	std::string error = lua_tostring(L, -1);
		//	GP_ERROR(error.c_str());
		//}
		char luaCommand[] = "GAME:ShowDifficultySet()";
		int status = luaL_dostring(L, luaCommand);
		if (status != 0) {
			std::string error = lua_tostring(L, -1);
			GP_ERROR(error.c_str());
		}
	}
	else if (Touch::test(about, camera, x, y)){
		char luaCommand[] = "GAME:ShowAboutScreen()";
		int status = luaL_dostring(L, luaCommand);
		if (status != 0) {
			std::string error = lua_tostring(L, -1);
			GP_ERROR(error.c_str());
		}
	}
	else{
		int i = 7;
	}

	
}

MainMenu::~MainMenu()
{

}