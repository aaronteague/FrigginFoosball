#pragma once
#include "gameplay.h"

using namespace gameplay;

class Item;

struct Tile{
	enum TileTypeList{
		EMPTY,
		RESERVED,
		UNAVAILABLE,
		HOUSE_POINT,
		ITEM,
		OCCUPIED,
		TILLED
	};

	int fertility;
	Vector2 ownedBy;
	std::string mat;
	bool hole;
	Item* childItem;
	int type;
	Vector2 position;
	std::string luaFile;
	int water;

	std::string northWallpaper;
	std::string westWallpaper;
	std::string eastWallpaper;
	std::string southWallpaper;

	Tile();
	void Render();
};