#include "CameraOvermap.h"
#include "../MyCamera.h"
#include "../FSM.h"

CameraOvermap::CameraOvermap()
{
	currentTouch = lastTouch = Vector2(-1, -1);
	driftVelocity = Vector2::zero();
}

void CameraOvermap::Enter(Entity* camera, FSM* fsm, float& elapsedTime)
{
	self = (MyCamera*)camera;

	
	self->node->setRotation(Matrix::identity());
	self->node->setTranslation(0, 5, CAM_HEIGHT);

	fsm->proceed();
}
void CameraOvermap::Execute(Entity* camera, FSM* fsm, float& elapsedTime)
{
	if (camTarget.length() > 0){ // run to target
	//	Vector3 finalTrans(camTarget.x, camTarget.y, self->DEFAULT_HEIGHT);
		self->GetNode()->translateSmooth(camTarget, elapsedTime, RESPONSE_TIME);
		if ((camTarget - self->node->getTranslationWorld()).length() < 0.1){
			camTarget.set(Vector3::zero());
			driftVelocity = Vector2::zero();
		}
	}
	else if (currentTouch.x != -1){
		/*	if (lastTouch.x < 0)
		lastTouch = currentTouch;*/

		Vector2 dif = lastTouch - currentTouch;
		if (lastTouch.x != -1)
			self->node->translate(Vector3(dif.x, dif.y, 0));

		lastTouch = currentTouch;
	}
	else{// apply drift velocity
		self->node->translate(Vector3(driftVelocity.x, driftVelocity.y, 0));
		driftVelocity *= DRIFT_MULTIPLIER;
		if (driftVelocity.length() < 0.001)
			driftVelocity = Vector2::zero();
	}

	keepCamInBoundary();
}

void CameraOvermap::keepCamInBoundary()
{
	Vector3 pos = self->node->getTranslation();
	if (pos.x < 0)
		pos.x = 0;
	if (pos.x > 16)
		pos.x = 16;
	if (pos.y < 0)
		pos.y = 0;
	if (pos.y > 16)
		pos.y = 16;
	self->node->setTranslation(pos);
}

void CameraOvermap::Exit(Entity* camera, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
void CameraOvermap::OnMessage(Entity* camera, Telegram& telegram, FSM* fsm)
{
	if (!self)
		self = (MyCamera*)camera;

	switch (telegram.msg){
	case FSM::TOUCH_DOWN:
		currentTouch = self->screenToFieldCoordinates(*(Vector2*)telegram.extraInfo);
		driftVelocity.set(0, 0);
		touchDownTime = Game::getGameTime();
		break;
	case FSM::TOUCH_RELEASE:
		driftVelocity = lastTouch - currentTouch;
		currentTouch.set(-1, -1);
		lastTouch.set(-1, -1);
		
	//	if (Game::getGameTime() - touchDownTime < 500 && driftVelocity.length() < 0.3f)
		break;
	case FSM::SET_TARGET:
		camTarget = *((Vector3*)telegram.extraInfo);
		camTarget.z = CAM_HEIGHT;
		camTarget.x -= 1.0f;
		break;
	}
}