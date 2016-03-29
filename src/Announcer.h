#pragma once

#include "gameplay.h"

extern lua_State *L;

using namespace gameplay;

class Announcer{
public:
	Announcer();
	void loadFile(int index, std::string file);
	void say(int index);

	

private:
	std::vector<AudioSource*> audioFiles;
	AudioSource* currentAudio = NULL;
	static bool luaBinded;
};