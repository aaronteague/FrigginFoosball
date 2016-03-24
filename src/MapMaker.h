#include "gameplay.h"

using namespace gameplay;

class LuaLoader;

class MapMaker{
public:
	static std::vector<int> numOfBlock;
	static std::vector<std::vector<std::string>> blockRegistry;

	static std::vector<std::vector<std::string>> arrangeBlocks(LuaLoader* loader);
	static const int TURN_PROBABILITY = 3;
	enum BlockDirections{
		BLOCK_GOING_UP,
		BLOCK_GOING_DOWN,
		BLOCK_GOING_LEFT,
		BLOCK_GOING_RIGHT,
		BLOCK_SPLIT_HORIZONTAL,
		BLOCK_SPLIT_VERTICAL
	};

	static const int NUM_OF_BLOCK_TYPES = 31;
	enum TypeMap{
		SPACE,
		POND,
		NORMAL,
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		TOP_BOTTOM,
		LEFT_RIGHT,
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_LEFT_CORNER_BOTTOM_RIGHT,
		TOP_RIGHT_CORNER_BOTTOM_LEFT,
		BOTTOM_LEFT_CORNER_TOP_RIGHT,
		BOTTOM_RIGHT_CORNER_TOP_LEFT,
		TOP_BOTTOM_BRIDGE,
		LEFT_RIGHT_BRIDGE,
		CORNER_TOP_LEFT,
		CORNER_TOP_RIGHT,
		CORNER_BOTTOM_LEFT,
		CORNER_BOTTOM_RIGHT,
		CORNER_TOP_BOTH,
		CORNER_BOTTOM_BOTH,
		CORNER_BOTH_LEFT,
		CORNER_BOTH_RIGHT,
		BOTTOM_CORNER_TOP_BOTH,
		TOP_CORNER_BOTTOM_BOTH,
		LEFT_CORNER_BOTH_RIGHT,
		RIGHT_CORNER_BOTH_LEFT
	};

	std::vector<MapMaker*> child;
	
	bool isSplit;
	bool turnLock;
	bool isFirst;
	bool collided;
	bool modified;
	bool isTurn;
	//	int directionGoing;
	int worldBlockLength;
	std::vector<std::vector<int>> blockMapInt;
	Vector2 currentPosition;
	~MapMaker();  // the one after
	
	void traceForBridges(std::vector<Vector2> &positions, std::vector<std::vector<int>> &blockMapInt);  // NEXT STEP //////
	bool isOutBounds(int worldBlockLength, Vector2 position);
	Vector2 nextVec(Vector2 currentVector, int directionGoing);
	bool directionAllowed(int direction, std::vector<int> directionNoGo);
	void numOfSplits(int &counter);
	void numOfCollisions(int &counter);
	bool isCollided(int direction);
	static void numOfTypeOfBlock(int &counter, int blockType, std::vector<std::vector<int>> blockMapInt);
	static void addToBlockRegistry(LuaLoader* loader, std::string category, std::string numOfConstant, int index);
	int getOpposite(int direction);
	MapMaker(bool isFirst, bool turnLock, int directionGoing, Vector2 newPosition, std::vector<std::vector<int>> &blockMapInt, int worldBlockLength, std::vector<int> directionNoGo);
};