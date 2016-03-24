#include "Landscape.h"
#include "Entity.h"
#include "MyCamera.h"
#include "MapMaker.h"
//#include "Dependencies\Box2D\Box2D.h"
#include "Box2D/Box2d.h"
#include "Player.h"
#include "Item.h"
#include "Menus/Inventory.h"
#include "Weather.h"
#include "MatSystem.h"
#include "WallSystem.h"
//#include "Menus/TimeAndDate.h"




Landscape::Landscape(int screenWidth, int screenHeight)
{
	
	roomName = NO_ROOM;
	screenSize.set(screenWidth, screenHeight);
	focusedMenu = NULL;
	test = false;
	accumulator = 0;
	_scene = NULL;

	// let's check out the wallsystem thing


}
void Landscape::create(LuaLoader* loader)
{
//	Entity::ClearLists(); // makes sure that everything is fresh in case it was trying to load something previously

	
	b2Vec2 gravity(0.0f, 0.0f);  // no gravity
	physWorld = new b2World(gravity);  // create your world

	// load up the index file
	loader->setFile("res/luaFiles/landscapeIndex.lua");

	// create the filemap to use for the world
	blockFileMap = MapMaker::arrangeBlocks(loader);

	worldBlockLength = loader->getInt("WORLD_BLOCK_LENGTH");
	blockTileLength = loader->getInt("BLOCK_TILE_LENGTH");
	int maxNumOfTrees = loader->getInt("MAX_NUM_OF_TREES");
	int minNumOfTrees = loader->getInt("MIN_NUM_OF_TREES");

	weather = new Weather(Weather::RAIN, -35, screenSize, Vector2(worldBlockLength * blockTileLength, worldBlockLength * blockTileLength));

	// test of the mat system
	matSystem = new MatSystem(worldBlockLength * blockTileLength, worldBlockLength * blockTileLength);
	matSystem->addMat("res/Mats/house.png", Vector2(55, 55));
	matSystem->addMat("res/Mats/house.png", Vector2(55, 56));
	matSystem->addMat("res/Mats/tree.png", Vector2(53, 55));
	matSystem->addMat("res/Mats/plant.png", Vector2(50, 50));
	matSystem->addMat("res/Mats/house.png", Vector2(51, 51));
	
	// set up space for the tilemap
	tileMap.resize(worldBlockLength * blockTileLength);
	for (unsigned int i = 0; i < tileMap.size(); i++)
		tileMap[i].resize(worldBlockLength * blockTileLength);

	// retrieve the block barrier data
	blockMap.resize(worldBlockLength);
	for (unsigned int i = 0; i < blockFileMap.size(); ++i){
		blockMap[i].resize(worldBlockLength);
		for (unsigned int j = 0; j < blockFileMap[i].size(); ++j){
			Block* temp = new Block(loader, blockFileMap[i][j], Vector2(i, j), true);
			blockMap[i][j] = temp;
			std::vector<Vector4> blockBarriers = temp->getInitialBarriers();
			for (unsigned int k = 0; k < blockBarriers.size(); ++k){
				Vector4 adjBarrier(
					blockBarriers[k].x + i * blockTileLength,
					blockBarriers[k].y + j * blockTileLength,
					blockBarriers[k].z + i * blockTileLength,
					blockBarriers[k].w + j * blockTileLength);
				barriers.push_back(adjBarrier);
				buildBarrier(adjBarrier);
			}

			// arrange the tiles
			std::vector < std::vector<Tile*>> tileMap = temp->getInitialTileMap();
			for (int k = 0; k < blockTileLength; ++k)
				for (int l = 0; l < blockTileLength; ++l){
					Tile *tile = tileMap[k][l];
					tile->position.set(i * blockTileLength + k, j * blockTileLength + l);
					if (tile->type == Tile::ITEM)
						tile->childItem = Item::Create(loader, tileMap[k][l]->luaFile
						, tile->position, Item::SOUTH, Item::ABSTRACT, physWorld);
					
					PlayField::tileMap[tile->position.x]
						[tile->position.y] = tile;
				}
		}
	}

	// gather house points
	for (unsigned int i = 0; i < blockMap.size(); ++i)
		for (unsigned int j = 0; j < blockMap[i].size(); ++j){
			std::vector<Vector2> blockPoints
				= blockMap[i][j]->getInitialHousePoints();
			for (unsigned int k = 0; k < blockPoints.size(); ++k)
				housePoints.push_back(Vector2
					(blockPoints[k].x + i * blockTileLength
					, blockPoints[k].y + j * blockTileLength));
			}

	// fill with trees
	int numOfTrees = rand() % (maxNumOfTrees - minNumOfTrees)
		+ minNumOfTrees;
	
	int counter = 0;
	while (counter < numOfTrees){
		// can't be at the very edges
		int x = rand() % (tileMap.size() - 2) + 1;
		int y = rand() % (tileMap.size() - 2) + 1;
		// check a 3 x 3 grid to make sure it isn't in the way
		bool collided = false;
		for (int j = -1; j <= 1; ++j)
			for (int k = -1; k <= 1; ++k)
				if (tileMap[x + j][y + k]->type != Tile::EMPTY)
					collided = true;

		if (!collided){
			tileMap[x][y]->luaFile = "res/Items/Plants/tree.lua";

			tileMap[x][y]->childItem = Item::Create(loader, tileMap[x][y]->luaFile
				, Vector2(x, y), Item::WEST, Item::ABSTRACT, physWorld);
			tileMap[x][y]->type = Tile::ITEM;
			
			counter++;
		}
	}

	
	// check out the wallsystem
//	tileMap[33][30]->northWallpaper = "res/WallPaper/testWallpaper.png";
//	tileMap[34][29]->northWallpaper = "res/Doors/Door.lua";
//	tileMap[36][30]->northWallpaper = "res/WallPaper/testWallpaper.png";
//	tileMap[37][30]->northWallpaper = "res/WallPaper/testWallpaper.png";
//	tileMap[37][30]->eastWallpaper = "res/WallPaper/testWallpaper.png";
//	tileMap[36][28]->eastWallpaper = "res/Doors/Door.lua";
//	tileMap[37][27]->eastWallpaper = "res/WallPaper/testWallpaper.png";
//	wallSystem = new WallSystem(tileMap, loader);
//	std::vector<Vector4> wallBarriers = wallSystem->getBarriers();
//	for (unsigned int i = 0; i < wallBarriers.size(); ++i)
//		buildBarrier(wallBarriers[i]);

//	weather->updateWetAreas(tileMap);
}



