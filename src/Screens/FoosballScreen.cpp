#include "FoosballScreen.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"
#include "../FSM.h"

#include "../Opponent.h"
#include "../Table.h"
#include "../MyCamera.h"
#include "../Player.h"
#include "LogoSwoop.h"
#include "ShutterScreen.h"
#include "AdvertScreen.h"
#include "Pause.h"


#include "../Announcer.h"

#include "../States\CameraStage.h"

bool FoosballScreen::luaBinded = false;

FoosballScreen::FoosballScreen(Vector2 screenSize, Game* game, Table* table, Opponent* opponent, Player* player)
:screenSize(screenSize), game(game), player(player), opponent(opponent), table(table)
{
	if (!luaBinded){
		luabridge::getGlobalNamespace(L)
			.deriveClass<FoosballScreen, Screen>("FoosballScreen")
			.addFunction("SwoopLogo", &FoosballScreen::SwoopLogo)
			.addFunction("StartRound", &FoosballScreen::StartRound)
			.addFunction("ShowAds", &FoosballScreen::ShowAds)
			.endClass();
	}
	luaBinded = true;

	announcer = new Announcer();
	luabridge::setGlobal(L, announcer, "announcer");

	_scene = table->getScene();

	camera = table->getCamera();



	game->getPhysicsController()->setGravity(Vector3(0, 0, -10));




	//loadScene = NULL;
	_scene->getActiveCamera()->setAspectRatio(game->getAspectRatio());



	//////// UNCOMMENT THIS PART FOR RELEASE ///////////////
	/*advertScreen = new AdvertScreen(screenSize, game); */
	childScreen = new ShutterScreen(screenSize);
}

FoosballScreen::~FoosballScreen()
{
//	delete(announcer);
	
//	delete(camera);
//	delete(table);
//	delete(opponent);
//	delete(player);
//	if (childScreen)
//		delete(childScreen);
//	_scene->release();
	
}
void FoosballScreen::Update(const float& elapsedTime)
{
	table->Update(elapsedTime);
	if (opponent->getNode())
		opponent->Update(elapsedTime);
	if (player->getNode())
		player->Update(elapsedTime);
	camera->Update(elapsedTime);

	if (childScreen){
		childScreen->Update(elapsedTime);
		if (childScreen->getIsFinished() && childScreen == advertScreen) {
			childScreen = NULL;
			luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
			resume();
		}
		else if (childScreen->getIsFinished()) {
			if (childScreen == pauseScreen)
				ResumeGame();
			SAFE_DELETE(childScreen);
		}
	}

	//if (starAlpha > 0)
	//	updateLogoSwoop(elapsedTime);


}
void FoosballScreen::Render()
{
	if ((childScreen && childScreen != pauseScreen) || !childScreen) {
		_scene->visit(this, &FoosballScreen::drawScene);

		table->Render();
	}

	// let's draw the logo swoop if there
	if (childScreen)
		childScreen->Render();
}
void FoosballScreen::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	if (childScreen)
		childScreen->touchEvent(evt, x, y, contactIndex);
	else
		table->touchEvent(evt, x, y, contactIndex);
}

//void FoosballScreen::updateLogoSwoop(const float& elapsedTime)
//{
//
//}

bool FoosballScreen::drawScene(Node* node)
{
	std::string trueStr("true");
	// if doesn't receive tag or if visible tag is true
	node->getTag("visible");
	if (!node->getTag("visible") || trueStr.compare(node->getTag("visible")) == 0){ 
		Drawable* drawable = node->getDrawable();

		if (drawable)
			drawable->draw();
	}

	return true;
}

void FoosballScreen::StartRound()
{
	MessageDispatcher::Instance()->dispatchMessage(0, NULL, table, FSM::START_ROUND, NULL);
	MessageDispatcher::Instance()->dispatchMessage(0, NULL, camera, FSM::START_ROUND, table);
	if (opponent->getNode()) // getNode() will check 
		MessageDispatcher::Instance()->dispatchMessage(0, NULL, opponent, FSM::START_ROUND, NULL);
	if (player->getNode())
		player->MakeInvisible();
	if (player->getNode())
		player->setAnimState("Idle");
	if (opponent->getNode())
	opponent->setAnimState("Idle");
	roundRunning = true;
	
}

void FoosballScreen::SwoopLogo(int milliseconds)
{
	if (childScreen)
		SAFE_DELETE(childScreen);

	childScreen = new LogoSwoop(screenSize, milliseconds);
	if (player->getNode())
		player->MakeInvisible();
	if (opponent->getNode())
		opponent->FadeToHands();
	MessageDispatcher::Instance()->dispatchMessage(0, NULL, camera, FSM::START_ROUND, table);
}

void FoosballScreen::ShowAds()
{
	advertScreen->ShowAds();
	childScreen = advertScreen;
	//luabridge::LuaRef pause = luabridge::getGlobal(L, "pause");
	//pause();
//	lua_getglobal(L, "pause");
//	lua_call(L, 0, 0);

}

void FoosballScreen::PauseGame()
{
	childScreen = pauseScreen = new Pause(screenSize, 0, 0, table, camera);
	pauseVelocity = ((PhysicsRigidBody*)table->getBall()->getCollisionObject())->getLinearVelocity();
	((PhysicsRigidBody*)table->getBall()->getCollisionObject())->setKinematic(true);
	
}

void FoosballScreen::ResumeGame()
{
	// do some stuff
	((PhysicsRigidBody*)table->getBall()->getCollisionObject())->setKinematic(false);
	((PhysicsRigidBody*)table->getBall()->getCollisionObject())->setLinearVelocity(pauseVelocity);
	SAFE_DELETE(pauseScreen);
	childScreen = NULL;
}

void FoosballScreen::keyEvent(Keyboard::KeyEvent evt, int key)
{
	if (childScreen == NULL && (key == Keyboard::KEY_BACKSPACE || key == Keyboard::KEY_ESCAPE)) {
		PauseGame();
	}
	table->keyEvent(evt, key);
}