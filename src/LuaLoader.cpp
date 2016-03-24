#include "LuaLoader.h"


LuaLoader::LuaLoader(lua_State *L)
:L(L), masterTable(NULL), currentTableFile(""), subTable(NULL)
{
	masterTable = new luabridge::LuaRef(L);
	*masterTable = luabridge::newTable(L);

	subTable = new luabridge::LuaRef(L);
	*subTable = luabridge::newTable(L);
}

LuaLoader::~LuaLoader()
{
	// hopefully this will leak lua memory and call lua's garbage collector
	//	*masterTable = luabridge::newTable(L);
}

void LuaLoader::setFile(const std::string &fileToSet)
{

	currentTableFile = fileToSet;
	openTable(fileToSet);

}

void LuaLoader::openFile(const std::string &luaFile){

	if (lastFileOpened.compare(luaFile) != 0){
		char* file = FileSystem::readAll(luaFile.c_str());
		int status = luaL_dostring(L, file);
		if (status != 0)
			GP_ERROR(("something is wrong with file " + luaFile).c_str());
		lastFileOpened = luaFile;
	}
}

void LuaLoader::openTable(const std::string &luaFile){
	luabridge::LuaRef masterTableRef = *masterTable;

	*subTable = masterTableRef[luaFile];

	if (subTable->isNil()){
		*subTable = luabridge::newTable(L);
		masterTableRef[luaFile] = *subTable;
	}
}

int LuaLoader::getInt(const std::string &key)
{


	luabridge::LuaRef subTableRef = *subTable;

	luabridge::LuaRef intRef = subTableRef[key];
	if (!intRef.isNil()){
		if (!intRef.isNumber())
			GP_ERROR((key + "is not an int type").c_str());

		const int theInt = intRef;
		return theInt;
	}
	else{
		openFile(currentTableFile);

		luabridge::LuaRef ref = luabridge::getGlobal(L, key.c_str());

		//if (ref.isNil())
		//	GP_ERROR((key + " is not a variable in " + lastFileOpened).c_str());
		//if (!ref.isNumber())
		//	GP_ERROR((key + " is not an int type").c_str());

		const int theInt = ref;
		subTableRef[key] = ref;

		return theInt;
	}
}

int LuaLoader::getInt(const std::string &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	luabridge::LuaRef ref = table[key];

	//if (ref.isNil())
	//	GP_ERROR(("there isn't a " + key + " in this table").c_str());
	//if (!ref.isNumber())
	//	GP_ERROR((key + "is not an int").c_str());

	const int num = ref;
	return num;
}
int LuaLoader::getInt(const int &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	luabridge::LuaRef ref = table[key];

	if (ref.isNil())
		GP_ERROR("there isn't this key in this table");
	if (!ref.isNumber())
		GP_ERROR("is not an int");

	const int num = ref;
	return num;
}

float LuaLoader::getFloat(const std::string &key)
{
	luabridge::LuaRef subTableRef = *subTable;

	luabridge::LuaRef floatRef = subTableRef[key];
	if (!floatRef.isNil()){
		if (!floatRef.isNumber())
			GP_ERROR((key + "is not a float type").c_str());

		const float theFloat = floatRef;
		return theFloat;
	}
	else{
		openFile(currentTableFile);

		luabridge::LuaRef ref = luabridge::getGlobal(L, key.c_str());

		if (ref.isNil())
			GP_ERROR((key + " is not a variable in " + lastFileOpened).c_str());
		if (!ref.isNumber())
			GP_ERROR((key + " is not a float type").c_str());

		const float theFloat = ref;
		subTableRef[key] = ref;

		return theFloat;
	}
}

float LuaLoader::getFloat(const std::string &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	luabridge::LuaRef ref = table[key];

	//if (ref.isNil())
	//	GP_ERROR(("there isn't a " + key + " in this table").c_str());
	//if (!ref.isNumber())
	//	GP_ERROR((key + "is not a float").c_str());

	const float num = ref;
	return num;
}
float LuaLoader::getFloat(const int &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	luabridge::LuaRef ref = table[key];

	//if (ref.isNil())
	//	GP_ERROR(("there isn't a " + std::to_string(key) + " in this table").c_str());
	//if (!ref.isNumber())
	//	GP_ERROR((std::to_string(key) + "is not a float").c_str());

	const float num = ref;
	return num;
}

bool LuaLoader::getBool(const std::string &key)
{
	luabridge::LuaRef subTableRef = *subTable;

	luabridge::LuaRef boolRef = subTableRef[key];
	if (!boolRef.isNil()){
		if (!boolRef.isNumber())
			GP_ERROR((key + "is not a bool type").c_str());

		const bool theBool = boolRef;
		return theBool;
	}
	else{
		openFile(currentTableFile);

		luabridge::LuaRef ref = luabridge::getGlobal(L, key.c_str());

		if (ref.isNil())
			GP_ERROR((key + " is not a variable in " + lastFileOpened).c_str());
		if (!ref.isNumber())
			GP_ERROR((key + " is not a bool type").c_str());

		const int theInt = ref;
		bool theBool = theInt;

		subTableRef[key] = ref;

		return theBool;
	}
}

