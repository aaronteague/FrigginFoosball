#include "Tile.h"
#include "Item.h"

Tile::Tile()
	:childItem(NULL)
{
	luaFile = "none";
	mat = "none";
	fertility = 0;
	water = 0;
}

void Tile::Render()
{
	if (childItem != NULL)
		childItem->Render();
}