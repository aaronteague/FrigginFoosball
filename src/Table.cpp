#include "Table.h"
#include "FSM.h"
#include "MyCamera.h"
#include "LuaLoader.h"
#include "PowerUp.h"

#include "States\TableIdle.h"

Table::Table(Scene* _scene, Vector2 screenSize, MyCamera* camera)
:_scene(_scene), screenSize(screenSize), p1Score(0), p2Score(0), showScore(false), camera(camera), tableNode(NULL)
{
	shadowBatch = SpriteBatch::create("res/shadow.png");
	shadowBatch->getStateBlock()->setDepthTest(true);

	font = Font::create("res/arial.gpb");


	screenTouchPos.set(-1, -1);
	worldTouchPos.set(-1, -1);

	// load up the table

	stateMachine->SetCurrentState(new TableIdle());

	loadSounds();
	powerUp = NULL;

	// bind taht lua
	luabridge::getGlobalNamespace(L)
		.beginNamespace("foostable")
		.deriveClass<Table, Entity>("FoosTable")
		.addFunction("Load", &Table::Load)
		.endClass()
		.endNamespace();
}


void Table::Update(const float& elapsedTime)
{
	if (notify){
		notify = false;
		luabridge::LuaRef notifyRef = luabridge::getGlobal(L, "resume");
		notifyRef();
	}

	if (tableNode == NULL)
		return;

	
	stateMachine->Update((float&)elapsedTime, true);
	powerUp->Update(elapsedTime);

	Vector3 trans = ballNode->getTranslation();
	((PhysicsRigidBody*)ballNode->getCollisionObject())->applyForce(trans * -0.7f); // opposite direction at fraction

	if (screenTouchPos.x >= 0){
		Vector2 newTouch = camera->screenToFieldCoordinates(screenTouchPos);
		if (worldTouchPos.x == -1 && worldTouchPos.y == -1)
			worldTouchPos = newTouch; // prevents jumping
		Vector2 difference = newTouch - worldTouchPos;


		
		float change = 1000 / elapsedTime;
		difference *= change;
		worldTouchPos = newTouch;
		for (unsigned int i = 0; i < p1Handles.size(); ++i){
			//	p1Handles[i]->translate(difference.x, difference.y, 0);
			((PhysicsRigidBody*)p1Handles[i]->getCollisionObject())->applyImpulse(Vector3(0, 0.1f, 0));
			int MAX_SPEED = 23;
			float MAX_ROTATION = 40;
			if (difference.y > MAX_SPEED)
				difference.y = MAX_SPEED;
			else if (difference.y < -MAX_SPEED)
				difference.y = -MAX_SPEED;

			if (difference.x > MAX_ROTATION)
				difference.x = MAX_ROTATION;
			else if (difference.x < -MAX_ROTATION)
				difference.x = -MAX_ROTATION;
			

			//Vector3 dir = manNodes[i][0]->getTranslationWorld() - poles[i]->getTranslationWorld();
			//dir.y = 0;
			//dir.normalize();
			//float ang = MATH_RAD_TO_DEG(atan2(dir.z, dir.x));
		
			//float rotate = 0.3f;
			//if (ang < -45 && ang > -135)
			//	rotate = 1.0f;
			
			// i have given up on the soft lock
		//	float softLock = getSoftLock(difference, poles[i]->getFirstChild()->getTranslationWorld(), poles[i]->getTranslationWorld());
			((PhysicsRigidBody*)p1Handles[i]->getCollisionObject())->setAngularVelocity(Vector3(0, difference.x, 0));
			
			((PhysicsRigidBody*)p1Handles[i]->getCollisionObject())->setLinearVelocity(Vector3(0, difference.y, 0));

		}
	}
	else{

		// update p1 pegs
		for (unsigned int i = 0; i < p1Handles.size(); ++i){
			Vector3 manTransWorld = p1Handles[i]->getFirstChild()->getTranslationWorld();
			Vector3 poleTrans = p1Handles[i]->getTranslation();
			float difference = (manTransWorld - poleTrans).x * 0.15f;

			Vector3 dir = manNodes[i][0]->getTranslationWorld() - poles[i]->getTranslationWorld();
			dir.y = 0;
			dir.normalize();
			float ang = MATH_RAD_TO_DEG(atan2(dir.z, dir.x));


			((PhysicsRigidBody*)p1Handles[i]->getCollisionObject())->setLinearVelocity(Vector3(0, 0, 0));
		}
	}

	// get shadows positions
	shadowPos.clear();
	shadowAlpha.clear();
	for (unsigned int i = 0; i < manNodes.size(); ++i)
	for (unsigned int j = 0; j < manNodes[i].size(); ++j){
		Vector3 pos = manNodes[i][j]->getTranslationWorld();

		shadowAlpha.push_back(1 - (pos.z * 1.0f) - 0.3f);
		pos.z = 0.01f;
		shadowPos.push_back(pos);
	}
	Vector3 pos = ballNode->getTranslationWorld();

	shadowAlpha.push_back(1.0f);
	pos.z = 0.011f;
	shadowPos.push_back(pos);

	// update ball roll sound
	const float SPEED_CALC_ADJUST = 2.0f;
	ballSpeed = ((PhysicsRigidBody*)ballNode->getCollisionObject())->getLinearVelocity().length() / elapsedTime * SPEED_CALC_ADJUST;

	if (ballSpeed > 0.01f && ballNode->getTranslationZ() < 0.25f)
		setSound(BALL_ROLL, true, ballSpeed);
	else
		setSound(BALL_ROLL, false);

	if (ballSpeed > 0.5f && !ballEmitter->isStarted() && ballNode->getTranslationZ() < 0.5f)
		ballEmitter->start();
	else if (ballSpeed <= 0.5f && ballEmitter->isStarted())
		ballEmitter->stop();

	roundTimer += elapsedTime;

	ballEmitter->update(elapsedTime);
	checkScriptCallbacks(elapsedTime);
}

