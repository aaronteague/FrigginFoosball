#include "MapMaker.h"
#include "LuaLoader.h"
#include "Block.h"

std::vector<int> MapMaker::numOfBlock;
std::vector<std::vector<std::string>> MapMaker::blockRegistry;

std::vector<std::vector<std::string>> MapMaker::arrangeBlocks(LuaLoader* loader)
{
	
	int worldBlockLength = loader->getInt("WORLD_BLOCK_LENGTH");
	int blockTileLength = loader->getInt("BLOCK_TILE_LENGTH");
	int numOfPonds = loader->getInt("NUM_OF_PONDS");
	int minNumOfTrees = loader->getInt("MIN_NUM_OF_TREES");
	int maxNumOfTrees = loader->getInt("MAX_NUM_OF_TREES");
	int numOfNeighborsToStart = loader->getInt("NUM_OF_NEIGHBORS_TO_START");
	int maxNumOfNeighbors = loader->getInt("MAX_NUM_OF_NEIGHBORS");
	int numOfNeighborFiles = loader->getInt("NUM_OF_NEIGHBOR_FILES");
//	int numOfPondBlockFiles = loader->getInt("NUM_OF_POND_BLOCK_FILES");
//	int numOfRegBlockFiles = loader->getInt("NUM_OF_REGULAR_BLOCKS_FILES");


	std::vector < std::vector<Block*>> blockMap;
	std::vector<std::vector<int>> blockMapInt;

	blockMap.resize(worldBlockLength);
	blockMapInt.resize(worldBlockLength);
	for (int i = 0; i < worldBlockLength; ++i){
		blockMap[i].resize(worldBlockLength);
		blockMapInt[i].resize(worldBlockLength);
	}

	MapMaker *mapMaker;
	bool acceptable = false;
	while (!acceptable){
		// lets build the initial map first, blank slate
		for (int i = 0; i < worldBlockLength; ++i)
			for (int j = 0; j < worldBlockLength; ++j)
				blockMapInt[i][j] = NORMAL;   // fill all normal
		blockMapInt[0][0] = CORNER_TOP_RIGHT;  // bottom left spot
		for (int i = 1; i < worldBlockLength - 1; i++)
			blockMapInt[i][0] = TOP;  // get the bottom row until the last
		blockMapInt[worldBlockLength - 1][0] = CORNER_TOP_LEFT;  // the tile on bottom right
		for (int i = 1; i < worldBlockLength - 1; i++)
			blockMapInt[0][i] = RIGHT;  // left side blocks
		for (int i = 1; i < worldBlockLength - 1; i++)
			blockMapInt[worldBlockLength - 1][i] = LEFT;   // right side
		blockMapInt[0][worldBlockLength - 1] = CORNER_BOTTOM_RIGHT;
		for (int i = 1; i < worldBlockLength - 1; i++)
			blockMapInt[i][worldBlockLength - 1] = BOTTOM;  // bottom side
		blockMapInt[worldBlockLength - 1][worldBlockLength - 1] = CORNER_BOTTOM_LEFT;


		std::vector<int> noGo;
		// first decide where if we want to start on the top, left, right, or bottom side
		int side = rand() % 4;  // num of sides
		if (side == 0){  // lets go from the left
			int blockY = rand() % (worldBlockLength - 4) + 2;
			mapMaker = new MapMaker(true, true, BLOCK_GOING_RIGHT, Vector2(0, blockY - 1), blockMapInt, worldBlockLength, noGo);
			//	mapMaker.push_back(temp);
		}
		else if (side == 1){  // lets go from the top
			int blockX = rand() % (worldBlockLength - 4) + 2;
			mapMaker = new MapMaker(true, true, BLOCK_GOING_DOWN, Vector2(blockX, worldBlockLength - 1), blockMapInt, worldBlockLength, noGo);
			//mapMaker = new MapMaker(NULL, true, true, BLOCK_GOING_DOWN, Vector2(blockX, worldBlockLength));
			//	mapMaker.push_back(temp);
		}
		else if (side == 2){  // lets start from the right
			int blockY = rand() % (worldBlockLength - 4) + 2;
			mapMaker = new MapMaker(true, true, BLOCK_GOING_LEFT, Vector2(worldBlockLength - 1, blockY), blockMapInt, worldBlockLength, noGo);
			//	mapMaker = new MapMaker(NULL, true, true, BLOCK_GOING_LEFT, Vector2(worldBlockLength, blockY));
			//	mapMaker.push_back(temp);
		}
		else{  // nope, we will start from the bottom
			int blockX = rand() % (worldBlockLength - 4) + 2;
			mapMaker = new MapMaker(true, true, BLOCK_GOING_UP, Vector2(blockX, 0), blockMapInt, worldBlockLength, noGo);
			//	mapMaker = new MapMaker(NULL, true, true, BLOCK_GOING_UP, Vector2(blockX, 0));
			//	mapMaker.push_back(temp);
		}
		//isSplit = mapMaker->hasSplit();
		int numOfSplits = 0;
		int numOfCollisions = 0;
		int numOfHorizontal = 0;
		int numOfVertical = 0;
		mapMaker->numOfCollisions(numOfCollisions);
		mapMaker->numOfSplits(numOfSplits);
		mapMaker->numOfTypeOfBlock(numOfHorizontal, TOP_BOTTOM, blockMapInt);
		mapMaker->numOfTypeOfBlock(numOfVertical, LEFT_RIGHT, blockMapInt);
		if (numOfSplits > 0 && numOfCollisions == 0 && numOfHorizontal > 4 && numOfVertical > 4){
			acceptable = true;
			std::vector<Vector2> positions;
			mapMaker->traceForBridges(positions, blockMapInt);  // find the bridges
		}
		delete(mapMaker);  // get rid of the object

	
	}
	

	numOfBlock.resize(NUM_OF_BLOCK_TYPES);
	blockRegistry.resize(NUM_OF_BLOCK_TYPES);
	luabridge::LuaRef blockList = loader->getTable("BlockFile");

	addToBlockRegistry(loader, "empty", "NUM_OF_EMPTY_BLOCK_FILES", SPACE);
	addToBlockRegistry(loader, "pond", "NUM_OF_POND_BLOCK_FILES", POND);
	addToBlockRegistry(loader, "normal", "NUM_OF_REGULAR_BLOCKS_FILES", NORMAL);
	addToBlockRegistry(loader, "top", "NUM_OF_TOP_BLOCK_FILES", TOP);
	addToBlockRegistry(loader, "bottom", "NUM_OF_BOTTOM_BLOCK_FILES", BOTTOM);
	addToBlockRegistry(loader, "left", "NUM_OF_LEFT_BLOCK_FILES", LEFT);
	addToBlockRegistry(loader, "right", "NUM_OF_RIGHT_BLOCK_FILES", RIGHT);
	addToBlockRegistry(loader, "top-bottom", "NUM_OF_TOP_BOTTOM_BLOCK_FILES", TOP_BOTTOM);
	addToBlockRegistry(loader, "left-right", "NUM_OF_LEFT_RIGHT_BLOCK_FILES", LEFT_RIGHT);
	addToBlockRegistry(loader, "top-left", "NUM_OF_TOP_LEFT_BLOCK_FILES", TOP_LEFT);
	addToBlockRegistry(loader, "top-right", "NUM_OF_TOP_RIGHT_BLOCK_FILES", TOP_RIGHT);
	addToBlockRegistry(loader, "bottom-left", "NUM_OF_BOTTOM_LEFT_BLOCK_FILES", BOTTOM_LEFT);
	addToBlockRegistry(loader, "bottom-right", "NUM_OF_BOTTOM_RIGHT_BLOCK_FILES", BOTTOM_RIGHT);
	addToBlockRegistry(loader, "top-left-corner-bottom-right", "NUM_OF_TOP_LEFT_CORNER_BOTTOM_RIGHT_BLOCK_FILES", TOP_LEFT_CORNER_BOTTOM_RIGHT);
	addToBlockRegistry(loader, "top-right-corner-bottom-left", "NUM_OF_TOP_RIGHT_CORNER_BOTTOM_LEFT_BLOCK_FILES", TOP_RIGHT_CORNER_BOTTOM_LEFT);
	addToBlockRegistry(loader, "bottom-left-corner-top-right", "NUM_OF_BOTTOM_LEFT_CORNER_TOP_RIGHT_BLOCK_FILES", BOTTOM_LEFT_CORNER_TOP_RIGHT);
	addToBlockRegistry(loader, "bottom-right-corner-top-left", "NUM_OF_BOTTOM_RIGHT_CORNER_TOP_LEFT_BLOCK_FILES", BOTTOM_RIGHT_CORNER_TOP_LEFT);
	addToBlockRegistry(loader, "top-bottom-bridge", "NUM_OF_TOP_BOTTOM_BRIDGE_BLOCK_FILES", TOP_BOTTOM_BRIDGE);
	addToBlockRegistry(loader, "left-right-bridge", "NUM_OF_LEFT_RIGHT_BRIDGE_BLOCK_FILES", LEFT_RIGHT_BRIDGE);
	addToBlockRegistry(loader, "corner-top-left", "NUM_OF_CORNER_TOP_LEFT_BLOCK_FILES", CORNER_TOP_LEFT);
	addToBlockRegistry(loader, "corner-top-right", "NUM_OF_CORNER_TOP_RIGHT_BLOCK_FILES", CORNER_TOP_RIGHT);
	addToBlockRegistry(loader, "corner-bottom-left", "NUM_OF_CORNER_BOTTOM_LEFT_BLOCK_FILES", CORNER_BOTTOM_LEFT);
	addToBlockRegistry(loader, "corner-bottom-right", "NUM_OF_CORNER_BOTTOM_RIGHT_BLOCK_FILES", CORNER_BOTTOM_RIGHT);
	addToBlockRegistry(loader, "corner-top-both", "NUM_OF_CORNER_TOP_BOTH_BLOCK_FILES", CORNER_TOP_BOTH);
	addToBlockRegistry(loader, "corner-bottom-both", "NUM_OF_CORNER_BOTTOM_BOTH_BLOCK_FILES", CORNER_BOTTOM_BOTH);
	addToBlockRegistry(loader, "corner-left-both", "NUM_OF_CORNER_LEFT_BOTH_BLOCK_FILES", CORNER_BOTH_LEFT);
	addToBlockRegistry(loader, "corner-right-both", "NUM_OF_CORNER_RIGHT_BOTH_BLOCK_FILES", CORNER_BOTH_RIGHT);
	addToBlockRegistry(loader, "bottom-corner-top-both", "NUM_OF_BOTTOM_CORNER_TOP_BOTH_BLOCK_FILES", BOTTOM_CORNER_TOP_BOTH);
	addToBlockRegistry(loader, "top-corner-bottom-both", "NUM_OF_TOP_CORNER_BOTTOM_BOTH_BLOCK_FILES", TOP_CORNER_BOTTOM_BOTH);
	addToBlockRegistry(loader, "left-corner-right-both", "NUM_OF_LEFT_CORNER_RIGHT_BOTH_BLOCK_FILES", LEFT_CORNER_BOTH_RIGHT);
	addToBlockRegistry(loader, "right-corner-left-both", "NUM_OF_RIGHT_CORNER_LEFT_BOTH_BLOCK_FILES", RIGHT_CORNER_BOTH_LEFT);

	// convert block ints into block files
	std::vector<std::vector<std::string>> blockFileMap;
	blockFileMap.resize(worldBlockLength);
	for (int i = 0; i < worldBlockLength; ++i)
		blockFileMap[i].resize(worldBlockLength);

	for (int i = 0; i < worldBlockLength; ++i)
		for (int j = 0; j < worldBlockLength; ++j){
			int randBlock;
			randBlock = rand() % numOfBlock[blockMapInt[i][j]];
			blockFileMap[i][j] = blockRegistry[blockMapInt[i][j]][randBlock];
		}



	

	return blockFileMap;
}

