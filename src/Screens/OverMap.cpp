#include "OverMap.h"
#include "../MyCamera.h"
#include "../FSM.h"
#include "../LuaLoader.h"
#include "../Dependencies\LuaBridge\LuaBridge.h"


bool OverMap::luaBinded = false;

OverMap::OverMap(Vector2 screenSize, Game* game)
:screenSize(screenSize), game(game)
{
	if (!luaBinded){
		luabridge::getGlobalNamespace(L)
			.deriveClass<OverMap, Screen>("OverMap")
			.endClass();
	}
	luaBinded = true;

	//	gameRunning = false;

	Bundle* bund = Bundle::create("res/corkboard.gpb");
	_scene = bund->loadScene();

	camera = new MyCamera(_scene, screenSize.x, screenSize.y);
	camera->SetState("Overmap");

	for (int i = 0; i < 5; ++i)
	for (int j = 0; j < 5; ++j){
		Node* node = _scene->findNode("Board")->clone();
		Material* corkMat = Entity::buildMaterial(_scene, Texture::create("res/cork-diffuse.png"), Entity::TEXTURED_NORMAL, false, -1);
		Entity::applyNormalMap(corkMat, Texture::create("res/cork-normal.png", true));
		corkMat->getParameter("u_specularExponent")->setValue(10.0f);
		corkMat->getParameter("u_ambientColor")->setValue(Vector3(0.3f, 0.3f, 0.3f));
		((Model*)node->getDrawable())->setMaterial(corkMat);
		node->setTranslation(i * 6, j * 6, 0);
		_scene->addNode(node);
	}

	Node* node = _scene->findNode("Paper");
	Model* paperModel = (Model*)node->getDrawable();
	paperModel->setMaterial(Entity::buildMaterial(_scene, Texture::create("res/pic.png"), Entity::TEXTURED, false, -1), 0);
	paperModel->setMaterial(Entity::buildMaterial(_scene, Texture::create("res/text.png"), Entity::TEXTURED, false, -1), 1);
	paperModel->setMaterial(Entity::buildMaterial(_scene, Texture::create("res/text.png"), Entity::TEXTURED, false, -1), 2);


	node = _scene->findNode("Tack");
	((Model*)node->getDrawable())->setMaterial(Entity::buildMaterial(_scene, NULL, Entity::COLORED, false, -1));

	font = Font::create("res/arial.gpb");


	//	_scene->getActiveCamera()->getNode()->setRotation(Matrix::identity());
	//	_scene->getActiveCamera()->getNode()->setTranslation(0, 0, 20);






	//if (startFont)
	//	startFont->release();
	//startFont = NULL;

	//	overmapBatch = SpriteBatch::create("res/overmap.png");


	progress = 0;
	if (FileSystem::fileExists("save.txt")){
		char levelIndex[10];
		FileSystem::open("save.txt", FileSystem::READ)->readLine(levelIndex, 10);

		progress = atoi(levelIndex);
	}
	//	FrameBuffer* defaultFB = FrameBuffer::getCurrent();
	//	RenderTarget* defaultRT = defaultFB->getRenderTarget();

	for (int i = 0; i < progress; ++i){
		if (i == progress - 1)
			addLevelFlier(i, _scene, true);
		else
			addLevelFlier(i, _scene, false);
	}

	bund->release();
}

void OverMap::Update(const float& elapsedTime)
{
	if (targetAnimate){
		if (targetAnimate->nodeAlpha < 1)
			targetAnimate->nodeAlpha += 0.01f;
		targetAnimate->shadowAlpha = 5.0f / (5.0f - targetAnimate->node->getTranslationZ() - 0.5f); // shadow
		if (!targetAnimate->clip->isPlaying()){
			targetAnimate->pinAlpha += 0.01f;

			//targetAnimate->pinAlpha = 1.0f;
			//targetAnimate->shadowAlpha = 1.0f;

			if (targetAnimate->pinAlpha >= 0.5f)
				targetAnimate = NULL;
		}

		if (targetAnimate)
			((Model*)targetAnimate->pin->getDrawable())->getMaterial()->getParameter("u_modulateAlpha")->setValue(targetAnimate->pinAlpha);
	}

	camera->Update(elapsedTime);
}

void OverMap::Render()
{
	//_scene->findNode("Board")->getDrawable()->draw();
	std::vector<Node*> boards;
	_scene->findNodes("Board", boards, false, true);
	for (unsigned int i = 0; i < boards.size(); ++i)
		boards[i]->getDrawable()->draw();
	//	_scene->findNode("Tack")->getDrawable()->draw();

	for (unsigned int i = 0; i < levelList.size(); ++i){
		levelList[i].shadow->getDrawable()->draw();
		levelList[i].node->getDrawable()->draw();
		levelList[i].pin->getDrawable()->draw();

	}
}

void OverMap::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	if (evt == Touch::TOUCH_PRESS){
		touchDownTime = Game::getGameTime();
		touchDownPos.set(x, y); // in pixels
	}
	else if (evt == Touch::TOUCH_RELEASE
		&& Game::getGameTime() - touchDownTime < 300
		&& (touchDownPos - Vector2(x, y)).length() < 50){
		if (gestureTapEvent(x, y))
			return;

	}

	Vector2 touchDown(x, y);
	if (evt == Touch::TOUCH_PRESS || evt == Touch::TOUCH_MOVE)
		MessageDispatcher::Instance()->dispatchMessage(0, NULL, camera, FSM::TOUCH_DOWN, &touchDown);
	else if (evt == Touch::TOUCH_RELEASE)
		MessageDispatcher::Instance()->dispatchMessage(0, NULL, camera, FSM::TOUCH_RELEASE, NULL);
}


