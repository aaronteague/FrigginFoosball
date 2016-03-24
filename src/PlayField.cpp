#include "PlayField.h"
//#include "Player.h"
//#include "Dependencies\Box2D\Box2D.h"
#include "Box2D/Box2d.h"
#include "Menu.h"
#include "MyCamera.h"
#include "Item.h"
#include "Weather.h"
#include "MatSystem.h"
#include "Character.h"
#include "FSM.h"
#include "WallSystem.h"
#include "Items/Hole.h"

void PlayField::Update(float &elapsedTime)
{
//	Vector2 targetPos = targetCharacter->GetPosition();
//	viewableZone.set(targetPos.x - VIEW_RANGE, targetPos.y - VIEW_RANGE
//		, targetPos.x + VIEW_RANGE, targetPos.y + VIEW_RANGE);

	if (touchDownPos.x > 0)
		pointToFollow = screenToFieldCoordinates(touchDownPos);


	// step the time
	const float idealFrameTime = 1000.0f / 60;
	accumulator += elapsedTime;
	while (accumulator > idealFrameTime){
		physWorld->Step(idealFrameTime / 1000.0f, 6, 2);
		accumulator -= idealFrameTime;

		for (unsigned int i = 0; i < charactersInField.size(); ++i)
			charactersInField[i]->UpdatePhys();

	}
	
	float alpha = accumulator / idealFrameTime;

	for (unsigned int i = 0; i < charactersInField.size(); ++i)
		charactersInField[i]->Update(elapsedTime, alpha);

	int width = tileMap.size() - 1;
	int length = tileMap[0].size() - 1;
	for (int i = width; i >= 0; i--)
	for (int j = length; j >= 0; j--)
	if (tileMap[i][j]->childItem){
		Item* item = tileMap[i][j]->childItem;
		item->Update(elapsedTime);
		// make alpha if close
		float distance = item->GetPosition().y - _camera->GetNode()->getTranslationY() - 3;
	//	item->SetAlpha(1);
		if (item->getDimensions().z > 2 && distance > 0 && distance < 9){
			//		item->GetNode()->setTag("transparent", "true");
			item->SetAlpha(distance / 9);
			//item->SetAlpha(0.3f);
		}
		else
			item->SetAlpha(1);
			
		
	}

//	physWorld->Step(1.0 / 60, 6, 2);
//	thePlayer->Update(elapsedTime, alpha);

	
	matSystem->Update(_camera->GetTarget()->getTranslation());
	
//	wallSystem->Update(_camera->GetTarget()->getTranslation());

	for (unsigned int i = 0; i < menusInField.size(); ++i)
		menusInField[i]->Update(elapsedTime);
	_camera->Update(elapsedTime);


	if (touchEventList.size() > 2)
		MessageDispatcher::Instance()->dispatchMessage(0, NULL, targetCharacter, FSM::STOP_FOLLOWING_POINT, NULL);
	tapHoldCheck();
}
void PlayField::Render()
{


	// render blocks
	for (unsigned int i = 0; i < blockMap.size(); ++i)
		for (unsigned int j = 0; j < blockMap[i].size(); ++j)
		{
			blockMap[i][j]->Render();
		}
	matSystem->Render(_camera->GetNode()->getCamera()->getViewProjectionMatrix());
	// render tiles
	Vector2 tPos = targetCharacter->GetPosition();
	unsigned int minX = tPos.x - VIEW_RANGE;
	if (minX < 0) minX = 0;
	unsigned int maxX = tPos.x + VIEW_RANGE;
	if (maxX > tileMap.size()) maxX = tileMap.size();
	unsigned int minY = tPos.y - VIEW_RANGE;
	if (minY < 0) minY = 0;
	unsigned int maxY = tPos.y + VIEW_RANGE;
	if (maxY > tileMap.size()) maxY = tileMap.size();

	// render characters
	//	thePlayer->Render();
	for (unsigned int i = 0; i < charactersInField.size(); ++i)
		charactersInField[i]->Render();

	//for (unsigned int i = minX; i < maxX; ++i)
	//	for (unsigned int j = minY; j < maxY; ++j)
	//		tileMap[i][j]->Render();


		for (unsigned int i = maxX - 1; i > minX; --i)
		for (unsigned int j = maxY - 1; j > minY; --j)
			tileMap[i][j]->Render();
	
		if (weather != NULL)
			weather->RenderPrecipitation(_camera->GetNode()->getCamera()->getViewProjectionMatrix());

	// render the menus
	for (unsigned int i = 0; i < menusInField.size(); ++i)
		menusInField[i]->Render();
}

