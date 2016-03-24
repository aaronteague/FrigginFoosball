#include "Tool.h"

Tool::Tool(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld)
:Item(loader, luaFile, position, rotation, status, containedWorld)
{
	entityType = "Tool";

	name = loader->getString("NAME");
	defaultDimensions.x = loader->getInt("MODEL_LENGTH");
	defaultDimensions.y = loader->getInt("MODEL_WIDTH");
	defaultDimensions.z = loader->getInt("MODEL_HEIGHT");
	radius = loader->getFloat("RADIUS");

	nodeF = loader->getString("NODE_FILE");
	nodeI = loader->getString("NODE_ID");
	textureF = loader->getString("NODE_TEXTURE");
	normalFile = loader->getString("NORMAL");

	cost = loader->getInt("COST");
	profit = loader->getInt("PROFIT");
	
	counter = 0;

	shape = CIRCLE;
}

luabridge::LuaRef Tool::save()
{
	luabridge::LuaRef itemTable = luabridge::newTable(L);
	itemTable["luaFile"] = luaFile.c_str();
	itemTable["rotation"] = (int)rotation;
	return itemTable;
}


void Tool::load(luabridge::LuaRef table){}


void Tool::Update(const float& elapsedTime)
{

}

void Tool::Render()
{
	node->getDrawable()->draw();
}


void Tool::prepSelf()
{
	counter = 0;
	sendToLists(textureF, nodeF, nodeI, this);
	if (normalFile.compare("") != 0)
		sendToLists(normalFile, "none", "none", this);

	if (status == Status::ABSTRACT)
		buildPhys(radius);
}

void Tool::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	if (counter == 0){
		if (normalFile.compare("") == 0)
			Item::node = nodeToScene(node, tex, _scene);
		else
			Item::node = nodeToScene(node, tex, _scene, TEXTURED_NORMAL);

	}
	else{
		applyNormalMap(dynamic_cast<Model*>(Item::node->getDrawable())->getMaterial(), tex);

		if (status == ABSTRACT){
			SetPosition(position.x, position.y);
			SetRotation(rotation);
			status = SOLID; // finished
		}
	}
	counter++;
}


