#include "main.h"


#include "Dependencies\LuaBridge\LuaBridge.h"

#include "FSM.h"
#include "Entity.h"

#include "LuaLoader.h"


// screens
#include "Screens\StartScreen.h"
#include "Screens\OverMap.h"
#include "Screens\FoosballScreen.h"
#include "Screens\CreditsScreen.h"
#include "Screens\MainMenu.h"
#include "Screens\AboutScreen.h"
#include "Screens\DifficultySet.h"

#include "Opponent.h"
#include "Player.h"
#include "Table.h"
#include "MyCamera.h"


//#include "States\CameraOvermap.h"

// Declare our game instance
main game;



main::main()    
{
}

void main::initialize()
{
//	Game::registerGesture(Gesture::GESTURE_ANY_SUPPORTED);

	L = luaL_newstate();
	
	luaL_openlibs(L);
	L = lua_newthread(L);

	// bind the main class's functions
	luabridge::getGlobalNamespace(L)
		.beginClass<main>("main")
	//	.addData("wakeUpTime", &main::wakeUpTime)
		.addFunction("StartGame", &main::StartGame)
		.addFunction("Wait", &main::Wait)
		.addFunction("WaitForTime", &main::WaitForTime)
		//	.addFunction("StartRound", &main::StartRound)
		//	.addFunction("SwoopLogo", &main::SwoopLogo)
		.addFunction("PlayMusic", &main::PlayMusic)
		.addFunction("PlaySound", &main::PlaySound)
		.addFunction("ShowLogo", &main::ShowLogo)
		//	.addFunction("ShowClickToStart", &main::ShowClickToStart)
		.addFunction("ShowOverMap", &main::ShowOverMap)
		.addFunction("ShowMainMenu", &main::ShowMainMenu)
		.addFunction("ShowAboutScreen", &main::ShowAboutScreen)
		.addFunction("SaveProgress", &main::SaveProgress)
		.addFunction("ResetTimer", &main::ResetTimer)
		.addFunction("ShowDifficultySet", &main::ShowDifficultySet)
		.endClass()
		.beginClass<Vector3>("Vector3")
		.addConstructor<void(*)(float, float, float)>()
		.endClass();
	
	luabridge::setGlobal(L, this, "GAME");

	Scene* _scene = Scene::load("res/table.gpb");

	MyCamera* camera = new MyCamera(_scene, getWidth(), getHeight());
	camera->SetState("Camera_Stage");
	luabridge::setGlobal(L, camera, "camera");

	table = new Table(_scene, Vector2(getWidth(), getHeight()), camera);
	luabridge::setGlobal(L, table, "gameTable");

	opponent = new Opponent(_scene, table);
	luabridge::setGlobal(L, opponent, "opponent");

	player = new Player(_scene, table, camera);
	luabridge::setGlobal(L, player, "player");

	lua_atpanic(L, panic);
	

	

	char* file = FileSystem::readAll("res/start.lua");


	int status = luaL_dostring(L, file);
	if (status != 0){		
		std::string error = lua_tostring(L, -1);
		GP_ERROR(error.c_str());
	}
	delete[] file;

	
}

void main::finalize()
{
    
}

void main::update(float elapsedTime)
{
	
	if (elapsedTime < 100)
		eventTimer += elapsedTime;

	checkForLuaWakeUp(elapsedTime);

//	if (gameStatus == OVERMAP)
//		overmap->Update(elapsedTime);
	if (currentScreen)
		currentScreen->Update(elapsedTime);

	//if (gameStatus != GAME_RUNNING)
	//	return;


    // Rotate model
 //   _scene->findNode("Table")->rotateY(MATH_DEG_TO_RAD((float)elapsedT100ime / 1000.0f * 180.0f));
	
	// add force to the ball towards center


	// make poles slowly fall back down
//	Quaternion rotation = p1Handles[0]->getRotation();
//	float angle = rotation.toAxisAngle(&Vector3(0, 1, 0));
	

	if (creditsScreen){
		creditsScreen->Update(elapsedTime);
		if (creditsScreen->getIsFinished())
			SAFE_DELETE(creditsScreen);
	}

	
	if (removeScreen)
		SAFE_DELETE(removeScreen);
}



