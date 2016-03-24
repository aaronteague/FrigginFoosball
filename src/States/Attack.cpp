#include "Attack.h"
#include "../Table.h"

#include "../FSM.h"
#include "../Opponent.h"

Attack::Attack(Table* table)
	:table(table) {
}

void Attack::Enter(Entity* opponent, FSM* fsm, float& elapsedTime)
{
	self = (Opponent*)opponent;
//	self->leftHand->setTranslation(self->correctedYTrans(self->_scene->findNode("Pole7")));
//	self->rightHand->setTranslation(self->correctedYTrans(self->_scene->findNode("Pole5")));
	self->changeAnimSet(5, 7);

	fsm->proceed();
}
void Attack::Execute(Entity* opponent, FSM* fsm, float& elapsedTime)
{
	Node* bar = table->getDefendingBar(self->ownedPoles);
	if (!bar)bar = table->getInterceptingBar();

	if (!self->belongsToCharacter(2, bar))  // "2" is useless
		return;

	if (!self->handOnBar(bar)){
		self->putHandOnBar(bar);
		return;
	}

	Node* closestMan = table->getClosestMan(bar);
	Vector3 difference = table->getDifference(closestMan);

	if (abs(difference.y) < 0.1f)
		((PhysicsRigidBody*)bar->getCollisionObject())->setLinearVelocity(Vector3(0, 0, 0));
	if (difference.y < 0){// slide up
	//	((PhysicsRigidBody*)bar->getCollisionObject())->applyForce(Vector3(0, LINEAR_FORCE, 0)); // wake up
		((PhysicsRigidBody*)bar->getCollisionObject())->applyImpulse(Vector3(0, LINEAR_FORCE, 0));

	}
	else{// slide down
	//	((PhysicsRigidBody*)bar->getCollisionObject())->applyForce(Vector3(0, LINEAR_FORCE * -1, 0)); // wake up
		((PhysicsRigidBody*)bar->getCollisionObject())->applyImpulse(Vector3(0, LINEAR_FORCE * -1, 0));

	}
	Vector3 axis(0, 1, 0);
	float rotationRad = bar->getRotation(&axis);
	float testAngle = MATH_RAD_TO_DEG(rotationRad);
	if (difference.x < STRIKE_DISTANCE){// STRIKE
  		((PhysicsRigidBody*)bar->getCollisionObject())->applyTorqueImpulse(Vector3(0, 0.1f, 0));
	}
	else if (difference.length() < READY_DISTANCE && difference.length() > READY_DISTANCE - 0.1f){ // GET READY TO STRIKE
		((PhysicsRigidBody*)bar->getCollisionObject())->applyTorqueImpulse(Vector3(0, BACK_ANGULAR_FORCE * -1, 0));
	}
}
void Attack::Exit(Entity* opponent, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
void Attack::OnMessage(Entity* opponent, Telegram& telegram, FSM* fsm)
{
	//switch (telegram.msg){

	//}
}