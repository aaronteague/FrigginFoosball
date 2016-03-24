#pragma once

#include "gameplay.h"
#include "PlayField.h"

using namespace gameplay;



class Room : public PlayField{
public:
	

	Room(std::string luaFile, LuaLoader* loader);
	
	Vector3 getPointFrom(int roomName);
	void prePrepare();
	virtual void Prepare();
	virtual void Update(float &elapsedTime); // oops, add const later
	virtual void Render();

	virtual luabridge::LuaRef save();
	virtual void load(luabridge::LuaRef &landscapeRef, LuaLoader* loader);
private:
	LuaLoader* loader;
	std::string luaFile;
	std::vector<Vector3> startPoints;
	std::string layoutFile;
	void createFromFile(std::string luaFile);
	void createFromTest();
};