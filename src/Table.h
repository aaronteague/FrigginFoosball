#pragma once

#include "gameplay.h"
#include "Entity.h"

using namespace gameplay;

class MyCamera;
class PowerUp;

class Table : public Entity, public PhysicsCollisionObject::CollisionListener
{
	friend class TableIdle;
public:
	Table(Scene* _scene, Vector2 screenSize, MyCamera* camera);
	void Load(std::string file);
	void Clear();
	void Update(const float& elapsedTime);
	void Render();

	Node* getInterceptingBar();
	Node* getDefendingBar(std::vector<Node*> &characterBars);
	Node* getClosestMan(Node* bar);
	Node* getClosestMan(int character);
	Vector3 getDifference(Node* man);

	std::vector<Node*> getHandles(int characterID = -1);
	Node* getBall(){ return ballNode; }
	Node* getGoal(int goalID);

	void keyEvent(Keyboard::KeyEvent evt, int key);
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	bool getRunning(){ return isRunning; }
//	void setRunning(bool running){ Table::isRunning = running; }
private:
	const int NUM_OF_SOUND_EFFECTS = 8;
	enum Sounds{
		BALL_ROLL,
		POLE_SPIN,
		BALL_LANDING,
		BALL_TAP_MAN,
		BALL_SMACK_MAN,
		BALL_HIT_WALL,
		BUMPER_AGAINST_WALL,
		BALL_IN_GOAL
	};

	float ANG_VELOCITY_LOCK = 0.5f;

	std::vector<AudioSource*> soundEffects;

	Scene* _scene;
	Vector2 screenSize;

	MyCamera* camera;

	std::vector<Node*> p1Handles;
	std::vector<Node*> p2Handles;
	std::vector<Node*> poles;
	Node* goalA;
	Node* goalB;

	Vector2 screenTouchPos;
	Vector2 worldTouchPos;

	Node* ballNode;
	float ballSpeed;
	Node* tableNode;
	Node* fieldNode;
	std::vector<Node*> sides;

	std::vector < std::vector<Node*>> manNodes;
	std::vector<Vector3> shadowPos;
	std::vector<float> shadowAlpha;

	SpriteBatch* shadowBatch;
	Font* font;
	PowerUp* powerUp;

	ParticleEmitter* ballEmitter = NULL;
	ParticleEmitter* magnetEmitter = NULL;

	int p1Score;
	int p2Score;
	bool showScore;
	bool notify = false;
	bool nearGoal = false;

	float roundTimer = 0;

	int ballOwner = 1;
	int speedExtraIndex = -1;

	void loadSounds();
	void setSound(Sounds soundToPlay, bool toPlay = true, float pitch = 1.0f, float gain = 1.0f);

	int getOwner(Node* man);
	/// ...
	/**
	* Collision event handler.
	*/
	void collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
		const PhysicsRigidBody::CollisionPair& pair,
		const Vector3& pointA, const Vector3& pointB);

	
	Node* hasHitMan(const PhysicsRigidBody::CollisionPair& pair);
	bool hasHitSides(const PhysicsRigidBody::CollisionPair& pair);

	bool isRunning = false;

	float getSoftLock(const Vector2& difference, const Vector3& manPos, const Vector3& polePos);
	void checkScriptCallbacks(const float& elapsedTime);
};