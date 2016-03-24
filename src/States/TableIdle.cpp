#include "TableIdle.h"
#include "../Table.h"
#include "../FSM.h"

TableIdle::TableIdle()
:self(NULL), floating(false)
{

}
void TableIdle::Enter(Entity* table, FSM* fsm, float& elapsedTime)
{
	self = (Table*)table;
	fsm->proceed();
}
void TableIdle::Execute(Entity* table, FSM* fsm, float& elapsedTime)
{
	srand(Game::getGameTime());
	if (floating)
		timer += elapsedTime;
	
	if (timer > FLOAT_TIMER){
		self->showScore = false;
		self->isRunning = true;
		floating = false;
		float x = (rand() % 100) / 100.0f - 0.5f;
		float y = (rand() % 100) / 100.0f - 0.5f;
		self->ballNode->setTranslation(0, 0, 5);
		((PhysicsRigidBody*)self->ballNode->getCollisionObject())->setKinematic(false);
		((PhysicsRigidBody*)self->ballNode->getCollisionObject())->applyImpulse(Vector3(x * 5, y * 5, 0));
		timer = 0;
	}

//	if (timer > FLOAT_TIMER)
//		((PhysicsRigidBody*)self->ballNode->getCollisionObject())->setKinematic(false);

	// update alpha of table if ball in not visable
	if (self->ballNode->getTranslationY() < -2.5f){
		float result = 1 - ((self->ballNode->getTranslationY() + 2.5f) * -1);
		((Model*)self->tableNode->getDrawable())->getMaterial()->getParameter("u_modulateAlpha")->setFloat(result);
	}		
	else
		((Model*)self->tableNode->getDrawable())->getMaterial()->getParameter("u_modulateAlpha")->setFloat(1.0f);

	// ball check
	if (self->ballNode->getTranslationZ() < -2){
		self->ballNode->setTranslation(0, 0, 0.5f);
	}

	if (powerupTimer >= 0)
		powerupTimer += elapsedTime;

	if (powerupTimer > POWERUP_TIMER){
		if (powerUpItem){
			self->_scene->removeNode(powerUpItem);
			((PhysicsRigidBody*)powerUpItem->getCollisionObject())->setKinematic(true);
			powerUpItem->setCollisionObject(PhysicsCollisionObject::GHOST_OBJECT);
			powerUpItem->getCollisionObject()->setEnabled(false);
			powerUpItem = NULL;
		}
		else if (magnetTarget > 0){
			magnetTarget = -1;
			((ParticleEmitter*)emitNode->getDrawable())->stop();
			self->magnetEmitter = NULL;
			self->ballNode->removeChild(emitNode);
		//	self->_scene->removeNode(emitNode);
		}
		
		self->speedExtraIndex = -1;
		
		powerupTimer = -1;
		
	}

	if (addPhysics){
		addPhysics = false;
		//powerUpItem->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box());
		powerUpItem->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box());
	}

	if (magnetTarget >= 0){  // do the magnet thingy
		//Node* bar = self->getInterceptingBar();
		Node* man = self->getClosestMan(magnetTarget);
		Vector3 difference = (man->getTranslationWorld() - self->ballNode->getTranslationWorld());
		if (self->getOwner(man) == magnetTarget){
			
			self->magnetEmitter = ((ParticleEmitter*)emitNode->getDrawable());
			((ParticleEmitter*)emitNode->getDrawable())->setVelocity(difference * 10, Vector3(0, 0, 0));

		//	((ParticleEmitter*)emitNode->getDrawable())->setVelocity(difference, Vector3(0,0,0));
			((PhysicsRigidBody*)self->ballNode->getCollisionObject())->applyForce(difference * 30);
		//	
			
			
			
		}
		else{
			self->magnetEmitter = NULL;
		}
		((ParticleEmitter*)emitNode->getDrawable())->update(elapsedTime);
	}
}
void TableIdle::Exit(Entity* table, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
void TableIdle::OnMessage(Entity* table, Telegram& telegram, FSM* fsm)
{
	PhysicsRigidBody* body = (PhysicsRigidBody*)self->ballNode->getCollisionObject();
	switch (telegram.msg){
	case FSM::GOAL_SCORED:
		
		body->setKinematic(true);
		self->ballNode->setTranslation(0, 0, 5);
		self->isRunning = false;
		
		break;
	case FSM::START_ROUND:
		self->showScore = true;
		self->ballNode->setTranslation(0, 0, 5);
		floating = true;
		timer = 0;
		self->isRunning = true;
		self->roundTimer = 0;
		break;
	case FSM::BARRIER_POWERUP:
		powerUpItem = (Node*)telegram.extraInfo;
		self->_scene->addNode(powerUpItem);
		if (powerUpItem->getCollisionObject())
			((PhysicsRigidBody*)powerUpItem->getCollisionObject())->setKinematic(true);
		if (self->ballOwner == 1){
			powerUpItem->setTranslationX(-5.7f);
		}
		else{
			powerUpItem->setTranslationX(5.7f);
		}
		powerUpItem->setRotation(Vector3(0, 0, 1), MATH_DEG_TO_RAD(90));
		addPhysics = true;
		powerupTimer = 0;
		notifyPowerReceived("Barrier", self->ballOwner);
		break;
	case FSM::SPEED_POWERUP:
		self->speedExtraIndex = self->ballOwner;
		powerupTimer = 0;
		notifyPowerReceived("Speed", self->ballOwner);
		break;
	case FSM::MAGNET_POWERUP:
		// add particles here eventually
		emitNode = (Node*)telegram.extraInfo;
		magnetTarget = self->ballOwner;
	//	emitNode->setTranslation(self->ballNode->getTranslation());
		self->ballNode->addChild(emitNode);
	//	self->_scene->addNode(emitNode);
		
		self->magnetEmitter = (ParticleEmitter*)emitNode->getDrawable();
		((ParticleEmitter*)emitNode->getDrawable())->start();
		powerupTimer = 0;
		notifyPowerReceived("Magnet", self->ballOwner);
		break;
	}
}

void TableIdle::notifyPowerReceived(std::string message, int owner)
{
	lua_getglobal(L, "notifyNearGoal");
	lua_pushstring(L, message.c_str()); // player two's goal
	lua_pushnumber(L, owner);
	lua_call(L, 2, 0);
	lua_pop(L, 0);
	lua_pop(L, 0);
}