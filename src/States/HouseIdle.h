#pragma once
#include "../State.h"

#include "gameplay.h"

using namespace gameplay;

class Character;

class HouseIdle : public State
{
public:
	HouseIdle();

	virtual void Enter(Entity* house, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* house, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* house, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* house, Telegram& telegram, FSM* fsm);

private:
	Character* interactee;
};