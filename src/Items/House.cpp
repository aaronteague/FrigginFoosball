#include "House.h"
#include "../LuaLoader.h"
#include "../Room.h"
#include "../FSM.h"

#include "../States/HouseIdle.h"
//extern Main game;

House::House(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld)
	:Item(loader, luaFile, position, Item::SOUTH, status, containedWorld)
{
	entityType = "House";
	Item::luaFile = luaFile;
	houseRef = NULL;
	textureF = loader->getString("NODE_TEXTURE");
	nodeI = loader->getString("NODE_ID");
	nodeF = loader->getString("NODE_FILE");
	roomFile = loader->getString("ROOM_FILE");

	luabridge::LuaRef interactPointTable = loader->getTable("INTERACT_POINT");
	interactOffset.x = loader->getFloat(1, interactPointTable);
	interactOffset.y = loader->getFloat(2, interactPointTable);

	luabridge::LuaRef pointTowardsTable = loader->getTable("INTERACT_TOWARDS");
	pointTowardsOffset.x = loader->getFloat(1, pointTowardsTable);
	pointTowardsOffset.y = loader->getFloat(2, pointTowardsTable);

	texNormal = loader->getString("NODE_NORMAL");

	luabridge::LuaRef boundPointsTable = loader->getTable("BOUNDS");
	counter = 1;
	while (true){
		luabridge::LuaRef boundPoint = loader->getTable(counter, boundPointsTable);

		if (boundPoint.isNil())
			break;
		Vector2 point;
		point.x = loader->getFloat(1, boundPoint) + position.x;
		point.y = loader->getFloat(2, boundPoint) + position.y;
		barrierPoints.push_back(point);
		counter++;
	}

	interactRange = 0.1f;
	stateMachine->SetCurrentState(new HouseIdle());
	
	
}




void House::Update(const float& elapsedTime)
{
	stateMachine->Update((float&)elapsedTime, true);
}

void House::Render()
{
	// just render it
	node->getDrawable()->draw();
}

void House::prepSelf()
{
	counter = 0;
	if (status == ABSTRACT){
		// house
		sendToLists(textureF, nodeF, nodeI, this);
		// window
		sendToLists(textureF, "none", "none", this);
		// normal
		if (texNormal.compare("") != 0)
			sendToLists(texNormal, "none", "none", this);
		buildPhys(barrierPoints);
	}
}

void House::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	if (status == ABSTRACT){
		// house
		if (counter == 0){
			if (texNormal.compare("") == 0)
				Item::node = nodeToScene(node, tex, _scene, TEXTURED_ANIMATED, true, 2, 0);
			else
				Item::node = nodeToScene(node, tex, _scene, TEXTURED_ANIMATED_NORMAL, true, 2, 0);

			Animation* _animation = Item::node->getAnimation("animations");
			_animation->createClips("res/Items/Houses/house.animation");
			animations.push_back(_animation->getClip("Open"));
			currentAnimClip = NULL;
		//	currentAnimClip = animations[0];
		//	currentAnimClip->play();
			SAFE_RELEASE(_animation);
		}
		// window
		else if (counter == 1){
			Material* mat = buildMaterial(_scene, tex, TEXTURED, true, 2);
			dynamic_cast<Model*>(Item::node->getDrawable())->setMaterial(mat, 1);
		}
		// normal
		else if (counter == 2){
			applyNormalMap(dynamic_cast<Model*>(Item::node->getDrawable())->getMaterial(0), tex);
		}
	
		if (counter == 2 || (texNormal.compare("") == 0 && counter == 1)){
			Item::node->setTranslation(position.x + currentDimensions.x / 2, position.y + currentDimensions.y / 2, 0);

			pointTowards.set(Item::node->getTranslationX() + pointTowardsOffset.x, Item::node->getTranslationY() + pointTowardsOffset.y);
			interactPoint.set(Item::node->getTranslationX() + interactOffset.x, Item::node->getTranslationY() + interactOffset.y);
			status = SOLID;
		}
	}
	counter++;
}

luabridge::LuaRef House::save()
{
	luabridge::LuaRef itemTable = luabridge::newTable(L);
	itemTable["luaFile"] = luaFile.c_str();
	itemTable["rotation"] = (int)rotation;
	return itemTable;
}
void House::load(luabridge::LuaRef table)
{
	*houseRef = table;
}

void House::openRoom(){
	LuaLoader* loader = new LuaLoader(L);
	room = new Room(roomFile, loader);
}