//Player* PlayField::getPlayer()
//{
//	return thePlayer;
//}

void PlayField::buildBarrier(Vector4 &barrier)
{
	b2Vec2 v1(barrier.x, barrier.y);
	b2Vec2 v2(barrier.z, barrier.w);
	b2FixtureDef lineFixDef;
	b2EdgeShape edge;
	edge.Set(v1, v2);
	lineFixDef.shape = &edge;
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(0, 0);
	b2Body *body = physWorld->CreateBody(&myBodyDef);
	body->CreateFixture(&lineFixDef);
}

void PlayField::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	if (evt == Touch::TouchEvent::TOUCH_PRESS){
		for (int i = menusInField.size() - 1; i >= 0; --i)
		{
			if (menusInField[i]->touchPress(Vector2(x, y))){
				focusedMenu = menusInField[i];
				break;
			}
		}

		// add touchEvent things here
		if (focusedMenu == NULL){
			touchDownPos.set(x, y);
			pointToFollow = screenToFieldCoordinates(touchDownPos);
			MessageDispatcher::Instance()->dispatchMessage(0, NULL, targetCharacter, FSM::FOLLOW_POINT, &pointToFollow);
			tapEventCheck(evt, x, y);
		}
		//Item* item = getTouchedItem(x, y);
	}
	else if (evt == Touch::TouchEvent::TOUCH_MOVE){
		if (focusedMenu != NULL)
			focusedMenu->touchMove(Vector2(x, y));
		else
			touchDownPos.set(x, y);
	}
	else if (evt == Touch::TouchEvent::TOUCH_RELEASE){
		if (focusedMenu != NULL){
			focusedMenu->touchRelease(Vector2(x, y));
			focusedMenu = NULL;
		}
		else{
			touchDownPos.set(-1, -1);
			MessageDispatcher::Instance()->dispatchMessage(0, NULL, targetCharacter, FSM::STOP_FOLLOWING_POINT, &pointToFollow);
			tapEventCheck(evt, x, y);
		}
	}
}

const Vector2& PlayField::screenToFieldCoordinates(Vector2 touchPos)
{
	return _camera->screenToFieldCoordinates(touchPos);
}

void PlayField::putItemInTile(Item* item, int x, int y)
{
	
	
	Vector3 dim = item->getDimensions();
	for (int i = x; i < x + (int)dim.x; ++i)
		for (int j = y; j < y + (int)dim.y; ++j){
			if (tileMap[i][j] != NULL && tileMap[i][j]->type == Tile::ITEM){
				Item* itemToRemove = removeItemFromTile(i, j);
				delete(itemToRemove);
			//	tileMap[i][j]->type = Tile::OCCUPIED;
			//	tileMap[i][j]->ownedBy.set(x, y);
			}
			if (tileMap[i][j] != NULL){
				tileMap[i][j]->type = Tile::OCCUPIED;
				tileMap[i][j]->ownedBy.set(x, y);
			}
		}

		if (item->GetStatus() == Item::PREVIEW){ // create item if scene is already made
		//	item->addSolidToPlayField(physWorld, _scene, Vector2(x, y));
			item->SetStatus(Item::SOLID, physWorld);

		}
	tileMap[x][y]->type = Tile::TileTypeList::ITEM;
	tileMap[x][y]->childItem = item;
	
}

