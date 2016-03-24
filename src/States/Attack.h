#pragma once
#include "../State.h"

class Opponent;
class Table;

class Attack : public State
{
public:

	Attack(Table* table);
	void Enter(Entity* opponent, FSM* fsm, float& elapsedTime);
	void Execute(Entity* opponent, FSM* fsm, float& elapsedTime);
	void Exit(Entity* opponent, FSM* fsm, float& elapsedTime);
	void OnMessage(Entity* opponent, Telegram& telegram, FSM* fsm);
private:
	const float READY_DISTANCE = 0.6f;
	const float STRIKE_DISTANCE = 0.3f;
	const float LINEAR_FORCE = 20;
	const float BACK_ANGULAR_FORCE = 0.1f;
	const float FORWARD_ANGULAR_FORCE = 0.001f;
	const float DESIRED_READY_ANGLE = 20;

	Opponent* self;
	Table* table;
};