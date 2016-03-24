#include "Announcer.h"
#include "Dependencies\LuaBridge\LuaBridge.h"

Announcer::Announcer()
{
	luabridge::getGlobalNamespace(L)
		.beginNamespace("announcer")
		.beginClass<Announcer>("Announcer")
		.addFunction("LoadFile", &Announcer::loadFile)
		.addFunction("Say", &Announcer::say)
		.endClass()
		.endNamespace();

	audioFiles.resize(10);
	
}
void Announcer::loadFile(int index, std::string file)
{
	if (index >= audioFiles.size() - 1)
		audioFiles.resize(index + 1);

	audioFiles[index] = AudioSource::create(file.c_str());
	
}
void Announcer::say(int index)
{
	if (currentAudio != NULL && currentAudio->getState() == AudioSource::PLAYING)
		currentAudio->stop();

	audioFiles[index]->play();
}