void PlayField::Prepare()
{
//	thePlayer->AddToEntityList();
//	_camera->AddToEntityList();
	
	// prepare menus
	for (unsigned int i = 0; i < menusInField.size(); ++i)
		menusInField[i]->AddToEntityList();

	// prepare characters
	for (unsigned int i = 0; i < charactersInField.size(); ++i)
		charactersInField[i]->AddToEntityList();



	// add the blocks
	for (unsigned int i = 0; i < blockMap.size(); ++i)
		for (unsigned int j = 0; j < blockMap[i].size(); ++j)
			blockMap[i][j]->AddToEntityList();

	for (unsigned int i = 0; i < tileMap.size(); ++i)
		for (unsigned int j = 0; j < tileMap[i].size(); ++j){
			if (tileMap[i][j]->type == Tile::ITEM){
				if (tileMap[i][j]->childItem == NULL)
					GP_ERROR("marked as item tile but has no child item!");
				else
					tileMap[i][j]->childItem->AddToEntityList();
			}
			if (tileMap[i][j]->mat.compare("none") != 0)
				// the rectangle part of this needs some doctoring
				matSystem->addMat(tileMap[i][j]->mat, Vector2(i, j));
		}
	
	// add weather and matsystem
	if (weather != NULL)
		weather->AddToEntityList();
	matSystem->AddToEntityList();
	if (wallSystem != NULL)
	wallSystem->AddToEntityList();



	Entity::LoadAssets();
}

void PlayField::Finalize(float screenWidth, float screenHeight, TimeAndDate *time)
{
	_scene = Scene::load("res/landscape.gpb");
	// add directional light
	Node* lightNode = Node::create("light");
	Light* light = Light::createDirectional(Vector3::one());
	
	lightNode->setLight(light);
	lightNode->setRotation(Matrix::identity());
	_scene->addNode(lightNode);
//	weather->updateLight(_scene, time);



	screenSize.set(screenWidth, screenHeight);
	_camera = new MyCamera(_scene, screenWidth, screenHeight);

	Entity::BuildAndFinish(_scene);
//	if (weather != NULL)
//		weather->updateWetAreas(tileMap);

}

void PlayField::addMenu(Menu* menu)
{
	// first see if it already exists in the menu list
	bool inMenu = false;
	for (unsigned int i = 0; i < menusInField.size(); ++i)
		if (menusInField[i] == menu){
			inMenu = true;
			break;
		}

	// if it does not exist, add it to list
	if (!inMenu){
		menu->SetPlayField(this);
		menusInField.push_back(menu);
		// see if it needs to be made
		if (_scene != NULL && menu->NeedsInitialized()){
			Entity::ClearLists();
			menu->AddToEntityList();
			//menu->prepSelf();
			Entity::LoadAssets();
			Entity::BuildAndFinish(_scene);
		}
			
	}
}
void PlayField::removeMenu(Menu* menu)
{
	// check for the menu in list
	int index = -1;
	for (unsigned int i = 0; i < menusInField.size(); ++i)
		if (menusInField[i] == menu){
			index = i;
			break;
		}

	// if found, remove it
	if (index != -1)
		menusInField.erase(menusInField.begin() + index);
}

void PlayField::addCharacter(Character* character)
{
	// check for character in list
	bool inList = false;
	for (unsigned int i = 0; i < charactersInField.size(); ++i)
		if (charactersInField[i] == character){
			inList = true;
			break;
		}

	// if it isn't there already, add it
	if (!inList){
		character->AddToPlayField(this, _scene, physWorld);
		charactersInField.push_back(character);
	}
}
void PlayField::removeCharacter(Character* character)
{
	int index = -1;
	for (unsigned int i = 0; i < charactersInField.size(); ++i)
		if (charactersInField[i] == character){
			index = i;
			break;
		}

	if (index != -1){
		character->RemovePlayField();
		charactersInField.erase(charactersInField.begin() + index);
		if (_scene != NULL && character->GetNode() != NULL)
			_scene->removeNode(character->GetNode());
		if (physWorld != NULL)
			character->removePhys();
	}
}