void main::render(float elapsedTime)
{
    // Clear the color and depth buffers
//	defaultFB->bind();
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
   /* _scene->visit(this, &main::drawScene);*/
	// draw shadows

	//if (gameStatus == GAME_RUNNING)
	//	table->Render();

	//// let's draw the logo swoop if there
	//if (starAlpha > 0){
	//	starBackBatch->start();
	//	for (int i = 0; i < 5; ++i)
	//	for (int j = 0; j < 5; ++j)
	//		starBackBatch->draw(gameplay::Rectangle(starOffset.x + 512 * i, starOffset.y + 512 * j, 512, 512), gameplay::Rectangle(0, 0, 512, 512), Vector4(1,1,1, starAlpha));

	//	starBackBatch->finish();

	//	logoBatch->start();
	//	logoBatch->draw(gameplay::Rectangle(topX, 0, getWidth(), getHeight() / 2), gameplay::Rectangle(0, 0, 1917, 555));
	//	logoBatch->draw(gameplay::Rectangle(bottomX, getHeight() / 2, getWidth(), getHeight() / 2), gameplay::Rectangle(0, 555, 1917, 525));
	//	logoBatch->finish();
	//}

	if (currentScreen) // splash screen
		currentScreen->Render();

	if (creditsScreen)
		creditsScreen->Render();

	//if (overmapBatch){
	//	overmapBatch->start();
	//	overmapBatch->draw(gameplay::Rectangle(0, 0, getWidth(), getHeight()), gameplay::Rectangle(0, 712, 2048, 1336));
	//	for (unsigned int i = 0; i < openLocations.size(); ++i)
	//		overmapBatch->draw(gameplay::Rectangle(openLocations[i].x, openLocations[i].y, 100, 100), gameplay::Rectangle(0, 0, 200, 200));
	//	overmapBatch->finish();

	//}

	//if (gameStatus == OVERMAP)
	//	overmap->Render();

//	Game::getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());
//	shadowFB->bind();
//	clear(CLEAR_COLOR_DEPTH_STENCIL, Vector4(0, 0, 1, 1), 1.0f, 0);

	// opengl code
//	GLuint FrameBufferName = 0;
//	glGenFramebuffers(1, &FrameBufferName);
//	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferName);

//	GLuint depthTexture;

//	Image::create(1024, 1024, Image::RGB, &depthTexture);


//	depthEffect->bind();
	// Visit all the nodes in the scene for drawing
//	_scene->visit(this, &main::drawShadow);
//	Image* screenShot = FrameBuffer::createScreenshot();

//	defaultFB->bind();
//	Texture* screenTex = Texture::create(screenShot);
//	SpriteBatch* batch = SpriteBatch::create(renderTarget->getTexture());
//	batch->start();
//	batch->draw(Rectangle(0, 0, 100, 100), Rectangle(0, 0, 1024, 1024));
//	batch->finish();
//	screenShot->release();
//	screenTex->release();
//	delete(batch);

}

//bool main::drawScene(Node* node)
//{
//    // If the node visited contains a drawable object, draw it
//    Drawable* drawable = node->getDrawable(); 
//	
//    if (drawable)
//        drawable->draw(_wireframe);
//
//    return true;
//}

//bool main::drawShadow(Node* node)
//{
//	// If the node visited contains a drawable object, draw it
//
//	Model* model = ((Model*)node->getDrawable());
//	if (model){
//	//	Material* defaultMat = model->getMaterial();
//	//	model->setMaterial(shadowMat);
//
//
//		model->draw(_wireframe);
//
////		model->setMaterial(defaultMat);
//	}
//
//	return true;
//}

void main::keyEvent(Keyboard::KeyEvent evt, int key)
{
 //   if (evt == Keyboard::KEY_PRESS)
 //   {
	//	switch (key)
	//	{
	//	case Keyboard::KEY_ESCAPE:
	//		exit();
	//		break;
	//	}
 //   }
	if (gameStatus == GAME_RUNNING){
		((FoosballScreen*)currentScreen)->keyEvent(evt, key);
	}
	//table->keyEvent(evt, key);
}

