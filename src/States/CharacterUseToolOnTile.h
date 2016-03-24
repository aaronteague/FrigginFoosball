#pragma once

#include "../State.h"

#include "Gameplay.h"

class Item;
struct Tile;

using namespace gameplay;

class CharacterUseToolOnTile : public State
{
public:
	CharacterUseToolOnTile();

	virtual void Enter(Entity* character, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* character, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* character, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* character, Telegram& telegram, FSM* fsm);
private:
	Tile* targetTile;
};