float Table::getSoftLock(const Vector2& difference, const Vector3& manPos, const Vector3& polePos)
{
	Vector3 dir = manPos - polePos;
	dir.y = 0;
	dir.normalize();
	float ang = MATH_RAD_TO_DEG(atan2(dir.z, dir.x));

	if (abs(difference.x) < 1.0f)
		return 0.0f;

	if (ang > 0)
		return 2.0f;
	
	if (difference.x > 0){
		if (ang < -160)
			return 0.0f;
		else if (ang < -90 && ang > -125)
			return 1.0f;
		else if (ang < -125 && ang > -180)
			//return 0.0f;
			return (-180.0f - ang) / -55.0f;
		else if (ang < 0.0)
			return 1.0f;
	}
	else if (difference.x < 0){
		if (ang > -20)
			return 0.0f;
		else if (ang > -90 && ang < -55)
			return 1.0f;
		else if (ang < 0 && ang > -55)
			//return 0.0f;
			return ang / -55.0f;
		else if (ang > -180)
			return 1.0f;
	}

	return 0.2f;
}

void Table::Render()
{
	if (tableNode == NULL)
		return;

	powerUp->Render();

	// just draw the shadows, the main class is covering the model drawing
	Vector3 r;
	Vector3 faceUp;
	Vector4 color;
	r.set(1, 0, 0);
	faceUp.set(0, 1, 0);
	color.set(0.9f, 0.9f, 0.9f, 0.5f);

	Matrix viewProjMatrix = _scene->getActiveCamera()->getViewProjectionMatrix();
	shadowBatch->setProjectionMatrix(viewProjMatrix);

	shadowBatch->start();
	for (unsigned int i = 0; i < shadowPos.size(); ++i){

		shadowBatch->draw(shadowPos[i], r, faceUp, 0.5f, 0.5f, 0, 0, 1, 1, Vector4(1, 1, 1, shadowAlpha[i]), Vector2(0.5f, 0.5f), 0);

	}
	shadowBatch->finish();

	// show score if needed
	if (showScore){
		font->start();
		font->drawText("The Current Score is:", gameplay::Rectangle(0, 0, screenSize.x, 200), Vector4(1, 1, 1, 1), font->getSize() * 10, Font::Justify::ALIGN_HCENTER);

		font->drawText(to_string(p1Score).c_str(), gameplay::Rectangle(0, 300, screenSize.x / 2, screenSize.y - 300), Vector4(1, 0, 0, 1), font->getSize() * 30, Font::Justify::ALIGN_HCENTER);
		font->drawText(to_string(p2Score).c_str(), gameplay::Rectangle(screenSize.x / 2, 300, screenSize.x / 2, screenSize.y - 300), Vector4(0, 0, 1, 1), font->getSize() * 30, Font::Justify::ALIGN_HCENTER);

		font->finish();
	}

	if (ballEmitter)
		ballEmitter->draw();
	if (magnetEmitter)
		magnetEmitter->draw();
}

