#pragma once
#include "../State.h"

#include "Gameplay.h"

using namespace gameplay;

class Dialogue;
class Character;
class Neighbor;

class NeighborTalkToPlayer : public State
{
public:
	NeighborTalkToPlayer();

	virtual void Enter(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* neighbor, Telegram& telegram, FSM* fsm);
private:
	static bool luaBinded;

	Dialogue* dialogue;
	Character* targetCharacter;
	Neighbor* self;

	std::string text;
	std::vector<std::string> answers;
	float fontSize;
	float textSpeed;
	bool finished;

	std::string getPlayerName(){
		return std::string("billy");
	}

	std::string getNeighborName(){
		return std::string("Bobby");
	}

	std::string getItemName(){
		return std::string("Button-up shirt");
	}
	void setEmotion(int emotion, int setToLetter);
	void say(float fontSize, float textSpeed, bool question, std::string dialogue);
	void popUpQuestion(std::string one, std::string two, std::string three, std::string four, std::string five, std::string six, std::string seven);
	void finish();
	

};