void main::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{

	
	if (currentScreen)
		currentScreen->touchEvent(evt, x, y, contactIndex);


	//if (openLocations.size() > 0)
	//	for (unsigned int i = 0; i < openLocations.size(); ++i)
	//		if (gameplay::Rectangle(openLocations[i].x, openLocations[i].y, 50, 50).contains(x, y)){
	//			StartGame(locationFiles[i]);

	//			return;
	//		}



//	if (gameStatus == OVERMAP)
//		overmap->touchEvent(evt, x, y, contactIndex);
	

//	if (gameStatus == GAME_RUNNING)
//		table->touchEvent(evt, x, y, contactIndex);
}





std::string main::to_string(int number)
{
	std::ostringstream os;
	os << number;
	return os.str();
}

FoosballScreen* main::StartGame()
{
	gameStatus = GAME_RUNNING;

	if (currentScreen)
		SAFE_DELETE(currentScreen);

	table->Clear();
	currentScreen = new FoosballScreen(Vector2(getWidth(), getHeight()), this, table, opponent, player);



	ResetTimer();
	return (FoosballScreen*)currentScreen;


}

void main::Wait(float milliseconds)
{

	
	int i = 7;
}

void main::WaitForTime(float milliseconds)
{

	wakeUpTime = milliseconds;

}

void main::checkForLuaWakeUp(const float& elapsedTime)
{

	if (wakeUpTime > 0 && eventTimer >= wakeUpTime){
		wakeUpTime = 0;
		luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
		resume();

	}
}



void main::PlayMusic(std::string title)
{
	if (currentMusic)
		currentMusic->release();

	LuaLoader loader(L);
	loader.setFile("res/music/SongList.lua");
	luabridge::LuaRef songList = loader.getTable("SONG_LIST");
	luabridge::LuaRef song = loader.getTable(title, songList);
	std::string file = loader.getString("FILE", song);
	std::string credits = loader.getString("CREDIT", song);

	creditsScreen = new CreditsScreen(Vector2(getWidth(), getHeight()), credits);

	currentMusic = AudioSource::create(file.c_str(), true);
	currentMusic->play();
	
	
}

void main::PlaySound(std::string file)
{
	
}
StartScreen* main::ShowLogo()
{
	currentScreen = new StartScreen(Vector2(getWidth(), getHeight()));
	return (StartScreen*)currentScreen;
}

OverMap* main::ShowOverMap()
{
	gameStatus = OVERMAP;

	if (currentScreen)
		removeScreen = currentScreen;
//		delete(currentScreen);

	currentScreen = new OverMap(Vector2(getWidth(), getHeight()), this);

	return (OverMap*)currentScreen;
}

MainMenu* main::ShowMainMenu()
{
	gameStatus = MAIN_MENU;

	if (currentScreen)
		removeScreen = currentScreen;

	currentScreen = new MainMenu(Vector2(getWidth(), getHeight()), this);

	return (MainMenu*)currentScreen;
}


AboutScreen* main::ShowAboutScreen()
{
	gameStatus = ABOUT_SCREEN;

	if (currentScreen)
		removeScreen = currentScreen;

	currentScreen = new AboutScreen(Vector2(getWidth(), getHeight()), this);

	return (AboutScreen*)currentScreen;
}

DifficultySet* main::ShowDifficultySet()
{
	gameStatus = DIFFICULTY_SET;

	if (currentScreen)
		removeScreen = currentScreen;

	currentScreen = new DifficultySet(Vector2(getWidth(), getHeight()), opponent);

	return (DifficultySet*)currentScreen;
}

// moveAhead is actually a boolean
void main::SaveProgress(int moveAheadLevel, int currentLevel)
{
	
	
	char levelIndex[10];
	FileSystem::open("res/save.txt", FileSystem::READ)->readLine(levelIndex, 10);
	progress = atoi(levelIndex);

	if (moveAheadLevel && currentLevel + 1 > progress && progress < 7)
		progress++;



	std::string pWord = 
		Entity::to_string(progress);

	std::auto_ptr<Stream> stream(FileSystem::open("res/save.txt", FileSystem::WRITE));
	if (stream.get() != NULL && stream->canWrite())
	{
		stream->write(pWord.c_str(), 1, pWord.size());
	}
}