Node* Table::getInterceptingBar()
{
	Vector3 velocity = ((PhysicsRigidBody*)ballNode->getCollisionObject())->getLinearVelocity();
	Vector3 position = ballNode->getTranslation();

	Node* closest = NULL;
	Node* secondClosest = NULL;
	float smallestDistance = 100;
	for (unsigned int i = 0; i < poles.size(); ++i){
		if (closest == NULL){
			closest = poles[i];

		}
		else{
			float distance = (poles[i]->getTranslation() - ballNode->getTranslation()).length();
			if (distance < smallestDistance){
				smallestDistance = distance;
				closest = poles[i];
			}
		}
	}
	smallestDistance = 100;
	for (unsigned int i = 0; i < poles.size(); ++i){
		if (secondClosest == NULL){
			secondClosest = poles[i];

		}
		else{
			float distance = (poles[i]->getTranslation() - ballNode->getTranslation()).length();
			if (distance < smallestDistance && poles[i] != closest){
				smallestDistance = distance;
				secondClosest = poles[i];
			}
		}
	}

	// use direction to find intercepting
	Vector3 newPos = velocity * 10 + position;

	float distance1 = (newPos - closest->getTranslation()).length();
	float distance2 = (newPos - secondClosest->getTranslation()).length();

	if (distance1 < distance2)
		return closest;
	else
		return secondClosest;

}
Node* Table::getDefendingBar(std::vector<Node*> &characterBars)
{
	// go for x > ball.x
	float shortestDistance = 100;
	Node* closestBar = characterBars[0];
	for (unsigned int i = 0; i < characterBars.size(); ++i){
		if (characterBars[i]->getTranslationX() > ballNode->getTranslationX()){
			float distance = characterBars[i]->getTranslationX() - ballNode->getTranslationX();
			if (distance < shortestDistance){
				shortestDistance = distance;
				closestBar = characterBars[i];
			}
		}
	}

	if (shortestDistance < 1)
		return closestBar;
	else
		return NULL;
}
Node* Table::getClosestMan(Node* bar)
{
	std::vector<Node*> manList;
	manList.push_back(bar->getFirstChild());
	while (true){
		Node* man = manList[manList.size() - 1]->getNextSibling();
		if (man == NULL)
			break;

		manList.push_back(man);
	}

	Node* closestMan = NULL;
	float closestDistance = 100;
	for (unsigned int i = 0; i < manList.size(); ++i){
		if (closestMan == NULL){
			closestMan = manList[i];

		}
		else{
			float distance = (manList[i]->getTranslation() - ballNode->getTranslation()).length();
			if (distance < closestDistance){
				closestMan = manList[i];
				closestDistance = distance;
			}
		}


	}

	return closestMan;
}
Vector3 Table::getDifference(Node* man)
{
	return Vector3(man->getParent()->getTranslationX(), man->getTranslationWorld().y, man->getTranslationWorld().z)
		- ballNode->getTranslationWorld();
}

std::vector<Node*> Table::getHandles(int characterID)
{
	if (characterID == -1)
		return poles;
	else if (characterID == 1)
		return p1Handles;
	else
		return p2Handles;
}

