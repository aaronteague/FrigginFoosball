#pragma once

#include "gameplay.h"

class FSM;
class Entity;
struct Telegram;

class State
{
public:
	virtual ~State(){}
	// this will execute when the state is entered
	virtual void Enter(Entity*, FSM*, float& elapsedTime) = 0;
	// this is called by the miner's update function each update step
	virtual void Execute(Entity*, FSM*, float& elapsedTime) = 0;
	// this will execute when the state is exited
	virtual void Exit(Entity*, FSM*, float& elapsedTime) = 0;
	// this executes if the agent receivers a message
	// from message dispatcher
	virtual void OnMessage(Entity*, Telegram&, FSM*) = 0;
};