luabridge::LuaRef PlayField::save(){
	luabridge::LuaRef landTable = luabridge::newTable(L);

//	landTable["weather"] = weather->save();

	// save tiles
	luabridge::LuaRef posX = luabridge::newTable(L);
	for (int x = 0; x < worldBlockLength * blockTileLength; ++x){
		luabridge::LuaRef posY = luabridge::newTable(L);
		for (int y = 0; y < worldBlockLength * blockTileLength; ++y){
			if (tileMap[x][y]->type != Tile::UNAVAILABLE){
				luabridge::LuaRef tile = luabridge::newTable(L);
				tile["type"] = tileMap[x][y]->type;
				tile["fert"] = tileMap[x][y]->fertility;
				tile["mat"] = tileMap[x][y]->mat;

				luabridge::LuaRef ownedByRef = luabridge::newTable(L);
				int obX = tileMap[x][y]->ownedBy.x;
				int obY = tileMap[x][y]->ownedBy.y;
				ownedByRef[1] = obX;
				ownedByRef[2] = obY;
				tile["ownBy"] = ownedByRef;
				if (tileMap[x][y]->type == Tile::ITEM)
					tile["childItem"] = tileMap[x][y]->childItem->save();
				posY[y + 1] = tile;
			}
			else
				int i = 7;
		}
		posX[x + 1] = posY;
	}
	landTable["tiles"] = posX;

	// save them block files
	luabridge::LuaRef pX = luabridge::newTable(L);
	for (int x = 0; x < worldBlockLength; ++x){
		luabridge::LuaRef pY = luabridge::newTable(L);
		for (int y = 0; y < worldBlockLength; ++y)
			pY[y + 1] = blockFileMap[x][y];
		pX[x + 1] = pY;
	}
	landTable["blockFiles"] = pX;

	// save barriers
	luabridge::LuaRef linesRef = luabridge::newTable(L);
	linesRef["size"] = barriers.size();
	for (unsigned int i = 0; i < barriers.size(); ++i){
		luabridge::LuaRef indivLine = luabridge::newTable(L);
		indivLine[1] = barriers[i].x;
		indivLine[2] = barriers[i].y;
		indivLine[3] = barriers[i].z;
		indivLine[4] = barriers[i].w;
		linesRef[i + 1] = indivLine;
	}
	landTable["lines"] = linesRef;
	

	// save zones here later
	//	landTable["thePlayer"] = thePlayer->save();

	return landTable;
}
void PlayField::load(luabridge::LuaRef &landscapeRef, LuaLoader* loader)
{
	b2Vec2 gravity(0.0f, 0.0f); // nope on gravity
	physWorld = new b2World(gravity);


//	LuaLoader* loader = new LuaLoader(L);

	// get the player
//	luabridge::LuaRef playerRef = landscapeRef["thePlayer"];
//	luabridge::LuaRef playerLuaFileRef = playerRef["luaFile"];
//	std::string playerLuaFile = playerLuaFileRef;
	//	thePlayer = new Player(loader, playerLuaFile, 55, 55, physWorld, this, screenSize);
	//	thePlayer->load(playerRef);

	loader->setFile("res/luaFiles/landscapeIndex.lua");


	//	menusInField.push_back(timeAndDate);


	worldBlockLength = loader->getInt("WORLD_BLOCK_LENGTH");
	blockTileLength = loader->getInt("BLOCK_TILE_LENGTH");

	matSystem = new MatSystem(worldBlockLength * blockTileLength, worldBlockLength * blockTileLength);
//	matSystem->addMat("res/Mats/house.png", Vector2(55, 55));
//	matSystem->addMat("res/Mats/house.png", Vector2(55, 56));
////	matSystem->addMat("res/Mats/tree.png", Vector2(53, 55));
//	matSystem->addMat("res/Mats/plant.png", Vector2(50, 50));
//	matSystem->addMat("res/Mats/house.png", Vector2(51, 51));

	// load up the tiles now
	luabridge::LuaRef tiles = landscapeRef["tiles"];
	tileMap.resize(worldBlockLength * blockTileLength);
	for (int i = 0; i < worldBlockLength * blockTileLength; ++i){
		tileMap[i].resize(worldBlockLength * blockTileLength);
		luabridge::LuaRef posX = tiles[i + 1];
		for (int j = 0; j < worldBlockLength * blockTileLength; ++j){
			luabridge::LuaRef posY = posX[j + 1];
			tileMap[i][j] = new Tile();
			tileMap[i][j]->type = Tile::UNAVAILABLE; // the default
			if (!posY.isNil()){
				tileMap[i][j]->type = posY["type"];
				tileMap[i][j]->fertility = posY["fert"];
				luabridge::LuaRef matRef = posY["mat"];
				std::string mat = matRef;
				tileMap[i][j]->mat = mat;
				luabridge::LuaRef ownedByRef = posY["ownBy"];
				int ownedByX = ownedByRef[1];
				int ownedByY = ownedByRef[2];
				tileMap[i][j]->ownedBy.set(ownedByX, ownedByY);
				tileMap[i][j]->position.set(i, j);
				if (tileMap[i][j]->type == Tile::ITEM){
					luabridge::LuaRef childItem = posY["childItem"];
					luabridge::LuaRef luaFileRef = childItem["luaFile"];
					luabridge::LuaRef rotationRef = childItem["rotation"];
					int rotation = rotationRef;
					std::string luaFile = luaFileRef;
					tileMap[i][j]->luaFile = luaFile;
					tileMap[i][j]->childItem = Item::Create(loader, luaFile, Vector2(i, j), (Item::Rotation)rotation, Item::ABSTRACT, physWorld);
				//	tileMap[i][j]->childItem->load(childItem);  // i don't think we have this set up yet
				}
			}
		}
	}

	// alright, lets load up the blocks from here
	luabridge::LuaRef blockFiles = landscapeRef["blockFiles"];
	blockFileMap.resize(worldBlockLength);
	blockMap.resize(worldBlockLength);
	for (int i = 0; i < worldBlockLength; ++i){
		luabridge::LuaRef blockX = blockFiles[i + 1];
		blockFileMap[i].resize(worldBlockLength);
		blockMap[i].resize(worldBlockLength);
		for (int j = 0; j < worldBlockLength; ++j){
			luabridge::LuaRef blockY = blockX[j + 1];
			std::string luaFile = blockY;
			blockFileMap[i][j] = luaFile;
			blockMap[i][j] = new Block(loader, luaFile, Vector2(i, j), false);
		}
	}

	// load barriers
	luabridge::LuaRef linesRef = landscapeRef["lines"];
	int numOfBarriers = linesRef["size"];
	for (int i = 0; i < numOfBarriers; ++i){
		luabridge::LuaRef indivLine = linesRef[i + 1];
		float pX = indivLine[1];
		float pY = indivLine[2];
		float pZ = indivLine[3];
		float pW = indivLine[4];
		Vector4 barrier(pX, pY, pZ, pW);
		barriers.push_back(barrier);
	}

	for (unsigned int i = 0; i < barriers.size(); ++i){
		buildBarrier(barriers[i]);
	}

	// load zones later




	//delete(loader);


}

