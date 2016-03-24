#pragma once

#include "gameplay.h"
#include "../Item.h"

class Room;

using namespace gameplay;

class House : public Item{
	friend class Item; /**<@ref*/
	friend class HouseIdle; /**<@ref*/
public:
	/** saves information in the form of a lua Table*/
	virtual luabridge::LuaRef save();

	/** gets information from lua table*/
	virtual void load(luabridge::LuaRef table);


	/** sets the preview position  returns false because we shouldn't be moving houses around, should we :P*/
	virtual bool SetPreviewPosition(float x, float y, PlayField* playField){ return false; }

	/** updates the logic of the house,  mostly gonna be animation, not much else to see here  only call if visiible*/
	virtual void Update(const float& elapsedTime);
	
	/** draws the house.  only call if in visible range*/
	virtual void Render();

private:
	int counter;
	luabridge::LuaRef *houseRef;

	Room* room;
	std::string roomFile;
	std::vector<Vector2> barrierPoints;

	std::string texNormal;

	void openRoom();

	virtual void prepSelf();

	/** adds self to game _scene*/
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	House(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld);
};