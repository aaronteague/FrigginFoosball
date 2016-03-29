#pragma once

#include "gameplay.h"
#include "Entity.h"

class Table;

class PowerUp : public Entity, public PhysicsCollisionObject::CollisionListener
{
public:
	PowerUp(Scene* _scene, Table* table, Node* ball);
	void Update(const float& elapsedTime);
	void Render();

	void collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
		const PhysicsRigidBody::CollisionPair& pair,
		const Vector3& pointA, const Vector3& pointB);

	void Clear();
private:
	enum Power{
		SPEED,
		MAGNET,
		BARRIER
	};
	std::vector<Node*> powerList;
	Node* currentFieldItem = NULL;
	Scene* _scene = NULL;
	float timer = 0;
	
	float fieldWidth = 11;
	float fieldLength = 7;

	Table* table;
	Node* ballNode;
	Node* wall;
	Node* emitNode;
	Node* removableObject = NULL;
	AnimationClip *animClip = NULL;
	
	AudioSource* receivedSound;
	AudioSource* spawnedSound;

	void spawnItem();
};