void Table::keyEvent(Keyboard::KeyEvent evt, int key)
{
	if (evt == Keyboard::KEY_PRESS)
	{
		switch (key)
		{
		case Keyboard::KEY_DOWN_ARROW:
			((PhysicsRigidBody*)ballNode->getCollisionObject())->applyImpulse(Vector3(0, -10, 0));
			break;
		case Keyboard::KEY_UP_ARROW:
			((PhysicsRigidBody*)ballNode->getCollisionObject())->applyImpulse(Vector3(0, 10, 0));
			break;
		case Keyboard::KEY_RIGHT_ARROW:
			((PhysicsRigidBody*)ballNode->getCollisionObject())->applyImpulse(Vector3(10, 0, 0));
			break;
		case Keyboard::KEY_LEFT_ARROW:
			((PhysicsRigidBody*)ballNode->getCollisionObject())->applyImpulse(Vector3(-10, 0, 0));
			break;
		}
	}
}

void Table::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	switch (evt)
	{
	case Touch::TOUCH_PRESS:
		screenTouchPos.set(x, y);
		break;
	case Touch::TOUCH_RELEASE:
		screenTouchPos.set(-1, -1);
		worldTouchPos.set(-1, -1);
		break;
	case Touch::TOUCH_MOVE:
		screenTouchPos.set(x, y);
		break;
	};
}

void Table::collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
	const PhysicsRigidBody::CollisionPair& pair,
	const Vector3& pointA, const Vector3& pointB)
{
	if (type == EventType::NOT_COLLIDING)
		return;

//	if (pair.objectA->getNode() != ballNode && pair.objectB->getNode() != ballNode)
	if (!eitherMatch(pair, ballNode))
		return;

	const int SCORE_TO_WIN = 10;

	if (eitherMatch(pair, goalA)){
		p1Score++;
		ballNode->setTag("visible", "false");
		if (p1Score < SCORE_TO_WIN){
			//luabridge::LuaRef notify = luabridge::getGlobal(L, "notifyScore");
			
			MessageDispatcher::Instance()->dispatchMessage(0, this, this, FSM::GOAL_SCORED, goalA);
			//notify(1, p1Score);
			//Table::notify = true;

			lua_getglobal(L, "notifyScore");
			lua_pushnumber(L, 1);
			lua_pushnumber(L, p1Score);
			lua_call(L, 2, 0);
			lua_pop(L, 0);
			lua_pop(L, 0);
		}
		else{
			int playerWin = 1;
			MessageDispatcher::Instance()->dispatchMessage(0, this, this, FSM::GAME_OVER, &playerWin);
			MessageDispatcher::Instance()->dispatchMessage(0, this, camera, FSM::GAME_OVER, NULL);

			lua_getglobal(L, "notifyGameEnd");
			lua_pushnumber(L, 1);
			lua_call(L, 1, 0);
			lua_pop(L, 0);
			

		}
	}
	else if (eitherMatch(pair, goalB)){
		p2Score++;
		ballNode->setTag("visible", "false");
		
		if (p2Score < SCORE_TO_WIN){
			//luabridge::LuaRef notify = luabridge::getGlobal(L, "notifyScore");
			
			
			MessageDispatcher::Instance()->dispatchMessage(0, this, this, FSM::GOAL_SCORED, goalB);

			lua_getglobal(L, "notifyScore");
			lua_pushnumber(L, 2);
			lua_pushnumber(L, p2Score);
			lua_call(L, 2, 0);
			lua_pop(L, 0);
			lua_pop(L, 0);
		}
		else{
			int playerWin = 2;
			MessageDispatcher::Instance()->dispatchMessage(0, this, this, FSM::GAME_OVER, &playerWin);
			MessageDispatcher::Instance()->dispatchMessage(0, this, camera, FSM::GAME_OVER, NULL);

			lua_getglobal(L, "notifyGameEnd");
			lua_pushnumber(L, 2);
			lua_call(L, 1, 0);
			lua_pop(L, 0);
		}
	}
	else if (eitherMatch(pair, fieldNode)){
		setSound(BALL_LANDING, true, 1.0f, ballSpeed);
	}
	else if (hasHitSides(pair)){
		setSound(BALL_HIT_WALL);
	}
	else{
		// get speed of bar spin to get tap sound gain
		Node* manHit = hasHitMan(pair);
		if (!manHit)
			return;

		float speedMultiplier = 1.0f;
		ballOwner = getOwner(manHit);
		if (ballOwner == speedExtraIndex)
			speedMultiplier = 3.0f;

		float angSpeed = ((PhysicsRigidBody*)manHit->getParent()->getCollisionObject())->getAngularVelocity().length() * 0.06f;
		if (angSpeed < 0.3f)
			setSound(BALL_TAP_MAN, true, 1.0f, 0.1f);
		else
			setSound(BALL_LANDING, true, 1.0f, 0.1f);
		Vector3 velocity = ((PhysicsRigidBody*)manHit->getParent()->getCollisionObject())->getAngularVelocity();
		const float VELOCITY_CORRECTION = -2.0f;
		((PhysicsRigidBody*)ballNode->getCollisionObject())->applyImpulse(Vector3(velocity.y* VELOCITY_CORRECTION, 0, 0) * speedMultiplier);
	}
		
}