MapMaker::~MapMaker()
{

}
void MapMaker::traceForBridges(std::vector<Vector2> &positions, std::vector<std::vector<int>> &blockMapInt)
{
	if (isTurn || isSplit || child.size() == 0)
	{
		if (positions.size() > 0){
			int whichOne = rand() % positions.size();
			Vector2 pos = positions[whichOne];
			if (!isOutBounds(worldBlockLength, pos))
				if (blockMapInt[pos.x][pos.y] == TOP_BOTTOM)
					blockMapInt[pos.x][pos.y] = TOP_BOTTOM_BRIDGE;
				else
					blockMapInt[pos.x][pos.y] = LEFT_RIGHT_BRIDGE;
			positions.clear();
		}
	}
	else{
		positions.push_back(currentPosition);
	}

	for (unsigned int i = 0; i < child.size(); i++){
		child[i]->traceForBridges(positions, blockMapInt);
	}
}
MapMaker::MapMaker(bool isFirst, bool turnLock, int directionGoing, Vector2 newPosition, std::vector<std::vector<int>> &blockMapInt, int worldBlockLength, std::vector<int> directionNoGo)
	: isFirst(isFirst), turnLock(turnLock), currentPosition(newPosition), blockMapInt(blockMapInt), worldBlockLength(worldBlockLength), collided(false), modified(false)
{
	isSplit = false;
	isTurn = false;
	//	int newDirection;
	collided = isCollided(directionGoing);

	//	directionNoGo.push_back(getOpposite(directionGoing));
	if (isFirst){  // if this is the first time, just go with it

		// go forward with it

		switch (directionGoing){
		case BLOCK_GOING_UP:
			//	newDirection = directionGoing;
			blockMapInt[currentPosition.x][currentPosition.y] = CORNER_TOP_BOTH;
			child.push_back(new MapMaker(false, true, directionGoing, Vector2(currentPosition.x, currentPosition.y + 1), blockMapInt, worldBlockLength, directionNoGo));
			break;
		case BLOCK_GOING_DOWN:
			blockMapInt[currentPosition.x][currentPosition.y] = CORNER_BOTTOM_BOTH;
			child.push_back(new MapMaker(false, true, directionGoing, Vector2(currentPosition.x, currentPosition.y - 1), blockMapInt, worldBlockLength, directionNoGo));
			break;
		case BLOCK_GOING_LEFT:
			blockMapInt[currentPosition.x][currentPosition.y] = CORNER_BOTH_LEFT;
			child.push_back(new MapMaker(false, true, directionGoing, Vector2(currentPosition.x - 1, currentPosition.y), blockMapInt, worldBlockLength, directionNoGo));
			break;
		case BLOCK_GOING_RIGHT:
			blockMapInt[currentPosition.x][currentPosition.y] = CORNER_BOTH_RIGHT;
			child.push_back(new MapMaker(false, true, directionGoing, Vector2(currentPosition.x + 1, currentPosition.y), blockMapInt, worldBlockLength, directionNoGo));
			break;
		}

	}

	else if (isOutBounds(worldBlockLength, currentPosition)){  // if not first, check to see if it is on the edge.  if it is, just stop making children
		// stahp
		if (currentPosition.x == 0){ // the left side of the board
			blockMapInt[currentPosition.x][currentPosition.y] = CORNER_BOTH_RIGHT;
		}
		else if (currentPosition.x == worldBlockLength - 1){  // on the right side of the board
			blockMapInt[currentPosition.x][currentPosition.y] = CORNER_BOTH_LEFT;
		}
		else if (currentPosition.y == 0){  // if on the bottom side of board
			blockMapInt[currentPosition.x][currentPosition.y] = CORNER_TOP_BOTH;
		}
		else if (currentPosition.y == worldBlockLength - 1){  // and if on top of board
			blockMapInt[currentPosition.x][currentPosition.y] = CORNER_BOTTOM_BOTH;
		}
		else{
			// something went wrong if we are here
			int i = 7;
		}
	}
	else if (turnLock){  // if the turnLock is on, you could only continue with the direction you were headed
		Vector2 newPosition;  // this overides the other "newPosition"
		switch (directionGoing){
		case BLOCK_GOING_UP:
			blockMapInt[currentPosition.x][currentPosition.y] = LEFT_RIGHT;
			newPosition.set(currentPosition.x, currentPosition.y + 1);
			break;
		case BLOCK_GOING_DOWN:
			blockMapInt[currentPosition.x][currentPosition.y] = LEFT_RIGHT;
			newPosition.set(currentPosition.x, currentPosition.y - 1);
			break;
		case BLOCK_GOING_LEFT:
			blockMapInt[currentPosition.x][currentPosition.y] = TOP_BOTTOM;
			newPosition.set(currentPosition.x - 1, currentPosition.y);
			break;
		case BLOCK_GOING_RIGHT:
			blockMapInt[currentPosition.x][currentPosition.y] = TOP_BOTTOM;
			newPosition.set(currentPosition.x + 1, currentPosition.y);
			break;
		}

		child.push_back(new MapMaker(false, false, directionGoing, newPosition, blockMapInt, worldBlockLength, directionNoGo));
	}

	else{ // all else is false.  See if we want to turn, if not, we keep going straight with it.
		// check if we want to turn
		if (rand() % TURN_PROBABILITY == 0 && !isOutBounds(worldBlockLength, nextVec(currentPosition, directionGoing))){  // turn time
			int turn = rand() % 3;
			switch (directionGoing){
			case BLOCK_GOING_UP:
				if (turn == 0 && directionAllowed(BLOCK_GOING_LEFT, directionNoGo)){ // turn left
					blockMapInt[currentPosition.x][currentPosition.y] = TOP_RIGHT_CORNER_BOTTOM_LEFT;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_LEFT, nextVec(currentPosition, BLOCK_GOING_LEFT), blockMapInt, worldBlockLength, directionNoGo));
					isTurn = true;
				}
				else if (turn == 1 && directionAllowed(BLOCK_GOING_RIGHT, directionNoGo)){  // turn right
					blockMapInt[currentPosition.x][currentPosition.y] = TOP_LEFT_CORNER_BOTTOM_RIGHT;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_RIGHT, nextVec(currentPosition, BLOCK_GOING_RIGHT), blockMapInt, worldBlockLength, directionNoGo));
					isTurn = true;
				}
				else if (turn == 2 && directionAllowed(BLOCK_GOING_LEFT, directionNoGo) && directionAllowed(BLOCK_GOING_RIGHT, directionNoGo)){ // split up
					isSplit = true;
					blockMapInt[currentPosition.x][currentPosition.y] = TOP_CORNER_BOTTOM_BOTH;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_LEFT, nextVec(currentPosition, BLOCK_GOING_LEFT), blockMapInt, worldBlockLength, directionNoGo));  // this one goes left
					child.push_back(new MapMaker(false, false, BLOCK_GOING_RIGHT, nextVec(currentPosition, BLOCK_GOING_RIGHT), blockMapInt, worldBlockLength, directionNoGo));  // this one goes right
				}
				else{  // probably not allowed to turn the direction we want
					blockMapInt[currentPosition.x][currentPosition.y] = LEFT_RIGHT;
					child.push_back(new MapMaker(false, false, directionGoing, nextVec(currentPosition, directionGoing), blockMapInt, worldBlockLength, directionNoGo));
				}
				break;
			case BLOCK_GOING_DOWN:
				if (turn == 0 && directionAllowed(BLOCK_GOING_LEFT, directionNoGo)){  // turn left
					blockMapInt[currentPosition.x][currentPosition.y] = BOTTOM_RIGHT_CORNER_TOP_LEFT;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_LEFT, nextVec(currentPosition, BLOCK_GOING_LEFT), blockMapInt, worldBlockLength, directionNoGo));
					isTurn = true;
				}
				else if (turn == 1 && directionAllowed(BLOCK_GOING_RIGHT, directionNoGo)){  // turn right
					blockMapInt[currentPosition.x][currentPosition.y] = BOTTOM_LEFT_CORNER_TOP_RIGHT;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_RIGHT, nextVec(currentPosition, BLOCK_GOING_RIGHT), blockMapInt, worldBlockLength, directionNoGo));
					isTurn = true;
				}
				else if (turn == 2 && directionAllowed(BLOCK_GOING_LEFT, directionNoGo) && directionAllowed(BLOCK_GOING_RIGHT, directionNoGo)){  // split up
					blockMapInt[currentPosition.x][currentPosition.y] = BOTTOM_CORNER_TOP_BOTH;
					isSplit = true;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_LEFT, nextVec(currentPosition, BLOCK_GOING_LEFT), blockMapInt, worldBlockLength, directionNoGo));  // left
					child.push_back(new MapMaker(false, false, BLOCK_GOING_RIGHT, nextVec(currentPosition, BLOCK_GOING_RIGHT), blockMapInt, worldBlockLength, directionNoGo));  // right
				}
				else{  // probably not allowed to turn the direction we want
					blockMapInt[currentPosition.x][currentPosition.y] = LEFT_RIGHT;
					child.push_back(new MapMaker(false, false, directionGoing, nextVec(currentPosition, directionGoing), blockMapInt, worldBlockLength, directionNoGo));
				}
				break;
			case BLOCK_GOING_LEFT:
				if (turn == 0 && directionAllowed(BLOCK_GOING_UP, directionNoGo)){  // turn up
					blockMapInt[currentPosition.x][currentPosition.y] = BOTTOM_LEFT_CORNER_TOP_RIGHT;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_UP, nextVec(currentPosition, BLOCK_GOING_UP), blockMapInt, worldBlockLength, directionNoGo));
					isTurn = true;
				}
				else if (turn == 1 && directionAllowed(BLOCK_GOING_DOWN, directionNoGo)){  // turn down (for what)
					blockMapInt[currentPosition.x][currentPosition.y] = TOP_LEFT_CORNER_BOTTOM_RIGHT;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_DOWN, nextVec(currentPosition, BLOCK_GOING_DOWN), blockMapInt, worldBlockLength, directionNoGo));
					isTurn = true;
				}
				else if (turn == 2 && directionAllowed(BLOCK_GOING_UP, directionNoGo) && directionAllowed(BLOCK_GOING_DOWN, directionNoGo)){  // split up
					blockMapInt[currentPosition.x][currentPosition.y] = LEFT_CORNER_BOTH_RIGHT;
					isSplit = true;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_UP, nextVec(currentPosition, BLOCK_GOING_UP), blockMapInt, worldBlockLength, directionNoGo));  // up
					child.push_back(new MapMaker(false, false, BLOCK_GOING_DOWN, nextVec(currentPosition, BLOCK_GOING_DOWN), blockMapInt, worldBlockLength, directionNoGo));  // right
				}
				else{  // probably not allowed to turn the direction we want
					blockMapInt[currentPosition.x][currentPosition.y] = TOP_BOTTOM;
					child.push_back(new MapMaker(false, false, directionGoing, nextVec(currentPosition, directionGoing), blockMapInt, worldBlockLength, directionNoGo));
				}
				break;
			case BLOCK_GOING_RIGHT:
				if (turn == 0 && directionAllowed(BLOCK_GOING_UP, directionNoGo)){  // turn up
					blockMapInt[currentPosition.x][currentPosition.y] = BOTTOM_RIGHT_CORNER_TOP_LEFT;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_UP, nextVec(currentPosition, BLOCK_GOING_UP), blockMapInt, worldBlockLength, directionNoGo));
					isTurn = true;
				}
				else if (turn == 1 && directionAllowed(BLOCK_GOING_DOWN, directionNoGo)){  // turn down
					blockMapInt[currentPosition.x][currentPosition.y] = TOP_RIGHT_CORNER_BOTTOM_LEFT;
					child.push_back(new MapMaker(false, false, BLOCK_GOING_DOWN, nextVec(currentPosition, BLOCK_GOING_DOWN), blockMapInt, worldBlockLength, directionNoGo));
					isTurn = true;
				}
				else if (turn == 2 && directionAllowed(BLOCK_GOING_UP, directionNoGo) && directionAllowed(BLOCK_GOING_DOWN, directionNoGo)){  // split it
					blockMapInt[currentPosition.x][currentPosition.y] = RIGHT_CORNER_BOTH_LEFT;
					isSplit = true;
					child.push_back(new MapMaker(false, true, BLOCK_GOING_UP, nextVec(currentPosition, BLOCK_GOING_UP), blockMapInt, worldBlockLength, directionNoGo));
					child.push_back(new MapMaker(false, true, BLOCK_GOING_DOWN, nextVec(currentPosition, BLOCK_GOING_DOWN), blockMapInt, worldBlockLength, directionNoGo));
				}
				else{  // probably not allowed to turn the direction we want
					blockMapInt[currentPosition.x][currentPosition.y] = TOP_BOTTOM;
					child.push_back(new MapMaker(false, false, directionGoing, nextVec(currentPosition, directionGoing), blockMapInt, worldBlockLength, directionNoGo));
				}
				break;
			}
		}
		else{  // ok, so no turning
			Vector2 newPosition; // overides other "newPosition"
			if (directionGoing == BLOCK_GOING_DOWN){
				blockMapInt[currentPosition.x][currentPosition.y] = LEFT_RIGHT;
				newPosition.set(currentPosition.x, currentPosition.y - 1);
			}
			else if (directionGoing == BLOCK_GOING_UP){
				blockMapInt[currentPosition.x][currentPosition.y] = LEFT_RIGHT;
				newPosition.set(currentPosition.x, currentPosition.y + 1);
			}
			else if (directionGoing == BLOCK_GOING_LEFT){
				blockMapInt[currentPosition.x][currentPosition.y] = TOP_BOTTOM;
				newPosition.set(currentPosition.x - 1, currentPosition.y);
			}
			else if (directionGoing == BLOCK_GOING_RIGHT){
				blockMapInt[currentPosition.x][currentPosition.y] = TOP_BOTTOM;
				newPosition.set(currentPosition.x + 1, currentPosition.y);
			}
			child.push_back(new MapMaker(false, false, directionGoing, newPosition, blockMapInt, worldBlockLength, directionNoGo));
		}
	}

}

