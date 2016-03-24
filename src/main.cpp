#include "main.h"


#include "Dependencies\LuaBridge\LuaBridge.h"

#include "FSM.h"
#include "Entity.h"

#include "LuaLoader.h"
//#include "Overmap.h"

// screens
#include "Screens\StartScreen.h"
#include "Screens\OverMap.h"
#include "Screens\FoosballScreen.h"


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
		.addFunction("SaveProgress", &main::SaveProgress)
		.addFunction("ResetTimer", &main::ResetTimer)
		.endClass()
		.beginClass<Vector3>("Vector3")
		.addConstructor<void(*)(float, float, float)>()
		.endClass();
	
	luabridge::setGlobal(L, this, "GAME");
	
	//announcer = new Announcer();
	//luabridge::setGlobal(L, announcer, "announcer");

  /*  _scene = Scene::load("res/table.gpb");*/

	


//	table->Load("res/table.lua");

	
	
	//Game::getPhysicsController()->setGravity(Vector3(0, 0, -10));

	//
	//logoBatch = SpriteBatch::create("res/Logo.png");
	//starBackBatch = SpriteBatch::create("res/stars.png");

	////loadScene = NULL;
 //   _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

//	defaultFB = FrameBuffer::getCurrent();
//	RenderTarget* target = RenderTarget::create("banana", Texture::create(Image::create(1024, 1024, Image::RGBA)));
//	defaultFB->setRenderTarget(target);
//	renderTarget = RenderTarget::create("shadow", Texture::create(Image::create(1024, 1024, Image::RGBA)));
//	depthTarget = DepthStencilTarget::create("shadowDepth", DepthStencilTarget::DEPTH_STENCIL, 1024, 1024);

//	shadowFB = FrameBuffer::create("shadow");
//	shadowFB->setRenderTarget(renderTarget);
//	shadowFB->setDepthStencilTarget(depthTarget);
	

//	depthEffect = Effect::createFromFile("res/shaders/depth.vert", "res/shaders/depth.frag");
//	shadowMat = buildMaterial(_scene, NULL, SHADOW, false, -1);

	//opponent = new Opponent(_scene, table);
	//luabridge::setGlobal(L, opponent, "opponent");
	
//	opponent->Load("res/opponent.lua");

//	player->Load("res/player.lua");

//	std::string what;
	int status = luaL_dofile(L, "res/start.lua");
	
//	what = lua_tostring(L, -1);
	
	
	//camera->SetTarget(table);
	int banana = 7;
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
    if (evt == Keyboard::KEY_PRESS)
    {
		switch (key)
		{
		case Keyboard::KEY_ESCAPE:
			exit();
			break;
		}
    }
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

	currentScreen = new FoosballScreen(Vector2(getWidth(), getHeight()), this);

//	int status = luaL_dofile(L, file.c_str());

	ResetTimer();
	return (FoosballScreen*)currentScreen;

//	openLocations.clear();
//	locationFiles.clear();
//	SAFE_DELETE(overmapBatch);

}

void main::Wait(float milliseconds)
{
//	luabridge::LuaRef co = luabridge::getGlobal(L, "co");
//	timer = 0;
//	tTarget = milliseconds;
	//lua_yield(L, 0);
	
	int i = 7;
}

void main::WaitForTime(float milliseconds)
{

	wakeUpTime = milliseconds;

}

void main::checkForLuaWakeUp(const float& elapsedTime)
{
	// we may get rid of this type soon
	//timer += elapsedTime;
	//if (tTarget > 0 && timer > tTarget || banana > 0 && timer > banana){
	//	tTarget = -1;
	//	luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
	//	resume();
	//	//int code = lua_resume(L, NULL, 1);
	//	//std::string banana = lua_tostring(L, -1);
	//	
	//}

	// ok, new part, where we block the times out
//	luabridge::LuaRef wakeUpTimeRef = luabridge::getGlobal(L, "wakeUpTime");
//	float wakeUpTime = wakeUpTimeRef;
	if (wakeUpTime > 0 && eventTimer >= wakeUpTime){
		wakeUpTime = 0;
		luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
		resume();
	//	int status = lua_resume(L, L, 0);
	//	int banana = 7;
	}
}

//void main::StartRound()
//{
//	MessageDispatcher::Instance()->dispatchMessage(0, NULL, table, FSM::START_ROUND, NULL);
//	MessageDispatcher::Instance()->dispatchMessage(0, NULL, camera, FSM::START_ROUND, table);
//	roundRunning = true;
//}

//void main::SwoopLogo(int milliseconds)
//{
//	logoTop = new MyCurve(Curve::LINEAR);
//	logoBottom = new MyCurve(Curve::LINEAR);
//
//	logoTop->addPoint(0, -1 * (float)getWidth());
//	logoTop->addPoint(milliseconds / 3, -0.25f * (float)getWidth());
//	logoTop->addPoint(milliseconds / 3, (float)getWidth() / 4);
//	logoTop->addPoint(milliseconds / 3, getWidth());
//
//	logoBottom->addPoint(0, getWidth());
//	logoBottom->addPoint(milliseconds / 3, (float)getWidth() / 4);
//	logoBottom->addPoint(milliseconds / 3, -0.25f * (float)getWidth());
//	logoBottom->addPoint(milliseconds / 3, -1 * (float)getWidth());
//
//	starAlpha = 0.1f;
//	starOffset.set(-512, -512);
//}

