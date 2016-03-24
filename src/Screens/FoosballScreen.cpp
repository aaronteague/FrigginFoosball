#include "FoosballScreen.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"
#include "../FSM.h"

#include "../Opponent.h"
#include "../Table.h"
#include "../MyCamera.h"
#include "../Player.h"
#include "LogoSwoop.h"


#include "../Announcer.h"

#include "../States\CameraStage.h"

bool FoosballScreen::luaBinded = false;

FoosballScreen::FoosballScreen(Vector2 screenSize, Game* game)
:screenSize(screenSize), game(game)
{
	if (!luaBinded){
		luabridge::getGlobalNamespace(L)
			.deriveClass<FoosballScreen, Screen>("FoosballScreen")
			.addFunction("SwoopLogo", &FoosballScreen::SwoopLogo)
			.addFunction("StartRound", &FoosballScreen::StartRound)
			.endClass();
	}
	luaBinded = true;

	announcer = new Announcer();
	luabridge::setGlobal(L, announcer, "announcer");

	_scene = Scene::load("res/table.gpb");

	camera = new MyCamera(_scene, screenSize.x, screenSize.y);
	camera->SetState("Camera_Stage");
	luabridge::setGlobal(L, camera, "camera");

	table = new Table(_scene, screenSize, camera);
	luabridge::setGlobal(L, table, "gameTable");

	game->getPhysicsController()->setGravity(Vector3(0, 0, -10));




	//loadScene = NULL;
	_scene->getActiveCamera()->setAspectRatio(game->getAspectRatio());

	opponent = new Opponent(_scene, table);
	luabridge::setGlobal(L, opponent, "opponent");

	player = new Player(_scene, table, camera);
	luabridge::setGlobal(L, player, "player");
}
void FoosballScreen::Update(const float& elapsedTime)
{
	table->Update(elapsedTime);
	opponent->Update(elapsedTime);
	player->Update(elapsedTime);
	camera->Update(elapsedTime);

	if (logoSwoop){
		logoSwoop->Update(elapsedTime);
		if (logoSwoop->isFinished())
			SAFE_DELETE(logoSwoop);
	}

	//if (starAlpha > 0)
	//	updateLogoSwoop(elapsedTime);


}
void FoosballScreen::Render()
{
	_scene->visit(this, &FoosballScreen::drawScene);

	table->Render();

	// let's draw the logo swoop if there
	if (logoSwoop)
		logoSwoop->Render();
}
void FoosballScreen::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	//if (gameStatus == GAME_RUNNING)
		table->touchEvent(evt, x, y, contactIndex);
}

//void FoosballScreen::updateLogoSwoop(const float& elapsedTime)
//{
//
//}

bool FoosballScreen::drawScene(Node* node)
{
	// If the node visited contains a drawable object, draw it
	Drawable* drawable = node->getDrawable();

	if (drawable)
		drawable->draw();

	return true;
}

void FoosballScreen::StartRound()
{
	MessageDispatcher::Instance()->dispatchMessage(0, NULL, table, FSM::START_ROUND, NULL);
	MessageDispatcher::Instance()->dispatchMessage(0, NULL, camera, FSM::START_ROUND, table);
	roundRunning = true;
}

void FoosballScreen::SwoopLogo(int milliseconds)
{
	logoSwoop = new LogoSwoop(screenSize, milliseconds);
}

void FoosballScreen::keyEvent(Keyboard::KeyEvent evt, int key)
{
	table->keyEvent(evt, key);
}