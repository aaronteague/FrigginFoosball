#pragma once

#include <string>
#include "Gameplay.h"
#include "Dependencies\LuaBridge\LuaBridge.h"

using namespace gameplay;

class LuaLoader{
public:
	/**  Constructor
	@param L the luastate to pass to */
	LuaLoader(lua_State *L);

	/** dustructor*/
	~LuaLoader();

	/** sets the file to load info out of*/
	void setFile(const std::string &fileToSet);

	/** gets an int from a key*/
	int getInt(const std::string &key);
	/** gets an int from key from within a table*/
	int getInt(const std::string &key, const luabridge::LuaRef &table);
	/** gets an int from key from within a table*/
	int getInt(const int &key, const luabridge::LuaRef &table);


	/** gets a float from a key*/
	float getFloat(const std::string &key);
	/** gets a float from a key from within a table*/
	float getFloat(const std::string &key, const luabridge::LuaRef &table);
	/** gets a float from a key from within a table*/
	float getFloat(const int &key, const luabridge::LuaRef &table);


	/** gets a bool from a key*/
	bool getBool(const std::string &key);
	/** gets a bool from a key from within a table*/
	bool getBool(const std::string &key, const luabridge::LuaRef &table);
	/** gets a bool from a key from within a table*/
	bool getBool(const int &key, const luabridge::LuaRef &table);


	/** gets a string from a key*/
	std::string getString(const std::string &key);
	/** gets a string from a key from within a table*/
	std::string getString(const std::string &key, const luabridge::LuaRef &table);
	/** gets a string from a key from within a table*/
	std::string getString(const int &key, const luabridge::LuaRef &table);


	/**gets a table from a key*/
	luabridge::LuaRef getTable(const std::string &key);
	/** gets a table from a key within a table*/
	luabridge::LuaRef getTable(const std::string &key, const luabridge::LuaRef &table);
	/** gets a table from a key from within a table*/
	luabridge::LuaRef getTable(const int &key, const luabridge::LuaRef &table);

	std::string test(std::string value);

private:
	lua_State *L;
	luabridge::LuaRef *masterTable;
	luabridge::LuaRef *subTable;

	std::string currentTableFile;
	std::string lastFileOpened;
	void openFile(const std::string &luaFile);
	void openTable(const std::string &luaFile);
};