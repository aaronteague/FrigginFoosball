#pragma once
#include "../State.h"

#include "Gameplay.h"

using namespace gameplay;

class NeighborWait : public State
{
public:
	NeighborWait();

	virtual void Enter(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* neighbor, Telegram& telegram, FSM* fsm);
private:
	const float WAIT_TIME = 10000;  // wait 10 seconds
	float timer;
};