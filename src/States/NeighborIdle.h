#pragma once

#include "../State.h"

#include "Gameplay.h"

using namespace gameplay;

class NeighborIdle : public State
{
public:
	NeighborIdle();

	virtual void Enter(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* neighbor, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* neighbor, Telegram& telegram, FSM* fsm);
private:
};