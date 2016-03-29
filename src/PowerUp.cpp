#include "PowerUp.h"

#include "FSM.h"
#include "Table.h"

PowerUp::PowerUp(Scene* _scene, Table* table, Node* ball)
:_scene(_scene), table(table), ballNode(ball)
{
	Bundle* bundle = Bundle::create("res/powerUps.gpb");
	Texture* tex = Texture::create("res/powerUps.png");
	powerList.resize(3);
	Node* node = bundle->loadNode("Magnet");
//	node->setScale(0.3f, 0.3f, 0.3f);
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, tex, Entity::TEXTURED_ANIMATED, true, 5));
	powerList[MAGNET] = node;

	node = bundle->loadNode("Barrier");
//	node->setScale(0.3f, 0.3f, 0.3f);
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, tex, Entity::TEXTURED_ANIMATED, true, 5));
	powerList[BARRIER] = node;

	node = bundle->loadNode("Speed");
//	node->setScale(0.3f, 0.3f, 0.3f);
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, tex, Entity::TEXTURED_ANIMATED, true, 5));
	powerList[SPEED] = node;

	wall = bundle->loadNode("Wall");
	((Model*)wall->getDrawable())->setMaterial(buildMaterial(_scene, tex, Entity::TEXTURED_SHADELESS, true, -1));
	
	ParticleEmitter* emitter = ParticleEmitter::create("res/particle.particle");
	emitNode = Node::create("emitter");
	emitNode->setDrawable(emitter);

	tex->release();
	bundle->release();

	receivedSound = AudioSource::create("res/sounds/ping.wav");
	spawnedSound = AudioSource::create("res/sounds/ping.wav");
}
void PowerUp::Update(const float& elapsedTime)
{
	
	if (removableObject){
		_scene->removeNode(removableObject);
		removableObject->getCollisionObject()->removeCollisionListener(this);
		removableObject->getCollisionObject()->setEnabled(false);
		removableObject = NULL;
	}

	if (elapsedTime < 100.0f && !currentFieldItem)
		timer += elapsedTime;

	float SPAWN_TIME = 20000; // 20 seconds
	if (timer >= SPAWN_TIME){
		timer = 0;
		if (!currentFieldItem)
			spawnItem();
	}
}

void PowerUp::Render()
{
	// add more to it later, mmkay?
}

void PowerUp::collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
	const PhysicsRigidBody::CollisionPair& pair,
	const Vector3& pointA, const Vector3& pointB)
{
	if (type != EventType::COLLIDING)
		return;


	if (eitherMatch(pair, currentFieldItem) && eitherMatch(pair, ballNode))
	{
		std::string id(currentFieldItem->getId());
		int message = -1;
		Node* passNode = NULL;
		if (id.compare("Speed") == 0)
			message = FSM::SPEED_POWERUP;
		else if (id.compare("Magnet") == 0){
			message = FSM::MAGNET_POWERUP;
			passNode = emitNode;
		}
		else if (id.compare("Barrier") == 0){
			message = FSM::BARRIER_POWERUP;
			passNode = wall;
		}


		
		_scene->removeNode(currentFieldItem);
		receivedSound->play();
		removableObject = currentFieldItem;
		currentFieldItem = NULL;
		//if (removableObject == NULL)
		//	int problem = 666;
		//currentFieldItem->getCollisionObject()->setEnabled(false);
		//currentFieldItem = NULL;
		MessageDispatcher::Instance()->dispatchMessage(0, this, (Entity*)table, message, passNode);
	}

}

void PowerUp::spawnItem()
{

	srand(Game::getAbsoluteTime());
	int index = MATH_RANDOM_0_1() * 3;
	currentFieldItem = powerList[index];
	_scene->addNode(currentFieldItem);

	currentFieldItem->setTranslationZ(0);
	float x = MATH_RANDOM_0_1() * fieldWidth - fieldWidth / 2.0f;
	currentFieldItem->setTranslationX(x);
	float y = MATH_RANDOM_0_1() * fieldLength - fieldLength / 2.0f;
	currentFieldItem->setTranslationY(y);

	Animation* anim = currentFieldItem->getAnimation("animations");
	anim->createClips("res/powerUp.animation");
	animClip = anim->getClip("Idle");
	animClip->play();
	spawnedSound->play();
	anim->release();

	currentFieldItem->setCollisionObject(PhysicsCollisionObject::GHOST_OBJECT, PhysicsCollisionShape::sphere(0.3f, Vector3::zero(), true));
	currentFieldItem->getCollisionObject()->setEnabled(true);
	currentFieldItem->getCollisionObject()->addCollisionListener(this);
}

void PowerUp::Clear()
{
	if (currentFieldItem)
		removableObject = currentFieldItem;
		
	timer = 0;
}