Item* PlayField::removeItemFromTile(int x, int y)
{
	Item* item = tileMap[x][y]->childItem;
	tileMap[x][y]->childItem = NULL;
	Vector3 dim = item->getDimensions();
	for (int i = x; i < x + dim.x; ++i)
		for (int j = y; j < y + dim.y; ++j)
			tileMap[x][y]->type = Tile::EMPTY;
	return item;
}

Item* PlayField::GetItemFromPoint(unsigned int x, unsigned int y)
{
	// make sure that it is in range

	if (x >= 0 && x < tileMap.size()
		&& y >= 0 && y < tileMap.size()){
		if (tileMap[x][y]->type == Tile::ITEM)
			return tileMap[x][y]->childItem;
		if (tileMap[x][y]->type == Tile::OCCUPIED){
			Vector2 pos = tileMap[x][y]->ownedBy;
			return tileMap[pos.x][pos.y]->childItem;
		}
	}
	return NULL;
}

bool PlayField::digTile(Tile* tile)
{
	if (tile->childItem == NULL){
		Entity::ClearLists();
		tile->type = Tile::ITEM;
		tile->childItem = new Hole(tile->position, physWorld);
		tile->childItem->AddToEntityList();
		Entity::LoadAssets();
		Entity::BuildAndFinish(_scene);
		return true;
	}else
		return false;
}

bool PlayField::tillTile(Tile* tile)
{
	if (tile->childItem == NULL){
		Entity::ClearLists();
		matSystem->addMat("res/Mats/Dirt.png", tile->position);
		matSystem->AddToEntityList();
		Entity::LoadAssets();
		Entity::BuildAndFinish(_scene);
		Entity::ClearLists();
		return true;
	}
else
	return false;

}

bool PlayField::waterTile(Tile* tile)
{
	tile->water++;
	if (tile->water > 5)
		tile->water = 5;
	return true;
}

