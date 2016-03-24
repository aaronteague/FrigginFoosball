#pragma once

#include "gameplay.h"
#include "../Item.h"

using namespace gameplay;

class Tool : public Item{
	friend class Item; /**@ref*/
public:
	/** saves information in the form of a lua Table*/
	virtual luabridge::LuaRef save();

	/** gets information from lua table*/
	virtual void load(luabridge::LuaRef table);

	/** updates the logic of the house,  mostly gonna be animation, not much else to see here  only call if visiible*/
	virtual void Update(const float& elapsedTime);

	/** draws the house.  only call if in visible range*/
	virtual void Render();

	/** gets the assets info and puts it into the lists*/
	virtual void prepSelf();

	/** adds self to game _scene*/
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);
private:
	
	int profit;
	int cost;

	std::string normalFile;
	int counter;

	Tool(LuaLoader* loader, std::string luaFile, Vector2 position, Rotation rotation, Status status, b2World* containedWorld);
};