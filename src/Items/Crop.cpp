#include "Crop.h"

Crop::Crop(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld)
:Item(loader, luaFile, position, rotation, status, containedWorld)
{
	entityType = "Crop";

	name = loader->getString("NAME");
	defaultDimensions.x = loader->getInt("MODEL_LENGTH");
	defaultDimensions.y = loader->getInt("MODEL_WIDTH");
	defaultDimensions.z = loader->getInt("MODEL_HEIGHT");
	
	nodeF = loader->getString("NODE_FILE");
	nodeI = loader->getString("NODE_ID");
	textureF = loader->getString("NODE_TEXTURE");
	normalFile = loader->getString("NORMAL");

	luabridge::LuaRef profitTable = loader->getTable("PROFIT");
	profit.resize(NUM_OF_PROFITS);
	for (int i = 0; i < NUM_OF_PROFITS; ++i)
		profit[i] = loader->getInt(i + 1, profitTable);

	counter = 0;

	shape = CIRCLE;
	radius = 0.2f;
}

luabridge::LuaRef Crop::save()
{
	luabridge::LuaRef itemTable = luabridge::newTable(L);
	itemTable["luaFile"] = luaFile.c_str();
	itemTable["rotation"] = (int)rotation;
	return itemTable;
}


void Crop::load(luabridge::LuaRef table){}


void Crop::Update(const float& elapsedTime)
{
	
}

void Crop::Render()
{
	node->getDrawable()->draw();
}


void Crop::prepSelf()
{
	counter = 0;
	sendToLists(textureF, nodeF, nodeI, this);
	if (normalFile.compare("") != 0)
		sendToLists(normalFile, "none", "none", this);

	if (status == Status::ABSTRACT)
		buildPhys(radius);
}

void Crop::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	if (counter == 0){
		if (normalFile.compare("") == 0)
			Item::node = nodeToScene(node, tex, _scene);
		else
			Item::node = nodeToScene(node, tex, _scene, TEXTURED_NORMAL);

	}else{
		applyNormalMap(dynamic_cast<Model*>(Item::node->getDrawable())->getMaterial(), tex);

		if (status == ABSTRACT){
			SetPosition(position.x, position.y);
			SetRotation(rotation);
			status = SOLID; // finished
		}
	}
	counter++;
}


