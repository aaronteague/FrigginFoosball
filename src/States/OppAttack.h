#pragma once
#include "../State.h"

using namespace gameplay;

class Opponent;
class Table;

class OppAttack : public State
{
public:
	OppAttack(Table* table);
	void Enter(Entity* opponent, FSM* fsm, float& elapsedTime);
	void Execute(Entity* opponent, FSM* fsm, float& elapsedTime);
	void Exit(Entity* opponent, FSM* fsm, float& elapsedTime);
	void OnMessage(Entity* opponent, Telegram& telegram, FSM* fsm);
private:
	const float READY_DISTANCE = 0.6f;
	const float READY_ANGLE = -60.0f;
	const float STRIKE_DISTANCE = 0.3f;
	const float STRIKE_DISTANCE_BACK = 0.3f;
	const float STRIKE_ANGLE = -115.0f;
	const float BACK_SPEED = -0.1f;
	const float FORWARD_SPEED = 0.2f;
	const float LINEAR_FORCE = 15;

	Opponent *self;
	Table* table;

	float poleAngle(Node* pole);
	void rotateToPos(Node* pole, float angle, float speed);
};