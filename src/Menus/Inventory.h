#pragma once
#include "../Menu.h"

class Item;
struct ItemIcon;
class MyCurve;
class PlayField;
class Character;

using namespace gameplay;



class Inventory : public Menu{
public:
	// add visibility later.  We want to be able to have neighbors use this class
	// but not have to load up spritebatches and shit whenever we add an item 
	// because, well, it won't even be visible.
	Inventory(const Vector2 &screenSize, Character* belongsTo);

	void holdItem(Item* item);
	bool addItem(std::string itemFile, LuaLoader* loader, int indexToPlaceInto = -1);
	bool addItem(ItemIcon* icon, int indexToPlaceInto = -1);
	
	virtual bool touchPress(Vector2 position);
	virtual void touchMove(Vector2 position);
	virtual void touchRelease(Vector2 position);

	virtual void Update(const float& elapsedTime);
	virtual void Render();

	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	luabridge::LuaRef save();
	void load(luabridge::LuaRef inventoryRef, LuaLoader* loader);

private:
	const int NUM_OF_ITEM_SLOTS = 12;
	const int TAP_LEEWAY = 50;
	const int TAP_DURATION = 250;
	const float SLIDE_TIME = 1000;
	const int INVENTORY_COLUMNS = 4;
	const int INVENTORY_ROWS = 3;
	 float INVENTORY_SLOT_WIDTH = 51;
	 float INVENTORY_SLOT_HEIGHT = 48;
	int INVENTORY_OFFSET_X;
	int INVENTORY_OFFSET_Y;

	float MENU_FIX;

	MyCurve *_curve;
	Character* belongsTo;
	std::vector<SpriteBatch*> itemBatch;
	std::vector<std::string> textureArray;
	std::vector<std::vector<ItemIcon*>> renderList;
	std::vector<ItemIcon*> itemArray;
	ItemIcon* iconTouched;
	Vector2 dockPosition;
	Vector2 outPosition;
	float dockToOutLength;
	float touchTimer;
	Vector2 buttonOffset;
	Vector2 touchOffset;
	gameplay::Rectangle buttonRec;
	gameplay::Rectangle buttonImgRecPause;
	gameplay::Rectangle buttonImgRecBack;

	unsigned int buildCounter;

	void setIconDimensions(ItemIcon *icon, int index);
	void setIconPositions();
	int findClosestMatch(ItemIcon* icon);
	void slideTo(const Vector2 &destination, const float &time);
	void removeItem(ItemIcon* icon);
	void addToRenderList(ItemIcon* icon);
};