#pragma once

#include "gameplay.h"
#include "Tile.h"
#include "Dependencies\LuaBridge\LuaBridge.h"
#include <string>
#include "Block.h"
#include "Dependencies\micropather.h"


using namespace micropather;
using namespace gameplay;

class Item;
class MyCamera;
class b2World;
class Character;
//class Player;
class Menu;
class Weather;
class MatSystem;
class TimeAndDate;
class WallSystem;

class PlayField : public Graph{
public:
	/**length of time for a touch to register as a tap*/
	const int MAX_TOUCH_TIME = 500;

	// might get rid of this thingy
	enum RoomName{
		NO_ROOM = -1,
		LIVING_ROOM = 0,
		NORTH_ROOM = 1,
		WEST_ROOM = 2,
		EAST_ROOM = 3
	};

	enum TapEventType{
		FIRST_DOWN,
		FIRST_UP,
		SECOND_DOWN,
		SECOND_UP,
		INVALID
	};

	int getRoomName(){ 

		return roomName; }

	std::vector<Vector2> getPath(int startX, int startY, int endX, int endY);

	PlayField()
		:wallSystem(NULL), weather(NULL){}

	Scene* getScene(){ return _scene; }
	MyCamera* getCamera(){ return _camera; }

	void setFocusedMenu(Menu* menu);
	void addMenu(Menu* menu);
	void removeMenu(Menu* menu);

	virtual void addCharacter(Character* character);
	void SetTargetCharacter(Character* character);
	void removeCharacter(Character* character);

	const Vector2& screenToFieldCoordinates(Vector2 touchPos);
	bool isTile(int x, int y);
	bool isFreeTile(int x, int y);
	const Tile& getTile(int x, int y);
	Tile* getTilePtr(int x, int y){ return tileMap[x][y]; }
	void putItemInTile(Item* item, int x, int y);
	Item* removeItemFromTile(int x, int y);
	void lightUp(int x, int y, bool light);
	Item* GetItemFromPoint(unsigned int x, unsigned int y);

	bool digTile(Tile* tile);
	bool tillTile(Tile* tile);
	bool waterTile(Tile* tile);

	void updateLight(TimeAndDate *time);

	virtual luabridge::LuaRef save();
	virtual void load(luabridge::LuaRef &landscapeRef, LuaLoader* loader);

	virtual void Prepare();
	void Finalize(float screenWidth, float screenHeight, TimeAndDate *time);

	virtual void Update(float &elapsedTime);
	virtual void Render();

	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	void ageUp();

protected:
	struct TouchStruct{
		int touchEvent;
		// options for touching
		Character* touchedCharacter;
		Item* touchedItem;
		Tile* touchedTile;

		Vector2 touchedScreenCoords;
		double timeStamp;
		TouchStruct(double timeStamp, int touchEvent, Vector2 touchedScreenCoords)
			:touchEvent(touchEvent), touchedCharacter(NULL), touchedItem(NULL)
			, touchedTile(NULL), timeStamp(timeStamp)
		{
			TouchStruct::touchedScreenCoords.set(touchedScreenCoords);
		}
	};
	const int VIEW_RANGE = 15;

	Scene* _scene;
	MyCamera* _camera;
	b2World *physWorld;
	Vector2 screenSize;
	Vector2 pointToFollow;
	Vector2 touchDownPos;
//	TimeAndDate* timeAndDate;
//	Player* thePlayer;
	Character* targetCharacter;
	std::vector<Character*> charactersInField;
	std::vector<Menu*> menusInField;
	std::vector<TouchStruct> touchEventList;
	Menu* focusedMenu;
	Weather* weather;
	MatSystem* matSystem;
	WallSystem* wallSystem;
	int roomName;
	double accumulator; // for timestep
	int worldBlockLength;
	int blockTileLength;
	std::vector<std::vector<Tile*>> tileMap;
	std::vector<std::vector<Block*>> blockMap;
	std::vector<Vector4> barriers;
	std::vector<std::vector<std::string>> blockFileMap;
//	Vector4 viewableZone;

	void buildBarrier(Vector4 &barrier);
	Item* getTouchedItem(int x, int y);
	Character* getTouchedCharacter(int x, int y);
	Tile* getTouchedTile(int x, int y);

	void tapEventCheck(Touch::TouchEvent evt, int x, int y);
	// start back here
	bool lengthAcceptableForTap(Vector2& start, Vector2& end);
	bool tapIsAcceptable(TouchStruct& newTouch);
	void tapHoldCheck();
	// research making these a private "friend"
	float LeastCostEstimate(void* nodeStart, void* nodeEnd);
	virtual void AdjacentCost(void* node, std::vector< StateCost > *neighbors);
	void PrintStateInfo(void* node){}
};