#pragma once

#include "../State.h"

#include "Gameplay.h"

class Item;

using namespace gameplay;

class CharacterInteractWithItem : public State
{
public:
	CharacterInteractWithItem();

	virtual void Enter(Entity* character, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* character, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* character, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* character, Telegram& telegram, FSM* fsm);
private:
	Item* item;
	bool backup;
};