void OverMap::addLevelFlier(int index, Scene* scene, bool animate)
{
	FrameBuffer* defaultFB = FrameBuffer::getCurrent();

	LuaLoader loader(L);
	loader.setFile("res/levelList.lua");
	luabridge::LuaRef levelTable = loader.getTable("LevelList");
	//	for (int i = 0; i < progress; ++i){
	luabridge::LuaRef level = loader.getTable(index + 1, levelTable);

	luabridge::LuaRef placement = loader.getTable("placement", level);
	float x = loader.getFloat(1, placement);
	float y = loader.getFloat(2, placement);

	Level temp;
	temp.pos.set(x, y);
	temp.levelFile = loader.getString("levelFile", level);
	temp.image = loader.getString("image", level);
	temp.text = loader.getString("text", level);

	temp.node = _scene->findNode("Paper")->clone();

	//temp.shadow = Node::create("shadow");

	FrameBuffer* tempFB = FrameBuffer::create("tempFB", 1024, 1024);
	Texture* tempTex = Texture::create(1024, 1024, Image::RGBA);
	RenderTarget* target = RenderTarget::create("tempRT", 1024, 1024);
	Image* tempImg = Image::create(1024, 1024, Image::RGBA);
	//defaultFB->setRenderTarget(target);
	tempFB->setRenderTarget(target);
	tempFB->bind();
	//	Image* screenShot = tempFB->createScreenshot();



	game->clear(Game::CLEAR_COLOR_DEPTH, Vector4(1, 1, 1, 1), 1.0f, 0);

	// render to a texture
	font->start();
	font->drawText(temp.text.c_str(), gameplay::Rectangle(20, 0, 980, 700), Vector4(0, 0, 0, 1), font->getSize() * 8, Font::Justify::ALIGN_TOP_LEFT, true, false, gameplay::Rectangle(20, 0, 980, 700));
	font->finish();

	//	test = target->getTexture();

	Model* flierModel = (Model*)temp.node->getDrawable();
	flierModel->setMaterial(Entity::buildMaterial(_scene, Texture::create(temp.image.c_str()), Entity::TEXTURED_ANIMATED, false, 10), 0); // image
	flierModel->setMaterial(Entity::buildMaterial(_scene, target->getTexture(), Entity::TEXTURED_ANIMATED, false, 10), 1); // text
	flierModel->setMaterial(Entity::buildMaterial(_scene, target->getTexture(), Entity::TEXTURED_ANIMATED, false, 10), 2); // border

	Vector3 trans(temp.pos.x, temp.pos.y, 0.133f);
	BoundingBox box(Vector3(-1, -1.3f, -0.5f), Vector3(1, 1.3f, 0.5f));

	//	Matrix mat;
	//	mat.translate(trans);
	//	box.transform(mat);
	flierModel->getMesh()->setBoundingBox(box);

	temp.node->setTranslation(trans);


	scene->addNode(temp.node);

	temp.pin = _scene->findNode("Tack")->clone();
	temp.pin->setTranslation(trans + Vector3(0, 1.2f, 0.2f));
	((Model*)temp.pin->getDrawable())->setMaterial(Entity::buildMaterial(_scene, NULL, Entity::COLORED, true, -1));
	_scene->addNode(temp.pin);

	temp.shadow = _scene->findNode("Shadow")->clone();
	temp.shadow->setTranslation(trans + Vector3(0, 0, -0.1f));
	((Model*)temp.shadow->getDrawable())->setMaterial(Entity::buildMaterial(_scene, NULL, Entity::COLORED, true, -1));
	_scene->addNode(temp.shadow);


	if (animate){
		Animation* anim = temp.node->getAnimation();
		anim->createClips("res/Paper.animation");
		temp.clip = anim->getClip("Fly_In");
		temp.clip->play();
		temp.nodeAlpha = temp.pinAlpha = temp.shadowAlpha = 0;

	}

	levelList.push_back(temp);


	if (animate)
		targetAnimate = &levelList[levelList.size() - 1];

	defaultFB->bind();

	// set target for camera
	MessageDispatcher::Instance()->dispatchMessage(0, NULL, camera, FSM::SET_TARGET, &trans);
}

bool OverMap::gestureTapEvent(int x, int y)
{
	if (_scene){
		Ray* pickRay = camera->getPickRay(x, y);
		for (unsigned int i = 0; i < levelList.size(); ++i){
			Model* model = (Model*)levelList[i].node->getDrawable();
			BoundingBox original = model->getMesh()->getBoundingBox();
			BoundingBox box(original); // copy constructor
			Matrix mat;
			mat.translate(levelList[i].node->getTranslation());
			box.transform(mat);
			//BoundingBox box = original; // copy constructor

			if (pickRay->intersects(box) != Ray::INTERSECTS_NONE){
				//((main*)game)->StartGame(levelList[i].levelFile);
				gameStatus = 2; // GAME_RUNNING
				int status = luaL_dofile(L, levelList[i].levelFile.c_str());
				return true;
				int banana = 7; // use this to look at the status above
			}

		}
	}
	return false;
}