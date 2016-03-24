#include "CameraZoomIntoCharacter.h"

#include "../MyCamera.h"
#include "../FSM.h"

CameraZoomIntoCharacter::CameraZoomIntoCharacter()
{
	target = NULL;
	zoomedIn = false;
}

void CameraZoomIntoCharacter::Enter(Entity* camera, FSM* fsm, float& elapsedTime)
{
	const float CAMERA_HEIGHT = 10;
	const float CAMERA_Y_OFFSET = -12;

	MyCamera* self = (MyCamera*)camera;

	if (target == NULL){
		target = self->target;
		Vector3 trans = self->node->getTranslation();
		entryTrans = trans; // so we get a copy, not a reference
	}
	
	
	Vector3 targetTrans = target->getTranslation();
	Vector3 finalTrans
		(targetTrans.x + xOffset
		, targetTrans.y + CAMERA_Y_OFFSET,
		CAMERA_HEIGHT);
	self->node->translateSmooth(finalTrans, elapsedTime, RESPONSE_TIME);

	float distance = (self->node->getTranslation() - finalTrans).length();
	if (distance < 0.1){
		zoomedIn = true;
		fsm->proceed();
	}
}
void CameraZoomIntoCharacter::Execute(Entity* camera, FSM* fsm, float& elapsedTime)
{

}
void CameraZoomIntoCharacter::Exit(Entity* camera, FSM* fsm, float& elapsedTime)
{
	MyCamera* self = (MyCamera*)camera;

	// zoom back out before going
	self->node->translateSmooth(entryTrans, elapsedTime, RESPONSE_TIME);

	float distance = (self->node->getTranslation() - entryTrans).length();
	if (distance < 0.1){
		zoomedIn = false;
		fsm->proceed();
	}
}
bool CameraZoomIntoCharacter::OnMessage(Entity* camera, Telegram& telegram, FSM* fsm)
{
	MyCamera* self = (MyCamera*)camera;

	switch (telegram.msg)
	{
	case FSM::ZOOM_OUT:
		fsm->proceed();
		if (!zoomedIn)
			fsm->proceed(); // second call if enter statement wasn't finished
		break;
	}

	return true;
}