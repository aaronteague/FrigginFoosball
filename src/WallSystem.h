#pragma once

#include "gameplay.h"
#include "Entity.h"

using namespace gameplay;

struct Tile;
class Item;
class LuaLoader;


struct WallSegment{
	bool ghost;
	std::string file;
	std::string doorFile;
	std::string paperFile;
	int side;
	SpriteBatch* _batch;
	int index;
	Vector2 pos;
	std::vector<Vector3> trans;
	Node* door;
	Node* wallAroundDoor;
	std::string doorID;
	std::string doorTex;
	std::string wallAroundDoorID;
	int length;
	std::vector<Vector4> barriers;
	WallSegment(std::string file, int side, Vector2 pos, LuaLoader *loader);
	Vector3 topTrans;

	// the extra bit to close off area
	bool hasExtra;
	std::vector<Vector3> extraTrans;
	Vector3 extraTopTrans;
};

struct CloseOff{
	Vector3 trans;
	int side;
};

class WallSystem : public Entity{
public:
	/** which side of a tile the wallSegment faces*/
	enum WallSide{
		NORTH,
		WEST,
		EAST,
		SOUTH
	};
	/** constructor*/
	WallSystem(std::vector<std::vector<Tile*>> &tileMap, LuaLoader *loader);

	/** calculates and returns the barriers*/
	std::vector<Vector4> getBarriers();

	/** handles a press*/
	bool touchPress(Item* wallpaper);
	
	/** handles a move of finger*/
	void touchMove();

	/** handles the release of a touch*/
	void touchRelease();

	/** updates logic of wallSystem
	@param trans the position to base visibility on*/
	void Update(const Vector3& trans);

	/** draws the wallSystem based on what "Update" has organized for us*/
	void Render(const Matrix& viewProjMatrix);

	/** destructor*/
	~WallSystem();
private:
	std::vector<std::string> texFiles;
	std::vector<std::string> texFilesToBuild;
	std::vector<std::vector<std::vector<WallSegment*>>> wallSegments;
	std::vector<WallSegment*> doors;
	std::vector<std::vector<WallSegment*>> sortedWallSegments;
	std::vector<SpriteBatch*> spriteRenderList;
	std::vector<std::vector<WallSegment*>> renderList;
	std::vector<WallSegment*> doorsToDraw;
	std::vector<SpriteBatch*> batchList;
	std::vector<std::vector<WallSegment*>> buildList;
	
	std::vector<CloseOff> closeOffs;
	unsigned int buildCounter;

	SpriteBatch* wallTopBatch;

	std::vector<float> rotation;
	const int RANGE = 30;

	std::vector<Vector4> barriers;

	bool segmentBeenUsed(WallSegment* segment, std::vector<WallSegment*> &existingSegments);
	void changeWallpaper(std::string, WallSide side);
	void calculateBarriers();
	void findAndBuildExtras();
	bool hasPerpendicularWall(Vector2 pos, Vector2 direction);

	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	virtual void Update(const float& elapsedTime){} // not used
	virtual void Render(){} // not used
};