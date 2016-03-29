#include "CameraStage.h"
#include "../MyCamera.h"
#include "../FSM.h"
#include "../MyCurve.h"

#include "CameraFollow.h"


CameraStage::CameraStage(){}

void CameraStage::Enter(Entity* camera, FSM* fsm, float& elapsedTime)
{
	self = (MyCamera*)camera;

	fsm->proceed();
}
void CameraStage::Execute(Entity* camera, FSM* fsm, float& elapsedTime)
{
	if (curve){
		Vector3 pos;
		curve->evaluate(elapsedTime, pos.x, pos.y, pos.z);
		self->node->setTranslation(pos);
		if (curve->isComplete())
			SAFE_DELETE(curve);
	}
	else if (self->followNode != NULL)
		self->node->setTranslation(self->followNode->getTranslation() - self->followOffset);
}
void CameraStage::Exit(Entity* camera, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
void CameraStage::OnMessage(Entity* camera, Telegram& telegram, FSM* fsm)
{
	if (!self)
		self = (MyCamera*)camera;
	self->panSpeed = self->panRadius = 0;

	switch (telegram.msg){
	case FSM::START_ROUND:
		fsm->SetCurrentState(new CameraFollow((Table*)telegram.extraInfo));
		break;
	case FSM::ZOOM_INTO_TARGET:
		curve = new MyCurve(Curve::LINEAR);
		Vector3 camPos(self->node->getTranslationWorld());
	//	Vector3 targetPos(self->target->getNode()->getTranslationWorld());
		Vector3 targetPos(self->target->getLookPosition());
		Vector3 dif = camPos - targetPos;
		Vector3 finalPos = camPos - (dif / 2);
		curve->addPoint(0, camPos.x, camPos.y, camPos.z);
		curve->addPoint(*(int*)telegram.extraInfo, finalPos.x, finalPos.y, finalPos.z);
		break;
	}
}