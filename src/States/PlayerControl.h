#pragma once
#include "../State.h"
//#include "Tile.h"

class PlayerControl : public State
{
private:
	bool leftPressed, rightPressed, upPressed, downPressed;
	
public:
	
	PlayerControl();
	

	virtual void Enter(Entity* player, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* player, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* player, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* player, Telegram& telegram, FSM* fsm);
};