void Landscape::Update(float &elapsedTime)
{
	PlayField::Update(elapsedTime);

	if (weather != NULL)
		weather->Update(elapsedTime, _camera->GetNode()->getTranslation()); // needs to get camera's position


	
}
void Landscape::Render()
{
	const Matrix viewProjMatrix = _camera->GetNode()->getCamera()->getViewProjectionMatrix();

	if (weather != NULL)
		weather->RenderClouds(viewProjMatrix);

	PlayField::Render();
	
	
}

//void Landscape::addHouse(Item* house)
//{
//	
//	int houseIndex = MATH_RANDOM_0_1() * housePoints.size() - 1;
//	if (houseIndex != -1){
//		Vector2 pos = housePoints[houseIndex];
//		putItemInTile(house, pos.x, pos.y);
//	}
//}

void Landscape::addHouseFile(LuaLoader* loader, std::string houseFile)
{
	_scene = NULL;
//	Game::getAbsoluteTime();
	srand(Game::getAbsoluteTime());
	houseFiles.push_back(houseFile);
	bool continueLoop = true; // find a house point for house to stay
	Vector2 vec;
	while (continueLoop){
		
		//int housePointIndex = rand() % housePoints.size() - 1;

		// replace random house position for now, for testing
		int housePointIndex = housePoints.size() - 1;
		vec = housePoints[housePointIndex];
		if (tileMap[vec.x][vec.y]->type != Tile::ITEM)
			continueLoop = false;
	}
	// clear the way
	// clear a 6 * 6 space for our 4 * 4 house
	for (int i = vec.x - 1; i < (int)vec.x + 4 + 1; ++i)
		for (int j = vec.y - 1; j < (int)vec.y + 4 + 1; ++j)
			if (tileMap[i][j]->type == Tile::ITEM)
				// there seems to be a problem here sometimes...
				removeItemFromTile(i, j);


	// now add the damn thing
	Item* item = Item::Create(loader, houseFile, vec, Item::SOUTH, Item::ABSTRACT, physWorld);
	
	putItemInTile(item, vec.x, vec.y);
}

void Landscape::addCharacter(LuaLoader* loader, Character* character)
{
	PlayField::addCharacter(character);
	// check for existance of house file
	std::string houseFile = character->getHouseFile();
	int index = -1;
	for (unsigned int i = 0; i < houseFiles.size(); ++i)
		if (houseFiles[i].compare(houseFile) == 0)
			index = i;
	if (index == -1)  // if the house isn't already in list
		addHouseFile(loader, houseFile);
	
	// set character position to random house point
	int housePointIndex = rand() % housePoints.size();

	Vector2 pos = housePoints[housePointIndex];
	character->SetPosition(pos.x, pos.y);
	
	
}

luabridge::LuaRef Landscape::save()
{
	luabridge::LuaRef ref = PlayField::save();
	
	luabridge::LuaRef pointsRef = luabridge::newTable(L);
	for (unsigned int i = 0; i < housePoints.size(); ++i){
		luabridge::LuaRef point = luabridge::newTable(L);
		point[1] = housePoints[i].x;
		point[2] = housePoints[i].y;
		pointsRef[i + 1] = point;
	}
	ref["housePoints"] = pointsRef;
	
	luabridge::LuaRef houseFilesRef = luabridge::newTable(L);
	for (unsigned int i = 0; i < houseFiles.size(); ++i){
		houseFilesRef[i + 1] = houseFiles[i];
	}
	ref["houseFiles"] = houseFilesRef;

	return ref;
}
void Landscape::load(luabridge::LuaRef &landscapeRef, LuaLoader* loader)
{
	PlayField::load(landscapeRef, loader);
	
	luabridge::LuaRef pointsTable = landscapeRef["housePoints"];
	int counter = 1;
	while (true){
		luabridge::LuaRef pointRef = pointsTable[counter];
		if (pointRef.isNil())
			break;

		float x = pointRef[1];
		float y = pointRef[2];

		Vector2 point(x, y);
		housePoints.push_back(point);
		counter++;
	}

	counter = 1;
	luabridge::LuaRef houseFilesTable = landscapeRef["houseFiles"];
	while (true){
		luabridge::LuaRef fileRef = houseFilesTable[counter];
		if (fileRef.isNil())
			break;
		std::string houseFile = fileRef;
		houseFiles.push_back(houseFile);
		counter++;
	}

//	luabridge::LuaRef weatherRef = landscapeRef["weather"];
//	weather = new Weather(Weather::RAIN, -10, screenSize);
//	weather->load(weatherRef);
}