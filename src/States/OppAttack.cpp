#include "OppAttack.h"
#include "../Table.h"
#include "../Opponent.h"
#include "../FSM.h"

OppAttack::OppAttack(Table* table)
{
	OppAttack::table = table;
}

void OppAttack::Enter(Entity* opponent, FSM* fsm, float& elapsedTime)
{
	self = (Opponent *)opponent;
	fsm->proceed();
}

void OppAttack::Execute(Entity* opponent, FSM* fsm, float& elapsedTime)
{
	PhysicsRigidBody* ballBody = (PhysicsRigidBody*)table->getBall()->getCollisionObject();
	gameplay::Vector3 ballVelocity = ballBody->getLinearVelocity();
	gameplay::Vector3 ballPos = table->getBall()->getTranslationWorld();

	float difficulty = self->difficulty;

	int numOfPoles = self->ownedPoles.size();
	for (int i = 0; i < numOfPoles; ++i) {
		float angle = poleAngle(self->ownedPoles[i]);
		Vector3 polePos = self->ownedPoles[i]->getTranslationWorld();
		PhysicsRigidBody* poleBody = (PhysicsRigidBody*)self->ownedPoles[i]->getCollisionObject();
		
		bool ballFollow = false;

		if (ballPos.x < polePos.x // STRIKE
			&& ballPos.x > polePos.x - STRIKE_DISTANCE) {
			ballFollow = true;
			rotateToPos(self->ownedPoles[i], STRIKE_ANGLE, FORWARD_SPEED * difficulty);
		}
		else if (ballPos.x < polePos.x // READY
			&& ballPos.x > polePos.x - READY_DISTANCE) {
			ballFollow = true;
			rotateToPos(self->ownedPoles[i], READY_ANGLE, BACK_SPEED * difficulty);
		}
		else if (ballPos.x > polePos.x // BEHIND STRIKE
			&& ballPos.x < polePos.x + STRIKE_DISTANCE) {
			ballFollow = true;
			poleBody->applyTorqueImpulse(Vector3(0, FORWARD_SPEED, 0));
		}

		if (ballFollow) {
			Node* closestMan = table->getClosestMan(self->ownedPoles[i]);
			Vector3 difference = table->getDifference(closestMan);

			if (abs(difference.y) < 0.1f)
				poleBody->setLinearVelocity(Vector3(0, 0, 0));
			if (difference.y < 0) {// slide up
				poleBody->applyImpulse(Vector3(0, LINEAR_FORCE * difficulty, 0));
			}
			else {// slide down
				poleBody->applyImpulse(Vector3(0, LINEAR_FORCE * difficulty * -1, 0));

			}
		}
	}
}

void OppAttack::Exit(Entity* opponent, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}

void OppAttack::OnMessage(Entity* opponent, Telegram& telegram, FSM* fsm)
{

}

float OppAttack::poleAngle(Node* pole)
{
	Node* firstMan = pole->getFirstChild();
	Vector3 difference = firstMan->getTranslationWorld() - pole->getTranslationWorld();

	difference.y = 0;
	difference.normalize();

	return MATH_RAD_TO_DEG(atan2(difference.z, difference.x));
}

void OppAttack::rotateToPos(Node* pole, float angle, float speed)
{
	const float VARIANCE = 1.0f;
	float currentAngle = poleAngle(pole);
	if (currentAngle > angle - VARIANCE && currentAngle < angle + VARIANCE) {
		((PhysicsRigidBody*)pole->getCollisionObject())->setAngularVelocity(0, 0, 0); // STAHP
	}
	else {
		((PhysicsRigidBody*)pole->getCollisionObject())->applyTorqueImpulse(Vector3(0, speed, 0)); // MOVE
	}
}
