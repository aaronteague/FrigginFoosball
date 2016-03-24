#pragma once

#include "Character.h"


using namespace gameplay;

class Dialogue;

class Neighbor : public Character{
	friend class NeighborIdle;/**< @ref*/
	friend class NeighborTalkToPlayer;/**< @ref*/
	friend class NeighborWait;/**< @ref*/
public:
	/**constructor
	@param loader the lua loader object to use to load files
	@param luaFile the lua file that houses the attributes of this object
	@param posX the position to place neighbor on x-axis
	@param posY the position to place neightobr on y-axis
	@param screenSize the size of the screen.  Needed because this object could house a menu object*/
	Neighbor(LuaLoader *loader, std::string luaFile, int posX, int posY, Vector2 screenSize);

	/** returns aspects of the character in the form of a table that could change so that it could be put into a file */
//	virtual luabridge::LuaRef save();


	/** loads from a table aspects of a character
	@param characterRef a table with changing aspects of the character*/
//	virtual void load(luabridge::LuaRef characterRef);
private:
	Dialogue* dialogue;
};