Node* Table::hasHitMan(const PhysicsRigidBody::CollisionPair& pair)
{
	for (unsigned int i = 0; i < manNodes.size(); ++i)
	for (unsigned int j = 0; j < manNodes[i].size(); ++j)
	if (eitherMatch(pair, manNodes[i][j]))
		return manNodes[i][j];

	//if didn't see a collision here
	return NULL;	
}

Node* Table::getGoal(int goalID)
{
	if (goalID == 1)
		return goalA;
	else
		return goalB;
}

void Table::loadSounds()
{
	soundEffects.resize(NUM_OF_SOUND_EFFECTS);

	soundEffects[BALL_ROLL] = AudioSource::create("res/sounds/ball-roll.wav");
	soundEffects[BALL_ROLL]->setLooped(true);
//	soundEffects[POLE_SPIN] = AudioSource::create("res/sounds/pole-spin.wav");
	soundEffects[BALL_LANDING] = AudioSource::create("res/sounds/ball-landing.wav");
	soundEffects[BALL_TAP_MAN] = AudioSource::create("res/sounds/ball-tap-man.wav");
	soundEffects[BALL_SMACK_MAN] = AudioSource::create("res/sounds/ball-smack-man.wav");
	soundEffects[BALL_HIT_WALL] = AudioSource::create("res/sounds/ball-hit-wall.wav");
	soundEffects[BUMPER_AGAINST_WALL] = AudioSource::create("res/sounds/bumper-against-wall.wav");
//	soundEffects[BALL_IN_GOAL] = AudioSource::create("res/sounds/ball-in-goal.wav");
}

void Table::setSound(Sounds soundToPlay, bool toPlay, float pitch, float gain)
{
	AudioSource *sound = soundEffects[soundToPlay];

	sound->setPitch(pitch);
	sound->setGain(gain);

	if (toPlay && sound->getState() != AudioSource::PLAYING){
		sound->play();  // do i need play here instead
	}
	else if (!toPlay && sound->getState() == AudioSource::PLAYING)
		sound->stop();
}

bool Table::hasHitSides(const PhysicsRigidBody::CollisionPair& pair)
{
	for (unsigned int i = 0; i < sides.size(); ++i)
	if (eitherMatch(pair, sides[i]))
		return true;

	return false; // no hit

	
}

