#pragma once
#include "../State.h"

#include "gameplay.h"

using namespace gameplay;

class CameraZoomIntoCharacter : public State
{
public:

	CameraZoomIntoCharacter();


	virtual void Enter(Entity* camera, FSM* fsm, float& elapsedTime);
	virtual void Execute(Entity* camera, FSM* fsm, float& elapsedTime);
	virtual void Exit(Entity* camera, FSM* fsm, float& elapsedTime);
	virtual bool OnMessage(Entity* camera, Telegram& telegram, FSM* fsm);

private:

	bool zoomedIn;
	const int RESPONSE_TIME = 200;
	float xOffset = 0;

	Vector3 entryTrans;
	const Node* target;
};