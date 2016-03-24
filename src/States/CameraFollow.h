#pragma once

#include "gameplay.h"
#include "../State.h"

using namespace gameplay;

class MyCamera;
class Table;
class MyCurve;

class CameraFollow : public State
{
public:
	CameraFollow(Table* table);

	void Enter(Entity* camera, FSM* fsm, float& elapsedTime);
	void Execute(Entity* camera, FSM* fsm, float& elapsedTime);
	void Exit(Entity* camera, FSM* fsm, float& elapsedTime);
	void OnMessage(Entity* camera, Telegram& telegram, FSM* fsm);

private:
	const float ZOOM_DISTANCE = 2.5f;
	const float RESPONSE_TIME = 500.0f;
	const float ZOOM_IN_TIME = 1000.0f;

	MyCamera* self;
	Table* table;
	MyCurve* curve = NULL;
	MyCurve* rotCurve = NULL;

	Vector3 goalAViewPos;
	Vector3 goalBViewPos;
};