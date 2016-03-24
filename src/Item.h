#pragma once
#include "Entity.h"

class b2World;
class b2Body;
class Item;
class PlayField;


using namespace gameplay;

struct ItemIcon{
	Item *item;
	gameplay::Rectangle rec;
	gameplay::Rectangle imgRec;
	std::string texture;
	Vector2 offset;
	bool visible;
	bool preview;
	ItemIcon(LuaLoader* loader, Item* item);
};

class Item : public Entity{
	friend struct ItemIcon; /**<@ref*/
public:
	/** the orientation of the item*/
	enum Rotation{
		SOUTH,
		EAST,
		NORTH,
		WEST
	};

	/** the status of the item*/
	enum Status{
		ABSTRACT, /**< not quite an object in the playfield, more of something stored in an icon*/
		PREVIEW, /**< not quite an object with physical bounds, more of a preview of item in the playfield*/
		FREE_BOUND, /**< similar to preview, but location is based on a 3D VECTOR and not bound to tiled positions*/
		SOLID /**< an object in the playfield, placed, and with a bounding box/sphere */
	};

	enum PhysicShape{
		NONE,
		CIRCLE,
		BOX,
		CUSTOM
	};

	/** destructor*/
	~Item();

	/** the static create function.  Loads a luafile and this creator decides what type of object it would be based on said file
	@param loader the lua loader to get information out of the luaFile
	@param luaFile the lua file with the information needed
	@param position the position we would like the item to be at
	@param rotation how we would like this item to be oriented
	@param inventory lists if this should be used as in icon so we know if we need to build phyic boundaries and node into scene
	@param containedWorld the physics world in which it is contained.  put null if it is gonna be in inventory*/	
	static Item* Create(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld);

	
	/** gets the lua File of the item*/
	std::string getLuaFile(){ return luaFile; }

	/** gets the status of item.  Will be either solid, preview, free_bound, or abstract*/
	int GetStatus(){ return status; }

	void SetStatus(Status status, b2World* physWorld = NULL);

	/** gets the dimensions of this object*/
	Vector3 getDimensions(){ return currentDimensions; }


	/** gets the icon of this item*/
	ItemIcon* GetIcon(){ return icon; }

	/** sets the rotation of this item*/
	void SetRotation(Rotation rotation);

	/** gets the roation of this item*/
	Rotation GetRotation(){ return rotation; }

	/** sets the position of this item*/
	virtual bool SetPosition(float x, float y, PlayField* playField = NULL);

	/** gets the position*/
	virtual const Vector2 &GetPosition(){ return position; }

	/** ages up the item by one day and changes it based on fertility and water in tile  Not every item will age, but all items could have this called*/
	virtual void ageUp(int &fertility, const int &water, Scene* _scene)
	{
		int i = 7;
	};

	void SetAlpha(float alpha);

	/** later, make this method pure virtual so each item has its own rules*/
	virtual bool canPlace(Vector2 pos, PlayField* playField);

	/** saves changes in this item  stores and returns it in the form of a lua table*/
	virtual luabridge::LuaRef save() 
	{ 
		luabridge::LuaRef itemTable = luabridge::newTable(L);
		itemTable["luaFile"] = luaFile.c_str();
		itemTable["rotation"] = (int)rotation;
		return itemTable;
	}
	/** loads the changes in item based on a lua table received*/
	virtual void load(luabridge::LuaRef){}

	/** updates the logic of this item.  only need to run this if it is visible  otherwise, it is a waste of CPU*/
	virtual void Update(const float& elapsedTime){}

	/** draws this object, only need to call this if it is visible range*/
	virtual void Render();

	/** gets node*/
	Node* GetNode(){ return node; }

	/** gets the point that the character interacts at*/
	Vector2 getInteractPoint();

	/** gets the point to turn character to*/
	Vector2 getPointTowards();

	/** returns true character is within range to interact*/
	bool inInteractZone(Vector2 pos);

protected:
	std::string nodeF;
	std::string nodeI;
	std::string textureF;
	std::string luaFile;

	Node* node;

	Vector2 position;

	Vector2 interactOffset;
	Vector2 interactPoint;
	Vector2 pointTowardsOffset;
	Vector2 pointTowards;
	
	float interactRange;

	bool inventory;	/**< states if this item is in the inventory of a character*/

	Status status;	/**< stores the state, as in abstract, preview, or solid*/
	
	ItemIcon* icon;	/**< the icon of this item*/

	Vector3 defaultDimensions;/**< dimensions of item*/
	Vector3 currentDimensions;/**< the modified dimensions based on rotation*/
	float radius;/**< radius if it has one*/

	b2World *physWorld;/**< the physic world this item is contained in*/
	b2Body* physBody;/**< the physic body that represents this*/

	std::vector<AnimationClip*> animations;
	AnimationClip* currentAnimClip;
	
	Rotation rotation; /** orientation*/

	PhysicShape shape; /** the physics shape*/

	/** the constructor, only to be used by the create function*/
	Item(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld);

	/** builds the physical body, in the form of a circle*/
	void buildPhys(float radius);

	/** builds the physical body, in the form of a box*/
	void buildPhys(int width, int length); // for a box type bound

	/** builds a custom shape for bounding box*/
	void buildPhys(std::vector<Vector2> points);

	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	void playAnimation(AnimationClip* clip);



	/** default constructor, not to be used though*/
	Item(){}

private:
	void fixDimensions(Rotation rotation);
	
};