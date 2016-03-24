#pragma once
#include "../State.h"

#include "Gameplay.h"

using namespace gameplay;

class Neighbor;

class PlayerTalkToNeighbor : public State
{
public:
	PlayerTalkToNeighbor();

	virtual void Enter(Entity* player, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* player, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* player, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* player, Telegram& telegram, FSM* fsm);
private:
	Neighbor* targetNeigh;
};