bool MapMaker::isOutBounds(int worldBlockLength, Vector2 position)
{
	if (position.x == 0){
		return true;
	}
	else if (position.x == worldBlockLength - 1){
		return true;
	}
	else if (position.y == 0){
		return true;
	}
	else if (position.y == worldBlockLength - 1){
		return true;
	}
	else{
		return false;
	}

}

Vector2 MapMaker::nextVec(Vector2 currentVector, int directionGoing)
{
	Vector2 newVec;
	if (directionGoing == BLOCK_GOING_UP){
		newVec.set(currentVector.x, currentVector.y + 1);
	}
	else if (directionGoing == BLOCK_GOING_DOWN){
		newVec.set(currentVector.x, currentVector.y - 1);
	}
	else if (directionGoing == BLOCK_GOING_LEFT){
		newVec.set(currentVector.x - 1, currentVector.y);
	}
	else if (directionGoing == BLOCK_GOING_RIGHT){
		newVec.set(currentVector.x + 1, currentVector.y);
	}
	else{
		int i = 7; // something went wrong
	}
	return newVec;;
}

int MapMaker::getOpposite(int direction)
{
	if (direction == BLOCK_GOING_UP){
		return BLOCK_GOING_DOWN;
	}
	else if (direction == BLOCK_GOING_DOWN){
		return BLOCK_GOING_UP;
	}
	else if (direction == BLOCK_GOING_LEFT){
		return BLOCK_GOING_RIGHT;
	}
	else if (direction == BLOCK_GOING_RIGHT){
		return BLOCK_GOING_LEFT;
	}
	else{
		return -1;  // something went wrong
	}
}