void Table::Load(std::string file)
{
	if (tableNode)
		return; // return if table has been loaded previously

	LuaLoader loader(L);
	loader.setFile(file);

	tableNode = _scene->findNode("Table");
	gameplay::Model* boxModel = dynamic_cast<gameplay::Model*>(tableNode->getDrawable());
	std::string tableTex = loader.getString("TABLE_TEXTURE");
	boxModel->setMaterial(buildMaterial(_scene, Texture::create(tableTex.c_str()), TEXTURED_SPECULAR, true, -1));

	Node* tempNode;
	PhysicsRigidBody::Parameters params(0.0f, 1.0, 1.0f, 1.0f, 1.0f, false);

	tempNode = _scene->addNode();
	tempNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(14.6f, 1.0f, 4.0f), Vector3(0, 4.3f, 2.0f), true), &params); // top
	tempNode->getCollisionObject()->addCollisionListener(this);
	sides.push_back(tempNode);

	tempNode = _scene->addNode();
	tempNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(14.6f, 1.0f, 4.0f), Vector3(0, -4.3f, 2.0f), true), &params); // bottom
	tempNode->getCollisionObject()->addCollisionListener(this);
	sides.push_back(tempNode);

	tempNode = _scene->addNode();
	tempNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(1, 3, 4), Vector3(6.6f, 2.5f, 2.0f), true), &params); // right-top
	tempNode->getCollisionObject()->addCollisionListener(this);
	sides.push_back(tempNode);

	goalA = _scene->addNode();
	goalA->setTranslation(7.0f, 0.0f, 0.0f);
	PhysicsCollisionObject* goalACol = goalA->setCollisionObject(PhysicsCollisionObject::GHOST_OBJECT, PhysicsCollisionShape::box(Vector3(1, 3, 4), Vector3(0.0f, 0.0f, 2.0f), true), &params); // right-goal
	goalACol->addCollisionListener(this); // collision listener

	tempNode = _scene->addNode();
	tempNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(1, 3, 4), Vector3(6.6f, -2.5f, 2.0f), true), &params); // right-bottom
	tempNode->getCollisionObject()->addCollisionListener(this);
	sides.push_back(tempNode);

	tempNode = _scene->addNode();
	tempNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(1, 3, 4), Vector3(-6.6f, 2.5f, 2.0f), true), &params); // left-top
	tempNode->getCollisionObject()->addCollisionListener(this);
	sides.push_back(tempNode);

	goalB = _scene->addNode();
	goalB->setTranslation(-7.0f, 0.0f, 0.0f);
	PhysicsCollisionObject* goalBCol = goalB->setCollisionObject(PhysicsCollisionObject::GHOST_OBJECT, PhysicsCollisionShape::box(Vector3(1, 3, 4), Vector3(0.0f, 0.0f, 2.0f), true), &params); // left-goal
	goalBCol->addCollisionListener(this);

	tempNode = _scene->addNode();
	tempNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(1, 3, 4), Vector3(-6.6f, -2.5f, 2.0f), true), &params); // left-bottom
	tempNode->getCollisionObject()->addCollisionListener(this);
	sides.push_back(tempNode);

	fieldNode = _scene->findNode("Field");
	node = fieldNode; // for targeting purposes
	std::string fieldTex = loader.getString("FIELD_TEXTURE");
	std::string fieldNormal = loader.getString("FIELD_NORMAL");
	Material* fieldMat = buildMaterial(_scene, Texture::create(fieldTex.c_str(), true), TEXTURED_NORMAL, false, -1);
	((gameplay::Model*)fieldNode->getDrawable())->setMaterial(fieldMat);
	applyNormalMap(fieldMat, Texture::create(fieldNormal.c_str(), true));

	params.restitution = 0.2f;
	fieldNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(14.0f, 8.0f, 4.0f), Vector3(0, 0.0f, -2.0f), true), &params); // field
	fieldNode->getCollisionObject()->addCollisionListener(this);

	// load up the ball
	ballNode = _scene->findNode("Ball");
	ballNode->setTag("visible", "false");
	ballEmitter = ParticleEmitter::create("res/particle.particle");
	Node* emitterNode = Node::create("emitter");
	//_scene->addNode(emitterNode);
	emitterNode->setDrawable(ballEmitter);
	ballNode->addChild(emitterNode);
	
	
	std::string ballTex = loader.getString("BALL_TEXTURE");
	std::string ballNormal = loader.getString("BALL_NORMAL");
	Material* ballMat = buildMaterial(_scene, Texture::create(ballTex.c_str()), TEXTURED_NORMAL, false, -1);
	((gameplay::Model*)ballNode->getDrawable())->setMaterial(ballMat);
	applyNormalMap(ballMat, Texture::create(ballNormal.c_str(), true));
	ballMat->getParameter("u_ambientColor")->setValue(Vector3(0.3f, 0.3f, 0.3f));

	PhysicsRigidBody::Parameters ballParams(10.0f, 0.5f, 0.5f, 0.0f, 0.3f);
	ballNode->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::sphere(), &ballParams);
	((PhysicsRigidBody*)ballNode->getCollisionObject())->setKinematic(true);
	ballNode->setTranslation(0, 0, 5);

	_scene->findNodes("Pole", poles, false, false);

	luabridge::LuaRef boundTable = loader.getTable("poleBounds");
	for (unsigned int i = 0; i < poles.size(); ++i){
		int index = (int)poles[i]->getId()[4] - '0';
		float pRadius = loader.getFloat(index, boundTable);

		((gameplay::Model*)poles[i]->getDrawable())->setMaterial(buildMaterial(_scene, NULL, COLORED, false, -1));

		PhysicsRigidBody::Parameters poleParam(10.0f, 0.0, 0.01f, 0.0f, 0.5f, false, Vector3::one(), Vector3(0, 1, 0), Vector3(0, 1, 0));
		PhysicsRigidBody::Parameters manParam(10.0f, 0.0f, 0.0f, 0.0f, 0.0f, true, Vector3::one(), Vector3::one(), Vector3(0, 1, 0));

		Vector3 translation = poles[i]->getTranslation();

		poles[i]->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(0.3f, pRadius * 2, 0.3f), Vector3(0, 0, 0), true), &poleParam);

		// get childs of handle
		Node* child = poles[i]->getFirstChild();
		std::vector<Node*> temp;
		if (child != NULL){
			((gameplay::Model*)child->getDrawable())->setMaterial(buildMaterial(_scene, NULL, COLORED, false, -1));
			
			child->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::sphere(0.2f, Vector3::zero(), true), &manParam);
			child->getCollisionObject()->addCollisionListener(this);
			//	Game::getPhysicsController()->createFixedConstraint(((PhysicsRigidBody*)handleNodeList[i]->getCollisionObject()), ((PhysicsRigidBody*)child->getCollisionObject()));
			temp.push_back(child);

		}

		if (child != NULL)
		while (true){
			child = child->getNextSibling();
			if (child == NULL)
				break;
			
			((gameplay::Model*)child->getDrawable())->setMaterial(buildMaterial(_scene, NULL, COLORED, false, -1));
			child->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::sphere(0.2f, Vector3::zero(), true), &manParam);
			child->getCollisionObject()->addCollisionListener(this);
			//	Game::getPhysicsController()->createFixedConstraint(((PhysicsRigidBody*)handleNodeList[i]->getCollisionObject()), ((PhysicsRigidBody*)child->getCollisionObject()));
			temp.push_back(child);
		}

		manNodes.push_back(temp);

	}

	int player1Pegs[4] = { 2, 4, 6, 7 };
	int player2Pegs[4] = { 0, 1, 3, 5 };


	// load up men
	std::vector<Node*> manNodeList;
	_scene->findNodes("Man", manNodeList, false, false);
	for (unsigned int i = 0; i < manNodeList.size(); ++i)
		((gameplay::Model*)manNodeList[i]->getDrawable())->setMaterial(buildMaterial(_scene, NULL, COLORED, false, -1));

	for (int i = 0; i < 4; ++i)
		p2Handles.push_back(_scene->findNode(std::string("Pole" + to_string(player1Pegs[i] + 1)).c_str()));
	for (int i = 0; i < 4; ++i)
		p1Handles.push_back(_scene->findNode(std::string("Pole" + to_string(player2Pegs[i] + 1)).c_str()));

	// now load up the room
	Node* room = _scene->findNode("Room");
	((gameplay::Model*)room->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(loader.getString("ROOM_TEXTURE").c_str()), Entity::TEXTURED_SHADELESS, false, -1));
	Node* windows = _scene->findNode("Window");
	((gameplay::Model*)windows->getDrawable())->setMaterial(buildMaterial(_scene, NULL, Entity::COLORED, false, -1));
	Node* outside = _scene->findNode("Sky");
	((gameplay::Model*)outside->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(loader.getString("OUTSIDE_TEXTURE").c_str()), Entity::TEXTURED_SHADELESS, false, -1));

	powerUp = new PowerUp(_scene, this, ballNode);



}

