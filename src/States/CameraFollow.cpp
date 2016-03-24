#include "CameraFollow.h"
#include "../MyCamera.h"
#include "../FSM.h"
#include "../Table.h"
#include "../MyCurve.h"

#include "CameraStage.h"

CameraFollow::CameraFollow(Table* table)
:self(NULL), table(table), curve(NULL)
, goalAViewPos(4.18f, -5.78f, 5.51f)
, goalBViewPos(-4.18f, -5.78f, 5.51f)
{}

void CameraFollow::Enter(Entity* camera, FSM* fsm, float& elapsedTime)
{
	self = (MyCamera*)camera;

	self->node->setTranslation(self->defaultPos);
	self->node->setRotation(Matrix::identity());
	self->node->rotateX(MATH_DEG_TO_RAD(45.46f));
	self->panSpeed = -1;
	fsm->proceed();
}
void CameraFollow::Execute(Entity* camera, FSM* fsm, float& elapsedTime)
{
	// fuck, i have to put constants here instead of the header because they aren't
	// working from the header for some odd reason
	float ZOOM_TIME = 1000.0f;

	Vector3 ballPos = table->getBall()->getTranslation();
	float goalADist = (table->getGoal(1)->getTranslation() - ballPos).length();
	float goalBDist = (table->getGoal(2)->getTranslation() - ballPos).length();

	if (false && goalADist < ZOOM_DISTANCE){ // zoom into goalA
		if (curve == NULL){
			curve = new MyCurve(Curve::LINEAR);
			Vector3 curPos = self->GetNode()->getTranslation();
			curve->addPoint(0, curPos.x, curPos.y, curPos.z);
			curve->addPoint(ZOOM_IN_TIME, goalAViewPos.x, goalAViewPos.y, goalAViewPos.z);
		}
	}
	else if (false && goalBDist < ZOOM_DISTANCE){ // zoom into goalB
		if (curve == NULL){
			curve = new MyCurve(Curve::LINEAR);
			Vector3 curPos = self->GetNode()->getTranslation();
			curve->addPoint(0, curPos.x, curPos.y, curPos.z);
			
			curve->addPoint(ZOOM_IN_TIME, goalBViewPos.x, goalBViewPos.y, goalBViewPos.z);
		}
	}
	else if (curve != NULL){ // if not near goal, zoom out
		SAFE_DELETE(curve);
	}

	// move the camera now
	if (rotCurve){
		float rotation;
		rotCurve->evaluate(elapsedTime, rotation);
		self->node->setRotation(Vector3(1, 0, 0), MATH_DEG_TO_RAD(rotation));
		if (rotCurve->isComplete())
			SAFE_DELETE(rotCurve);
	}
	else if (curve != NULL){ // if dependent on curve
		Vector3 newPos;
		curve->evaluate(elapsedTime, newPos.x, newPos.y, newPos.z);
		self->GetNode()->setTranslation(newPos);
	}
	
	else{ // if free going
		Vector3 finalTrans(ballPos.x, self->DEFAULT_Y_OFFSET, self->DEFAULT_HEIGHT);
		self->GetNode()->translateSmooth(finalTrans, elapsedTime, RESPONSE_TIME);
	}
}
void CameraFollow::Exit(Entity* camera, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
void CameraFollow::OnMessage(Entity* camera, Telegram& telegram, FSM* fsm)
{
	switch (telegram.msg){
	case FSM::GAME_OVER:
		fsm->SetCurrentState(new CameraStage());
		break;
	case FSM::PEEK_UP:
		rotCurve = new MyCurve(Curve::LINEAR);

		Vector3 axis(1, 0, 0);
		float curRot = MATH_RAD_TO_DEG(self->node->getRotation(&axis));
		float time = *(int*)telegram.extraInfo;
		rotCurve->addPoint(0, curRot);
		rotCurve->addPoint(time * 0.25f, curRot + 30);
		rotCurve->addPoint(time * 0.75f, curRot + 30);
		rotCurve->addPoint(time, curRot);
		break;
	}
}