void PlayField::SetTargetCharacter(Character* character){
	targetCharacter = character;
	_camera->SetTarget(targetCharacter->GetNode());
	
}

Item* PlayField::getTouchedItem(int x, int y)
{
	Vector2 tPos = targetCharacter->GetPosition();
	unsigned int minX = tPos.x - VIEW_RANGE;
	if (minX < 0) minX = 0;
	unsigned int maxX = tPos.x + VIEW_RANGE;
	if (maxX > tileMap.size()) maxX = tileMap.size();
	unsigned int minY = tPos.y - VIEW_RANGE;
	if (minY < 0) minY = 0;
	unsigned int maxY = tPos.y + VIEW_RANGE;
	if (maxY > tileMap.size()) maxY = tileMap.size();

	// do reverse, y than x so that it checks back to front
	for (unsigned int py = minY; py <maxY; ++py)
		for (unsigned int px = minX; px < maxX; ++px)
//	for (int i = 0; i < tileMap.size(); ++i)
//		for (int j = 0; j < tileMap.size(); ++j)
			if (tileMap[px][py]->type == Tile::ITEM)
				if (_camera->isTouching(tileMap[px][py]->childItem->GetNode(), x, y, tileMap[px][py]->childItem->getDimensions()))
					return tileMap[px][py]->childItem;
	
	return NULL;
		
}

Character* PlayField::getTouchedCharacter(int x, int y)
{
	for (unsigned int i = 0; i < charactersInField.size(); ++i){
		if (_camera->isTouching(charactersInField[i]->GetNode(), x, y, charactersInField[i]->GetHeight() / 2))
			return charactersInField[i];
	}

	return NULL;
}

Tile* PlayField::getTouchedTile(int x, int y)
{
	Vector2 pos = screenToFieldCoordinates(Vector2(x, y));
	if (isTile(pos.x, pos.y))
		return tileMap[pos.x][pos.y];
	return NULL;
}

void PlayField::tapEventCheck(Touch::TouchEvent evt, int x, int y)
{
	
	TouchStruct touch(Game::getAbsoluteTime(), evt, Vector2(x,y));


	int numOfEvents = touchEventList.size();
	switch (numOfEvents){
	case FIRST_DOWN:
		if (evt != Touch::TOUCH_PRESS){
			touchEventList.clear();
			break;  // if this isn't a touch-press, GTFO
		}

		touchEventList.push_back(touch);
		
		break;
	case FIRST_UP:
		if (evt != Touch::TOUCH_RELEASE){
			touchEventList.clear();
			break;
		}

		if (tapIsAcceptable(touch))
			touchEventList.push_back(touch);
		else
			touchEventList.clear();
		break;
	case SECOND_DOWN:
		if (evt != Touch::TOUCH_PRESS){
			touchEventList.clear();
			break;
		}

		if (tapIsAcceptable(touch)){
			touchEventList.push_back(touch);
		}
		else{
			touchEventList.clear();
			touchEventList.push_back(touch);
		}
		break;
	case SECOND_UP:
		if (evt != Touch::TOUCH_RELEASE){
			touchEventList.clear();
			break;
		}

		if (tapIsAcceptable(touch))
			if (touchEventList[0].touchedCharacter != NULL)
				MessageDispatcher::Instance()->dispatchMessage
				(0, NULL, targetCharacter, FSM::DOUBLE_TAPPED_CHARACTER, touchEventList[0].touchedCharacter);
			else if (touchEventList[0].touchedItem != NULL){
				MessageDispatcher::Instance()->dispatchMessage(0, NULL, targetCharacter, FSM::DOUBLE_TAPPED_ITEM, touchEventList[0].touchedItem);
		}	else if (touchEventList[0].touchedTile != NULL)
				MessageDispatcher::Instance()->dispatchMessage
				(0, NULL, targetCharacter, FSM::DOUBLE_TAPPED_TILE, touchEventList[0].touchedTile);
			
			// send off a message of double tap
				
		touchEventList.clear();	
		break;
	case INVALID:
		// yeah, shouldn't have made it this far
		// , figure out where it went wrong
		touchEventList.clear();		
		 break;
	}


	// if there is just a single touch down, find the thing touched, if any
	if (touchEventList.size() == 1){

		Item* item = getTouchedItem(x, y);
		Character* character = getTouchedCharacter(x, y);
		Tile* tile = getTouchedTile(x, y);

		if (character != NULL)
			touchEventList[FIRST_DOWN].touchedCharacter = character;		
		else if (item != NULL)
			touchEventList[FIRST_DOWN].touchedItem = item;
		else if (tile != NULL)
			touchEventList[FIRST_DOWN].touchedTile = tile;
		else // if all are null
			// get out and do nothing
			touchEventList.clear();
	}
}

