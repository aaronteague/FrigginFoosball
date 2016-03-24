#pragma once
#include "../State.h"

#include "Gameplay.h"

using namespace gameplay;

class PlayerFollowPoint : public State
{
public:
	PlayerFollowPoint();

	virtual void Enter(Entity* player, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* player, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* player, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* player, Telegram& telegram, FSM* fsm);

private:
	Vector2 *point;	
};