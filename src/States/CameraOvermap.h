#pragma once

#include "gameplay.h"
#include "../State.h"

using namespace gameplay;

class MyCamera;
class Entity;

class CameraOvermap : public State
{
public:
	CameraOvermap();

	void Enter(Entity* camera, FSM* fsm, float& elapsedTime);
	void Execute(Entity* camera, FSM* fsm, float& elapsedTime);
	void Exit(Entity* camera, FSM* fsm, float& elapsedTime);
	void OnMessage(Entity* camera, Telegram& telegram, FSM* fsm);

private:

	Vector2 lastTouch;
	Vector2 currentTouch;
	Vector2 driftVelocity;
	float DRIFT_MULTIPLIER = 0.9f;
	float touchDownTime;

	Vector3 camTarget;

	MyCamera* self = NULL;

	const float CAM_HEIGHT = 5;
	const float RESPONSE_TIME = 500.0f;

	void keepCamInBoundary();
};