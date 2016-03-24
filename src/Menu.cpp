#include "Menu.h"

Menu::Menu(const Vector2 &screenSize)
{
	Menu::screenSize.set(screenSize);
	Menu::scale.set(1000.0 / (float)screenSize.x, 1000.0 / (float)screenSize.y);

}


bool Menu::NeedsInitialized()
{
	// returns if _menuBatch needs initialized
	return _menuBatch == NULL; 
}
