#pragma once

#include "gameplay.h"

using namespace gameplay;

// global, mmkay
extern lua_State *L;
extern int gameStatus;

class Screen
{
public:
	Screen();
	virtual ~Screen(){}
	virtual void Update(const float& elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) = 0;

	bool getIsFinished(){ return isFinished; }
	void loadSound(int index, std::string file);
	void playSound(int index);
protected:
	bool isFinished = false;
	std::vector<AudioSource*> sounds;
private:
	static bool luaBinded;
	
};