#pragma once

#include "gameplay.h"

#include "../State.h"

using namespace gameplay;

class Table;

class TableIdle : public State
{
public:
	TableIdle();
	void Enter(Entity* table, FSM* fsm, float& elapsedTime);
	void Execute(Entity* table, FSM* fsm, float& elapsedTime);
	void Exit(Entity* table, FSM* fsm, float& elapsedTime);
	void OnMessage(Entity* table, Telegram& telegram, FSM* fsm);
private:
	Table* self;
	bool floating;
	const float FLOAT_TIMER = 1500;
	int timer = 0;
	int powerupTimer = -1;
	
	Node* powerUpItem = NULL;
	PhysicsCollisionObject* removableObject = NULL;
	bool addPhysics = false;
	int magnetTarget = -1;
	Node* emitNode;
	void notifyPowerReceived(std::string message, int owner);
};