bool MapMaker::directionAllowed(int direction, std::vector<int> directionNoGo)
{
	for (unsigned int i = 0; i < directionNoGo.size(); i++){
		if (direction == directionNoGo[i])
			return false;
	}
	return true;
}


void MapMaker::numOfSplits(int &counter)
{
	if (isSplit == true)
		++counter;
	for (unsigned int i = 0; i < child.size(); ++i){
		child[i]->numOfSplits(counter);
	}

}

void MapMaker::numOfCollisions(int &counter)
{
	if (collided == true)
		++counter;
	for (unsigned int i = 0; i < child.size(); ++i){
		child[i]->numOfCollisions(counter);
	}
}

void MapMaker::numOfTypeOfBlock(int &counter, int blockType, std::vector<std::vector<int>> blockMapInt)
{
	for (unsigned int i = 0; i < blockMapInt.size(); ++i){
		for (unsigned int j = 0; j < blockMapInt[i].size(); ++j){
			if (blockMapInt[i][j] == blockType)
				++counter;
		}
	}
}

bool MapMaker::isCollided(int direction)
{
	int collided = false;
	std::vector<Vector2> positions;
	positions.reserve(3);
	switch (direction)
	{
	case BLOCK_GOING_UP:
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_UP));
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_LEFT));
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_RIGHT));
		break;
	case BLOCK_GOING_DOWN:
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_DOWN));
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_LEFT));
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_RIGHT));
		break;
	case BLOCK_GOING_LEFT:
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_LEFT));
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_UP));
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_DOWN));
		break;
	case BLOCK_GOING_RIGHT:
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_RIGHT));
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_UP));
		positions.push_back(nextVec(currentPosition, BLOCK_GOING_DOWN));
		break;
	}

	for (unsigned int i = 0; i < positions.size(); ++i){
		if (!isOutBounds(worldBlockLength, currentPosition) && !isOutBounds(worldBlockLength, positions[i]))
		{
			int blockInt = blockMapInt[positions[i].x][positions[i].y];
			if (blockInt == NORMAL || blockInt == TOP || blockInt == BOTTOM || blockInt == LEFT || blockInt == RIGHT)
			{

			}
			//	collided = false;
			else
				collided = true;
		}
	}
	return collided;

}

void MapMaker::addToBlockRegistry(LuaLoader* loader, std::string category, std::string numOfConstant, int index)
{
	luabridge::LuaRef blockFile = loader->getTable("BlockFile");
	luabridge::LuaRef catTable = loader->getTable(category, blockFile);
	int num = loader->getInt(numOfConstant);
	numOfBlock[index] = loader->getInt(numOfConstant);
	for (int i = 0; i < numOfBlock[index]; ++i){
		std::string tempStr = loader->getString(i + 1, catTable);
		blockRegistry[index].push_back(tempStr);
	}
}