#pragma once

#include "gameplay.h"
#include "../Item.h"

using namespace gameplay;

class Hole : public Item{
public:
	/** this item actually uses a constructor, but no files!*/
	Hole(Vector2 position, b2World* containedWorld);

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
};