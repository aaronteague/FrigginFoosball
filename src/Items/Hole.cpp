#include "Hole.h"


Hole::Hole(Vector2 position, b2World* containedWorld)
{
	Item::position = position;
	Item::physWorld = containedWorld;
	entityType = "Hole";
	name = "Hole";
	physBody = NULL;
	defaultDimensions.set(1, 1, 0.2f);
	currentDimensions = defaultDimensions;

	shape = CIRCLE;
	radius = 0.1f;
	status = ABSTRACT;
}

luabridge::LuaRef Hole::save()
{
	luabridge::LuaRef itemTable = luabridge::newTable(L);
	itemTable["luaFile"] = luaFile.c_str();
	itemTable["rotation"] = (int)rotation;
	return itemTable;
}


void Hole::load(luabridge::LuaRef table){}

void Hole::Update(const float& elapsedTime)
{
	// nothing
}

void Hole::Render()
{
	node->getDrawable()->draw();
}

void Hole::prepSelf()
{
	sendToLists("res/Items/hole.png", "res/Items/hole.gpb", "Hole", this);
	buildPhys(radius);
}

void Hole::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	
	Item::node = nodeToScene(node, tex, _scene);
	status = SOLID;
	SetPosition(position.x, position.y);
//	Item::node->setTranslation(position.x, position.y, 1);
}

