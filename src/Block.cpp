#include "Block.h"

#include "Tile.h"

#include "Menus\TimeAndDate.h"

Block::Block(LuaLoader *loader, std::string file, Vector2 position, bool firstTime){
	Block::position.set(position);
	luaFile = file;

	loader->setFile(file);

	node = NULL;
	nodeF = loader->getString("NODE_FILE");
	textureFiles.resize(4);
	textureFiles[TimeAndDate::SPRING] = loader->getString("SPRING_TEXTURE");
	textureFiles[TimeAndDate::SUMMER] = loader->getString("SUMMER_TEXTURE");
	textureFiles[TimeAndDate::AUTUMN] = loader->getString("AUTUMN_TEXTURE");
	textureFiles[TimeAndDate::WINTER] = loader->getString("WINTER_TEXTURE");
	nodeI = loader->getString("NODE_ID");
	entityType = loader->getString("TYPE");
	blockTileLength = loader->getInt("BLOCK_TILE_LENGTH");
	name = loader->getString("NAME");
	std::string initialLayout = loader->getString("BLOCK_LAYOUT");

	if (firstTime){
		loader->setFile(initialLayout);
		int numOfLines = loader->getInt("NUM_OF_LINES");
		int numOfHousePoints = loader->getInt("NUM_OF_HOUSE_POINTS");
		int numOfZones = loader->getInt("NUM_OF_ZONES");

		luabridge::LuaRef blockRef = loader->getTable("block");
		luabridge::LuaRef tilesRef = loader->getTable("Tiles", blockRef);

		tileMap.resize(blockTileLength);
		for (int i = 0; i < blockTileLength; ++i){
			tileMap[i].resize(blockTileLength);
			luabridge::LuaRef pX = loader->getTable(i + 1, tilesRef);
			for (int j = 0; j < blockTileLength; ++j){
				tileMap[i][j] = new Tile();
				tileMap[i][j]->position.set(i, j);
				luabridge::LuaRef pY = loader->getTable(j + 1, pX);
				tileMap[i][j]->type = loader->getInt("Type", pY);
				tileMap[i][j]->mat = loader->getString("Mat", pY);
				tileMap[i][j]->fertility = loader->getInt("Fertility", pY);
				luabridge::LuaRef ownByRef = loader->getTable("ownedBy", pY);
				int numX = loader->getInt(1, ownByRef);
				int numY = loader->getInt(2, ownByRef);

				if (tileMap[i][j]->type == Tile::ITEM)
					tileMap[i][j]->luaFile = loader->getString("ItemLuaFile", pY);
				else
					tileMap[i][j]->luaFile = "none";
			}
		}

		// make the barriers
		luabridge::LuaRef linesRef = loader->getTable("Lines", blockRef);
		for (int i = 0; i < numOfLines; ++i){
			luabridge::LuaRef indivLine = loader->getTable(i + 1, linesRef);
			float pX = loader->getFloat(1, indivLine);
			float pY = loader->getFloat(2, indivLine);
			float pZ = loader->getFloat(3, indivLine);
			float pW = loader->getFloat(4, indivLine);
			barriers.push_back(Vector4(pX, pY, pZ, pW));
		}

		// make the house points
		luabridge::LuaRef housePointsRef(L);
		if (numOfHousePoints > 0)
			housePointsRef = loader->getTable("HousePoints", blockRef);
		for (int i = 0; i < numOfHousePoints; ++i){
			luabridge::LuaRef point = loader->getTable(i + 1, housePointsRef);
			int x = loader->getInt(1, point);
			int y = loader->getInt(2, point);
			housePoints.push_back(Vector2(x, y));
		}
		
		// check for start points
		int numOfStartPoints = loader->getInt("NUM_OF_START_POINTS");
		if (numOfStartPoints > 0){
			luabridge::LuaRef startPointsRef = loader->getTable("StartPointFrom", blockRef);
			for (int i = 0; i < numOfStartPoints; ++i){
				luabridge::LuaRef pointRef = loader->getTable(i + 1, startPointsRef);
				float posX = loader->getFloat(1, pointRef);
				float posY = loader->getFloat(2, pointRef);
				float rotation = loader->getFloat(3, pointRef);
				startPoints.push_back(Vector3(posX, posY, rotation));
			}
		}
	}

	currentTextureFile = textureFiles[TimeAndDate::AUTUMN];
}

void Block::SetPosition(float x, float y)
{
	// if node is made, set the node translation
	if (node != NULL){
		node->setTranslationX(x * blockTileLength + blockTileLength / 2);
		node->setTranslationY(y * blockTileLength + blockTileLength / 2);		
	}
	position.set(x, y);
}

void Block::prepSelf()
{
	sendToLists(currentTextureFile, nodeF, nodeI, this);

}

void Block::addToGame(Node* node, Texture* tex, Scene* _scene)
{
//	if (nodeI.compare("normal") == 0){
//		if (Block::node && Block::node->getModel()->getMaterial())
//			applyNormalMap(Block::node->getModel()->getMaterial(), tex);
//		else
//			Block::node = nodeToScene(node, tex, _scene, TEXTURED_NORMAL);
//	}
//	else
		Block::node = nodeToScene(node, tex, _scene, TEXTURED, true);
		dynamic_cast<Model*>(Block::node->getDrawable())->getMaterial()->getStateBlock()->setDepthTest(true);
	Block::node->rotateZ(MATH_DEG_TO_RAD(-90));
	SetPosition(position.x, position.y);
}

std::vector<std::vector<Tile*>> Block::getInitialTileMap()
{
	return tileMap;
}

void Block::clearInitialExcess()
{
		barriers.clear();
		housePoints.clear();

		for (unsigned int i = 0; i < tileMap.size(); i++){
			for (unsigned int j = 0; i < tileMap[i].size(); j++){
				delete(tileMap[i][j]);
			}
			tileMap[i].clear();
		}
		tileMap.clear();
}

void Block::Update(const float& elapsedTime)
{

}
void Block::Render()
{
	node->getDrawable()->draw();
}

std::vector<Vector2>& Block::getInitialHousePoints()
{
	return housePoints;
}