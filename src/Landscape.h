#pragma once

#include "gameplay.h"
#include "PlayField.h"




class Landscape : public PlayField{
public:
	Landscape(int screenWidth, int screenHeight);
	void create(LuaLoader* loader);


	virtual void addCharacter(LuaLoader* loader, Character* character);
	void clear();
	
	
	virtual void Update(float &elapsedTime);
	virtual void Render();

	virtual luabridge::LuaRef save();
	virtual void load(luabridge::LuaRef &landscapeRef, LuaLoader* loader);
private:
	
	int test;
	std::vector<Vector2> housePoints;
	std::vector<std::string> houseFiles;
	void addHouseFile(LuaLoader* loader, std::string houseFile);

};