bool LuaLoader::getBool(const std::string &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	luabridge::LuaRef ref = table[key];

	//if (ref.isNil())
	//	GP_ERROR(("there isn't a " + key + " in this table").c_str());
	//if (!ref.isNumber())
	//	GP_ERROR((key + "is not a bool").c_str());

	const int integer = ref;
	const bool boolean = integer;

	return boolean;
}
bool LuaLoader::getBool(const int &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	luabridge::LuaRef ref = table[key];

	//if (ref.isNil())
	//	GP_ERROR(("there isn't a " + std::to_string(key) + " in this table").c_str());
	//if (!ref.isNumber())
	//	GP_ERROR((std::to_string(key) + "is not a bool").c_str());

	const int integer = ref;
	const bool boolean = integer;

	return boolean;
}

std::string LuaLoader::test(std::string value)
{
	int status = luaL_dofile(L, currentTableFile.c_str());
	luabridge::LuaRef faggot(L, "banana");
	luabridge::setGlobal(L, faggot, "faggot");
	//	luabridge::getGlobal(L, "faggot");
	lua_getglobal(L, "faggot");
	//	luabridge::LuaRef stringRef = ref;
	//	if (!stringRef.isNil()){
	//		if (!stringRef.isString())
	//			GP_ERROR((key + "is not a string type").c_str());
	const char* word;
	word = lua_tostring(L, -1);
	//	std::string actualString(lua_tostring(L, -1));
	//	delete[] word;
	std::string banana = word;

	return banana;
	//}
	//else{
	//	openFile(currentTableFile);

	//	luabridge::LuaRef ref = luabridge::getGlobal(L, key.c_str());

	//	if (ref.isNil())
	//		GP_ERROR((key + " is not a variable in " + lastFileOpened).c_str());
	//	if (!ref.isString())
	//		GP_ERROR((key + " is not a string type").c_str());

	//	const std::string theString = ref;
	//	subTableRef[key] = ref;

	//	return theString;
	//}
}

std::string LuaLoader::getString(const std::string &key)
{
	luabridge::LuaRef subTableRef = *subTable;

	luabridge::LuaRef stringRef = subTableRef[key];
	if (!stringRef.isNil()){
		if (!stringRef.isString())
			GP_ERROR((key + "is not a string type").c_str());

		const std::string theString = stringRef;
		return theString;
	}
	else{
		openFile(currentTableFile);

		luabridge::LuaRef ref = luabridge::getGlobal(L, key.c_str());

		if (ref.isNil())
			GP_ERROR((key + " is not a variable in " + lastFileOpened).c_str());
		if (!ref.isString())
			GP_ERROR((key + " is not a string type").c_str());

		const std::string theString = ref;
		subTableRef[key] = ref;

		return theString;
	}
}

std::string LuaLoader::getString(const std::string &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	const luabridge::LuaRef stringRef = table[key];

	//if (stringRef.isNil())
	//	GP_ERROR(("there isn't a " + key + " in this table").c_str());
	//if (!stringRef.isString())
	//	GP_ERROR((key + "is not a string").c_str());

	const std::string theString = stringRef;
	return theString;
}
std::string LuaLoader::getString(const int &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	const luabridge::LuaRef stringRef = table[key];

	if (stringRef.isNil())
		return "";
	//	GP_ERROR(("there isn't a " + std::to_string(key) + " in this table").c_str());
	//if (!stringRef.isString())
	//	GP_ERROR((std::to_string(key) + "is not a string").c_str());

	const std::string theString = stringRef;
	return theString;
}

luabridge::LuaRef LuaLoader::getTable(const std::string &key)
{
	luabridge::LuaRef subTableRef = *subTable;

	luabridge::LuaRef tableRef = subTableRef[key];
	if (!tableRef.isNil()){
		if (!tableRef.isTable())
			GP_ERROR((key + "is not a bool type").c_str());

		return tableRef;
	}
	else{
		openFile(currentTableFile);

		luabridge::LuaRef ref = luabridge::getGlobal(L, key.c_str());

		if (ref.isNil())
			GP_ERROR((key + " is not a variable in " + lastFileOpened).c_str());
		if (!ref.isTable())
			GP_ERROR((key + " is not a table type").c_str());

		subTableRef[key] = ref;

		return ref;
	}
}

luabridge::LuaRef LuaLoader::getTable(const std::string &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	const luabridge::LuaRef anotherTable = table[key];

	//if (anotherTable.isNil())
	//	GP_ERROR(("there isn't a " + key + " in this table").c_str());
	//if (!anotherTable.isTable())
	//	GP_ERROR((key + "is not a table").c_str());

	return anotherTable;
}
luabridge::LuaRef LuaLoader::getTable(const int &key, const luabridge::LuaRef &table)
{
	if (table.isNil())
		GP_ERROR("not a valid table");

	const luabridge::LuaRef anotherTable = table[key];

	//if (anotherTable.isNil())
	//	GP_ERROR(("there isn't a " + std::to_string(key) + " in this table").c_str());
	//if (!anotherTable.isTable())
	//	GP_ERROR((std::to_string(key) + "is not a table").c_str());

	return anotherTable;
}