bool PlayField::lengthAcceptableForTap(Vector2& start, Vector2& end)
{

	const float MAX_MOVE_LENGTH = 150;
	const float SCALE_FACTOR = 1000;

	// get move length
	float scale = SCALE_FACTOR / screenSize.x; // get scale
	float maxMoveLength = MAX_MOVE_LENGTH * scale; // the most the finger could move until it isn't a tap
	Vector2 distanceVec = start;
	distanceVec.subtract(end);

	float actualDistance = distanceVec.length();

	return actualDistance < maxMoveLength;
}

void PlayField::tapHoldCheck()
{
	if (touchEventList.size() == 3 &&
		Game::getAbsoluteTime() - touchEventList[2].timeStamp
	> MAX_TOUCH_TIME){
		// send message oh hold
		if (touchEventList[0].touchedItem != NULL){
		//	focusedMenu = (Menu*)targetCharacter->GetInventory();
			// remove the item from tilemap?
			Vector2 pos = touchEventList[0].touchedItem->GetPosition();
			tileMap[pos.x][pos.y]->childItem = NULL;
			tileMap[pos.x][pos.y]->type = Tile::EMPTY;
			MessageDispatcher::Instance()->dispatchMessage(0, NULL, targetCharacter, FSM::TAP_HOLD_ITEM, touchEventList[0].touchedItem);
		}
		touchEventList.clear();
	}
}

bool PlayField::isTile(int x, int y)
{
	if (x >= 0 && x < worldBlockLength * blockTileLength
		&& y >= 0 && y < worldBlockLength * blockTileLength)
		if (tileMap[x][y]->type != Tile::UNAVAILABLE)
			return true;
	return false;
}

bool PlayField::isFreeTile(int x, int y){
	if (x >= 0 && x < worldBlockLength * blockTileLength
		&& y >= 0 && y < worldBlockLength * blockTileLength){
		int type = tileMap[x][y]->type;
		if (type == Tile::EMPTY || type == Tile::RESERVED)
			return true;
	}
	return false;
}

bool PlayField::tapIsAcceptable(TouchStruct& newTouch)
{
	bool distanceAcceptable
		= lengthAcceptableForTap
		(touchEventList[touchEventList.size() - 1].touchedScreenCoords
		, newTouch.touchedScreenCoords);

	// get time of touchDown
	float timeDown
		= newTouch.timeStamp - touchEventList[touchEventList.size() - 1].timeStamp;

	if (timeDown < MAX_TOUCH_TIME
		&& distanceAcceptable){
		return true;
	}
	else{
		return false;
	}
}

void PlayField::setFocusedMenu(Menu* menu)
{
	// check first to see if in menu list
	for (unsigned int i = 0; i < menusInField.size(); ++i)
		if (menu == menusInField[i]){
			focusedMenu = menu;
			break;
		}
}