Table::~Table()
{
//	SAFE_DELETE(shadowBatch);
//	font->release();
//	delete(stateMachine);
//	for (unsigned int i = 0; i < soundEffects.size(); ++i)
//	if (soundEffects[i])
//		soundEffects[i]->release();
//	delete(powerUp);
	
//	_scene->visit(this, &Table::removePhysObjects);

}

bool Table::removePhysObjects(Node* node)
{
	PhysicsCollisionObject *object = node->getCollisionObject();
	
	if (!object)return true;

//	object->removeCollisionListener(this);
//	delete(object);
	object->setEnabled(false);
//	node->release();
	return true;
}

void Table::Clear()
{
	p1Score = p2Score = 0;
	showScore = false;
	if (powerUp)
		powerUp->Clear();
}

int Table::getOwner(Node* man)
{
	Node* pole = man->getParent();

	for (unsigned int i = 0; i < p1Handles.size(); ++i)
	if (p1Handles[i] == pole)
		return 1;

	for (unsigned int i = 0; i < p2Handles.size(); ++i)
	if (p2Handles[i] == pole)
		return 2;

	return -1;  // something went terribly wrong
}

Node* Table::getClosestMan(int character)
{
	float shortestDistance = 100000.0f;
	Node* closestMan = NULL;

	std::vector<Node*> handles = getHandles(character);
	for (unsigned int i = 0; i < handles.size(); ++i){
		Node* man = handles[i]->getFirstChild();
		while (true){
			float distance = (man->getTranslationWorld() - ballNode->getTranslationWorld()).length();
			if (distance < shortestDistance){
				shortestDistance = distance;
				closestMan = man;
			}
			man = man->getNextSibling();
			if (!man)
				break;
		}
	}

	return closestMan;
}