void main::PlayMusic(std::string file)
{
	if (currentMusic)
		currentMusic->release();

	currentMusic = AudioSource::create(file.c_str(), true);
	currentMusic->play();
	
	
}

void main::PlaySound(std::string file)
{
	int i = 7;
}
StartScreen* main::ShowLogo()
{
	currentScreen = new StartScreen(Vector2(getWidth(), getHeight()));
	return (StartScreen*)currentScreen;
}
//void main::ShowClickToStart()
//{
//	
//}
OverMap* main::ShowOverMap()
{
	gameStatus = OVERMAP;

	if (currentScreen)
		SAFE_DELETE(currentScreen);

	currentScreen = new OverMap(Vector2(getWidth(), getHeight()), this);

//	if (!overmap)
//		overmap = new Overmap(Vector2(getWidth(), getHeight()), this);

	// clear table, player, and opponent here
	// // //
	//table->Clear();
	//opponent->Clear();
	//player->Clear();

	//if (splashBatch)
	//	SAFE_DELETE(splashBatch);
	return (OverMap*)currentScreen;
}

//void main::addLevelFlier(int index, Scene* scene, bool animate)
//{
//	FrameBuffer* defaultFB = FrameBuffer::getCurrent();
//	
//	LuaLoader loader(L);
//	loader.setFile("res/levelList.lua");
//	luabridge::LuaRef levelTable = loader.getTable("LevelList");
////	for (int i = 0; i < progress; ++i){
//		luabridge::LuaRef level = loader.getTable(index + 1, levelTable);
//
//		luabridge::LuaRef placement = loader.getTable("placement", level);
//		float x = loader.getFloat(1, placement);
//		float y = loader.getFloat(2, placement);
//
//		Level temp;
//		temp.pos.set(x, y);
//		temp.levelFile = loader.getString("levelFile", level);
//		temp.image = loader.getString("image", level);
//		temp.text = loader.getString("text", level);
//
//		temp.node = loadScene->findNode("Paper")->clone();
//
//		//temp.shadow = Node::create("shadow");
//
//		FrameBuffer* tempFB = FrameBuffer::create("tempFB", 1024, 1024);
//		Texture* tempTex = Texture::create(1024, 1024, Image::RGBA);
//		RenderTarget* target = RenderTarget::create("tempRT", 1024, 1024);
//		Image* tempImg = Image::create(1024, 1024, Image::RGBA);
//		//defaultFB->setRenderTarget(target);
//		tempFB->setRenderTarget(target);
//		tempFB->bind();
//		//	Image* screenShot = tempFB->createScreenshot();
//
//		clear(CLEAR_COLOR_DEPTH, Vector4(1, 1, 1, 1), 1.0f, 0);
//
//		// render to a texture
//		startFont->start();
//		startFont->drawText(temp.text.c_str(), gameplay::Rectangle(20, 0, 980, 700), Vector4(0, 0, 0, 1), startFont->getSize() * 8, Font::Justify::ALIGN_TOP_LEFT, true, false, gameplay::Rectangle(20, 0, 980, 700));
//		startFont->finish();
//
//		test = target->getTexture();
//
//		Model* flierModel = (Model*)temp.node->getDrawable();
//		flierModel->setMaterial(Entity::buildMaterial(loadScene, Texture::create(temp.image.c_str()), Entity::TEXTURED, false, -1), 0); // image
//		flierModel->setMaterial(Entity::buildMaterial(loadScene, target->getTexture(), Entity::TEXTURED, false, -1), 1); // text
//		flierModel->setMaterial(Entity::buildMaterial(loadScene, target->getTexture(), Entity::TEXTURED, false, -1), 2); // border
//
//		Vector3 trans(temp.pos.x, temp.pos.y, 0.133);
//		BoundingBox box(Vector3(-1, -1.3, -0.5), Vector3(1, 1.3, 0.5));
//
//		//	Matrix mat;
//		//	mat.translate(trans);
//		//	box.transform(mat);
//		flierModel->getMesh()->setBoundingBox(box);
//
//		temp.node->setTranslation(trans);
//
//
//		scene->addNode(temp.node);
//
//		levelList.push_back(temp);
//
//
////	}
//
//	defaultFB->bind();
//
//	// set target for camera
//	MessageDispatcher::Instance()->dispatchMessage(0, NULL, loadCamera, FSM::SET_TARGET, &trans);
//}

void main::SaveProgress(int moveAheadLevel, int currentLevel)
{
	if (moveAheadLevel && currentLevel + 1 > progress && progress < 7)
		progress++;

//	if (moveAheadLevel && progress < 7)
//		progress++;

	std::string pWord = 
		Entity::to_string(progress);

	std::auto_ptr<Stream> stream(FileSystem::open("save.txt", FileSystem::WRITE));
	if (stream.get() != NULL && stream->canWrite())
	{
		stream->write(pWord.c_str(), 1, pWord.size());
	}
}

//void main::gestureTapEvent(int x, int y)
//{
//
//}