std::vector<Vector2> PlayField::getPath(int startX, int startY, int endX, int endY)
{
	// check distance.  if under 5, go straight there
	Vector2 sVec(startX, startY);
	Vector2 eVec(endX, endY);
	sVec.subtract(eVec);
	if (sVec.length() < 5){
		std::vector<Vector2> vecPath;
		
		vecPath.push_back(eVec);
		return vecPath;
	}

	MicroPather pather(this);  // create the pather
	std::vector<void*> path; // where the path will be stored
	float totalCost;

	Tile* start = tileMap[startX][startY];  // where to start
	Tile* end = tileMap[endX][endY]; // where to end

	std::vector<Vector2> vecPath;  // store path in vector2 format
	int result = pather.Solve((void*)start, (void*)end, &path, &totalCost);

	for (unsigned int i = path.size(); i > 1; i--){ // skip first spot
		Tile* tile = (Tile*)path[i - 1];
		
		vecPath.push_back(Vector2(tile->position.x + 0.5, tile->position.y + 0.5));
	}

	// let's shorten the path to create less turns
	
	unsigned int indexA = 0;
	while (indexA < vecPath.size()){
		unsigned int indexB = indexA + 2;
		while (indexB < vecPath.size()){
			Vector2 direction = (vecPath[indexA] - vecPath[indexB]);
			float distance = direction.length();
			direction.normalize();
			Vector2 perpDir(direction.y * -1, direction.x);
			const float radius = 0.45f;
			int numOfPoints = distance / 0.02;
			bool collided = false;
			for (int i = 1; i <= numOfPoints; ++i){
				// first check one side
				Vector2 point1 = vecPath[indexA] + perpDir * radius + (direction * (i * 0.1));
				int tileType = tileMap[point1.x][point1.y]->type;
				if (tileType == Tile::OCCUPIED 
					|| tileType == Tile::ITEM
					|| tileType == Tile::UNAVAILABLE)
				{
 					collided = true;
					break;
				}
				// then the other
				Vector2 point2 = vecPath[indexA] + perpDir * radius * -1 + (direction * (i * 0.1));
				tileType = tileMap[point2.x][point2.y]->type;
				if (tileType == Tile::OCCUPIED
					|| tileType == Tile::ITEM
					|| tileType == Tile::UNAVAILABLE)
				{
					collided = true;
					break;
				}
			}
			
			if (!collided)
				vecPath.erase(vecPath.begin() + indexB - 1);
				
			indexB++;
		}
		indexA++;
	}
	
	return vecPath;
}

float PlayField::LeastCostEstimate(void* nodeStart, void* nodeEnd)
{
	Tile* start = (Tile*)nodeStart;
	Tile* end = (Tile*)nodeEnd;
	int dx = end->position.x - start->position.x;
	int dy = end->position.y - start->position.y;

	return abs(b2Vec2(dx, dy).Length()) * 10;  // return the cost from current tile to last
}
void PlayField::AdjacentCost(void* node, std::vector< StateCost > *neighbors)
{
	Tile* currentTile = (Tile*)node;
	int x = currentTile->position.x;
	int y = currentTile->position.y;

	//std::vector<int> xOff = { -1, 0, 1, 1, 1, 0, -1, -1 };  // x
	//std::vector<int> yOff = { 1, 1, 1, 0, -1, -1, -1, 0 };  // y
	//std::vector<int> cost = { 14, 10, 14, 10, 14, 10, 14, 10 };  // cost

	std::vector<int> xOff = {  0,  1, 0,  -1 };  // x
	std::vector<int> yOff = {  1,  0,  -1,  0 };  // y
	std::vector<int> cost = {  10,  10,  10, 10 };  // cost

	int tileMapSize = tileMap.size();
	for (int i = 0; i < 4; i++){
		if (x + xOff[i] >= 0 && x + xOff[i] < tileMapSize
			&& y + yOff[i] >= 0 && y + yOff[i] < tileMapSize){ // check to make sure it is in map
			int type = tileMap[x + xOff[i]][y + yOff[i]]->type;
			if (type == Tile::EMPTY || type == Tile::RESERVED)

			{
				int testX = x + xOff[i];
				int testY = y + yOff[i];

				StateCost stateCost = { tileMap[x + xOff[i]][y + yOff[i]], (float)cost[i] };
				neighbors->push_back(stateCost);  // if it is, put it in the vector
			}
		}
	}
}

void PlayField::ageUp(){
	Entity::ClearLists();

	for (unsigned int i = 0; i < tileMap.size(); ++i)
	for (unsigned int j = 0; j < tileMap[i].size(); ++j)
	if (tileMap[i][j]->childItem != NULL){
		Tile* tile = tileMap[i][j];
		tileMap[i][j]->childItem->ageUp(tile->fertility, tile->water, _scene);
		tileMap[i][j]->water--; // dry it up each day
	}

	
	// if models get changed from ageUp function

	Entity::LoadAssets();
	Entity::BuildAndFinish(_scene);
	Entity::ClearLists();
}

void PlayField::updateLight(TimeAndDate *time)
{
	if (weather != NULL)
		weather->updateLight(_scene, time);
}