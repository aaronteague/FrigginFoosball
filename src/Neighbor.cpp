#include "Neighbor.h"
#include "FSM.h"
#include "Menus\Inventory.h"
#include "Menus\Dialogue.h"
// states
 #include "States/NeighborIdle.h"


Neighbor::Neighbor(LuaLoader *loader, std::string luaFile, int posX, int posY, Vector2 screenSize)
:Character(loader, luaFile, posX, posY)
{
	// add this next line in when you add visibility to inventory
		// inventory = new Inventory(screenSize, this, false)
	inventory = new Inventory(screenSize, this);

	std::string dialogueFile = loader->getString("CHARACTER_DIALOGUE");
	dialogue = new Dialogue(screenSize, dialogueFile);
	stateMachine->SetCurrentState(new NeighborIdle());
}

//luabridge::LuaRef Neighbor::save()
//{
//	luabridge::LuaRef characterRef = luabridge::newTable(L);
//
//	characterRef["luaFile"] = luaFile;
//
//	return characterRef;
//}
//void Neighbor::load(luabridge::LuaRef characterRef)
//{

//}