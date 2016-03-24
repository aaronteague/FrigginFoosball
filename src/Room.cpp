#include "Room.h"
//#include "Dependencies\Box2D\Box2D.h"
#include "Box2D/Box2d.h"
#include "Weather.h"
#include "MatSystem.h"
#include "Character.h"
#include "WallSystem.h"
#include "MyCamera.h"


Room::Room(std::string luaFile, LuaLoader* loader)
:PlayField()
{
//	Room::loader = loader;
	Room::luaFile = luaFile;
	Room::roomName = roomName;
	_scene = NULL;
	_camera = NULL;
	physWorld = NULL;
	focusedMenu = NULL;
	weather = NULL;
	matSystem = NULL;
	touchDownPos.set(-1, -1);
}

void Room::Prepare()
{
	createFromFile(luaFile);
//	createFromTest();
	PlayField::Prepare();
}
void Room::prePrepare()
{
	//LuaLoader* loader = new LuaLoader(L);

	//// set up physics first
	//b2Vec2 gravity(0.0f, 0.0f); // no gravity
	//physWorld = new b2World(gravity);

	//// add weather // clear up the rain too later



	//// set up the block
	//Block* block = new Block(loader, luaFile, Vector2(0, 0), true);
	//std::vector<Block*> temp;
	//temp.push_back(block);
	//blockMap.push_back(temp);

	//// set up the tileMap
	//tileMap = block->getInitialTileMap();
	////fix positions

	//wallSystem = new WallSystem(tileMap, loader);

	//// get start points
	//startPoints = blockMap[0][0]->getStartPoints();

	//blockTileLength = tileMap.size();
	//worldBlockLength = 1;

	//weather = new Weather(Weather::RAIN, -35, screenSize);

	//matSystem = new MatSystem(blockTileLength, blockTileLength);
	//// add mats
	//for (unsigned int i = 0; i < tileMap.size(); ++i)
	//for (unsigned int j = 0; j < tileMap.size(); ++j)
	//if (tileMap[i][j]->mat.compare("none") != 0)
	//	matSystem->addMat(tileMap[i][j]->mat, Vector2(i, j));

	//// build the barriers
	//barriers = block->getInitialBarriers();
	////	barriers.push_back(Vector4(0, 0, blockTileLength, 0)); // bottom
	////	barriers.push_back(Vector4(blockTileLength, 0, blockTileLength, blockTileLength)); // right
	////	barriers.push_back(Vector4(blockTileLength, blockTileLength, 0, blockTileLength)); // top
	////	barriers.push_back(Vector4(0, blockTileLength, 0, 0)); // left

	//for (unsigned int i = 0; i < barriers.size(); ++i)
	//	buildBarrier(barriers[i]);

	//delete(loader);
}
void Room::createFromFile(std::string luaFile)
{

	LuaLoader* loader = new LuaLoader(L);
	loader->setFile(luaFile);

	// set up physics first
	b2Vec2 gravity(0.0f, 0.0f); // no gravity
	physWorld = new b2World(gravity);

	// add weather // clear up the rain too later



	// set up the block
	//Block* block = new Block(loader, luaFile, Vector2(0,0), true);
	//std::vector<Block*> temp;
	//temp.push_back(block);
	//blockMap.push_back(temp);

	// set up the tileMap
	//tileMap = block->getInitialTileMap();
	int counterX = 0;
	int counterY = 0;
	luabridge::LuaRef blockRef = loader->getTable("block");
	luabridge::LuaRef tileTable = loader->getTable("Tiles", blockRef);
	while (true){
		luabridge::LuaRef xRef = loader->getTable(counterX + 1, tileTable);
		if (xRef.isNil())
			break;
		std::vector<Tile*> temp;
		counterY = 0;
		tileMap.push_back(temp);
		while (true){
			luabridge::LuaRef yRef = loader->getTable(counterY + 1, xRef);
			if (yRef.isNil())
				break;
			Tile* tile = new Tile();
			tile->mat = loader->getString("Mat", yRef);
			tile->type = loader->getInt("Type", yRef);
			tile->northWallpaper = loader->getString("North", yRef);
			tile->westWallpaper = loader->getString("West", yRef);
			tile->eastWallpaper = loader->getString("East", yRef);
			tile->southWallpaper = loader->getString("South", yRef);
			tile->fertility = 0;
			tile->ownedBy.set(0, 0);
			tileMap[counterX].push_back(tile);
			
			counterY++;
		}
		counterX++;
	}
	//fix positions

	wallSystem = new WallSystem(tileMap, loader);
	
	// get start points
//	startPoints = blockMap[0][0]->getStartPoints();
	startPoints.push_back(Vector3(3, 3, 0));

	blockTileLength = tileMap.size();
	worldBlockLength = 1;

//	weather = new Weather(Weather::RAIN, -35, screenSize);

	matSystem = new MatSystem(blockTileLength, blockTileLength);
	// add mats
	for (unsigned int i = 0; i < tileMap.size(); ++i)
		for (unsigned int j = 0; j < tileMap.size(); ++j)
			if (tileMap[i][j]->mat.compare("") != 0)
				matSystem->addMat(tileMap[i][j]->mat, Vector2(i, j));

	// build the barriers
		std::vector<Vector4> barriers =	wallSystem->getBarriers();
//	barriers.push_back(Vector4(0, 0, blockTileLength, 0)); // bottom
//	barriers.push_back(Vector4(blockTileLength, 0, blockTileLength, blockTileLength)); // right
//	barriers.push_back(Vector4(blockTileLength, blockTileLength, 0, blockTileLength)); // top
//	barriers.push_back(Vector4(0, blockTileLength, 0, 0)); // left

	for (unsigned int i = 0; i < barriers.size(); ++i)
		buildBarrier(barriers[i]);

	delete(loader);
}

void Room::createFromTest()
{

}

void Room::Update(float &elapsedTime)
{
	PlayField::Update(elapsedTime);
	wallSystem->Update(_camera->GetTarget()->getTranslation());
}
void Room::Render()
{
	PlayField::Render();

	const Matrix viewProjMatrix = _camera->GetNode()->getCamera()->getViewProjectionMatrix();
	wallSystem->Render(viewProjMatrix);
}

Vector3 Room::getPointFrom(int roomName){
	// add some error prevention to this
	return startPoints[roomName];
}

luabridge::LuaRef Room::save()
{
	luabridge::LuaRef ref = PlayField::save();



	return ref;
}
void Room::load(luabridge::LuaRef &landscapeRef, LuaLoader* loader)
{
	PlayField::load(landscapeRef, loader);

	wallSystem = new WallSystem(tileMap, loader);
}