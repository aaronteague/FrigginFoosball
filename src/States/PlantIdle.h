#pragma once

#include "../State.h"


#include "Gameplay.h"

class Item;
class Entity;

using namespace gameplay;

class PlantIdle : public State
{
public:
	PlantIdle();

	virtual void Enter(Entity* plant, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* plant, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* plant, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* plant, Telegram& telegram, FSM* fsm);
private:
	Entity* interactee;
	std::vector<Item*> cropPackage;
	int animInfo;
};