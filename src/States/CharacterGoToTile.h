#pragma once
#include "../State.h"

#include "gameplay.h"

using namespace gameplay;

struct Tile;

class CharacterGoToTile : public State
{
private:
	std::vector<Vector2> path;
	Tile* targetTile;
	Vector2 currentVelocity;
	Vector2 lastPos;
	const float maxTimeForState = 20000; // 20 seconds
	float stateTimer;
public:
	virtual void Enter(Entity* character, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* character, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* character, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* character, Telegram& telegram, FSM* fsm);
};