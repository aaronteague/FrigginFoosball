#include "Screen.h"
#include "Dependencies\LuaBridge\LuaBridge.h"

bool Screen::luaBinded = false;

Screen::Screen()
{
	if (!luaBinded){
		luabridge::getGlobalNamespace(L)
			.beginClass<Screen>("Screen")
			.addFunction("loadSound", &Screen::loadSound)
			.addFunction("playSound", &Screen::playSound)
			.endClass();
	}
	luaBinded = true;
}

void Screen::loadSound(int index, std::string file)
{
	if (index >= sounds.size())
		sounds.resize(index + 1);

	if (sounds[index])
		sounds[index]->release();

	sounds[index] = AudioSource::create(file.c_str());
}
void Screen::playSound(int index)
{
	GL_ASSERT(sounds[index]);

	sounds[index]->play();
}