void Table::checkScriptCallbacks(const float& elapsedTime)
{
	// check for power-up received

	// check for near the goal
	const float NEAR_GOAL_LENGTH = 5.5f;
	const float GOAL_WIDTH_HALF = 1.2f;
	const float SLOW_SPEED = 1.0f;
	Vector3 bPos = ballNode->getTranslationWorld();
	// right
	if (bPos.x > NEAR_GOAL_LENGTH
		&& bPos.y < GOAL_WIDTH_HALF
		&& bPos.y > -GOAL_WIDTH_HALF
		&& ((PhysicsRigidBody*)ballNode->getCollisionObject())->getLinearVelocity().length() < SLOW_SPEED){
		if (!nearGoal){
			nearGoal = true;
			// send the signal
			lua_getglobal(L, "notifyNearGoal");
			lua_pushnumber(L, 1); // player ones' goal
			try{
				
				lua_call(L, 1, 0);
			}
			catch (std::exception &e){
				std::string error = e.what();
				GP_ERROR(error.c_str());
			}
			
			lua_pop(L, 0);
			
		}
	} // left
	else if (bPos.x < -NEAR_GOAL_LENGTH
		&& bPos.y < GOAL_WIDTH_HALF
		&& bPos.y > -GOAL_WIDTH_HALF
		&& ((PhysicsRigidBody*)ballNode->getCollisionObject())->getLinearVelocity().length() < SLOW_SPEED){
		if (!nearGoal){
			nearGoal = true;
			// send the signal
			lua_getglobal(L, "notifyNearGoal");
			lua_pushnumber(L, 2); // player two's goal
			try{
				lua_call(L, 1, 0);
			}
			catch (std::exception &e){
				std::string error = e.what();
				GP_ERROR(error.c_str());
			}
			lua_pop(L, 0);
		}
	}
	else{
		nearGoal = false;
	}

	// check for time of round ran
	const float LONG_ASS_TIME = 60000; // 60 seconds

	if (roundTimer > LONG_ASS_TIME){
		roundTimer = 0;
		lua_getglobal(L, "notifyLongTime");
//		lua_call(L, 0, 0);  // takes none, returns none

	}
}