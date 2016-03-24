#include "Opponent.h"
#include "FSM.h"
#include "Table.h"
#include "LuaLoader.h"
#include "Table.h"

#include "States\Attack.h"

Opponent::Opponent(Scene* _scene, Table* table)
:Character(_scene, table)
{


	node = NULL;

	// lua binding time
	luabridge::getGlobalNamespace(L)
		.beginNamespace("opponent")
		.deriveClass<Opponent, Character>("Opponent")
		.addFunction("Load", &Opponent::Load)
		.addFunction("FadeToHands", &Opponent::FadeToHands)
		.addFunction("FadeToCharacter", &Opponent::FadeToCharacter)
		
		.endClass()
		.endNamespace();
}

void Opponent::Load(std::string file)
{
	LuaLoader loader(L);
	loader.setFile(file);

	
	if (ownedPoles.size() == 0)
		ownedPoles = table->getHandles(2);

	stateMachine->SetCurrentState(new Attack(table));

	if (allPoles.size() == 0)
		allPoles = table->getHandles();

	leftHand = new Hand();
	rightHand = new Hand();

	if (leftHand->ownedPoles.size() == 0 || rightHand->ownedPoles.size() == 0){
		leftHand->ownedPoles.push_back(_scene->findNode("Pole7"));
		leftHand->ownedPoles.push_back(_scene->findNode("Pole8"));

		rightHand->ownedPoles.push_back(_scene->findNode("Pole3"));
		rightHand->ownedPoles.push_back(_scene->findNode("Pole5"));
	}

	
	// create the "hands"
	//if (!leftHand && !rightHand){
	//	leftHand = Node::create("Left_Hand");
	//	rightHand = Node::create("Right_Hand");
	//	_scene->addNode(leftHand);
	//	_scene->addNode(rightHand);
	//}

	//((Model*)leftHand->getDrawable())->setMaterial(buildMaterial(_scene, NULL, COLORED, false, -1));
	//((Model*)rightHand->getDrawable())->setMaterial(buildMaterial(_scene, NULL, COLORED, false, -1));

	std::string gpbFile = loader.getString("GPB_FILE");
	Bundle* characterBundle = Bundle::create(gpbFile.c_str());
	std::string id = loader.getString("ID");
	node = characterBundle->loadNode(id.c_str());
	std::string textureFile = loader.getString("CHARACTER_TEXTURE");
	Texture* bodyTex = Texture::create(textureFile.c_str());
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(textureFile.c_str(), true), TEXTURED_ANIMATED, true, 40), 0);
	std::string shirtFile = loader.getString("CHARACTER_SHIRT");
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, bodyTex, TEXTURED_ANIMATED, true, 40), 1);
	std::string eyeFile = loader.getString("CHARACTER_EYE");
	((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(eyeFile.c_str(), true), TEXTURED_ANIMATED, true, 40), 2);
	if (!loader.getInt("JAW")){
		std::string mouthFile = loader.getString("CHARACTER_MOUTH");
		((Model*)node->getDrawable())->setMaterial(buildMaterial(_scene, Texture::create(mouthFile.c_str(), true), TEXTURED_ANIMATED, true, 40), 3);
	}

	// hands
	std::string lHandId = loader.getString("LEFT_HAND");
	std::string rHandId = loader.getString("RIGHT_HAND");
	leftHand->node = characterBundle->loadNode(lHandId.c_str());
	((Model*)leftHand->node->getDrawable())->setMaterial(buildMaterial(_scene, bodyTex, TEXTURED_ANIMATED, true, 40));
	((Model*)leftHand->node->getDrawable())->getMaterial()->getParameter("u_modulateAlpha")->setFloat(0);
	rightHand->node = characterBundle->loadNode(rHandId.c_str());
	rightHand->node->scaleX(-1);
	((Model*)rightHand->node->getDrawable())->setMaterial(buildMaterial(_scene, bodyTex, TEXTURED_ANIMATED, true, 40));
	((Model*)rightHand->node->getDrawable())->getMaterial()->getParameter("u_modulateAlpha")->setFloat(0);
	_scene->addNode(leftHand->node);
	_scene->addNode(rightHand->node);
	Animation* _lhAnim = leftHand->node->getAnimation();
	Animation* _rhAnim = rightHand->node->getAnimation();
	_lhAnim->createClips("res/hands.animation");
	_rhAnim->createClips("res/hands.animation");
	leftHand->grabAnim = _lhAnim->getClip("Close_Hand");
	leftHand->releaseAnim = _lhAnim->getClip("Open_Hand");
	rightHand->grabAnim = _rhAnim->getClip("Close_Hand");
	rightHand->releaseAnim = _rhAnim->getClip("Open_Hand");

	Animation* _animation = node->getAnimation("animations");
	_animation->createClips("res/animation.animation");

	setupAnimSetList(_animation);

//	_scene->addNode(node);
	characterBundle->release();
	bodyTex->release();

	// put hands in their place
	putHandOnBar(leftHand->ownedPoles[0]);
	putHandOnBar(rightHand->ownedPoles[0]);

	Vector3 trans = node->getTranslation();
	int i = 7;
}



void Opponent::Update(const float& elapsedTime){
	if (node == NULL)
		return;


	// update left hand
	updateHand(leftHand);

	// update right hand
	updateHand(rightHand);
	
	if (table->getRunning()){
		stateMachine->Update((float&)elapsedTime, true);

		
		//checkAndChangeAnim();
	}

	checkForLuaResume();
	
	updateAnimation();
}



bool Opponent::handOnBar(Node* bar)
{
	if ((leftHand->node->getTranslation() - correctedYTrans(bar)).length() < 0.3f)
		return true;
	else if ((rightHand->node->getTranslation() - correctedYTrans(bar)).length() < 0.3f)
		return true;
	else
		return false;
}
void Opponent::moveHandTowardsBar(Node* bar, float elapsedTime)
{
	Hand* focusedHand;
	//Vector3 focusedVector;

	//Vector3 lhV = leftHand->getTranslation() - correctedYTrans(bar);
	

	if (bar == leftHand->ownedPoles[0] || bar == leftHand->ownedPoles[1])
		focusedHand = leftHand;
	else if (bar == rightHand->ownedPoles[0] || bar == rightHand->ownedPoles[1])
		focusedHand = rightHand;

	Vector3 focusedVector = focusedHand->node->getTranslation() - correctedYTrans(bar);
	//if (lhV.length() < rhV.length()){
	//	focusedHand = leftHand;
	//	focusedVector = lhV;
	//}
	//else{
	//	focusedHand = rightHand;
	//	focusedVector = rhV;
	//}

	focusedVector.normalize();
//	focusedVector *= HAND_MOVE_SPEED * elapsedTime;
	focusedVector *= HAND_MOVE_SPEED;

	
	focusedHand->node->translate(focusedVector * -1);

//	((PhysicsRigidBody*)focusedHand->getCollisionObject())->applyForce(Vector3(1, 0, 0)); // wake up
//	((PhysicsRigidBody*)focusedHand->getCollisionObject())->setLinearVelocity()
}


void Opponent::putHandOnBar(Node* bar)
{
	//if (bar == leftHandPoles[0] || bar == leftHandPoles[1])
	//	leftHand->setTranslation(correctedYTrans(bar));
	//else if (bar == rightHandPoles[0] || bar == rightHandPoles[1])
	//	rightHand->setTranslation(correctedYTrans(bar));

	if (!leftHand->goToTarget && (bar == leftHand->ownedPoles[0] || bar == leftHand->ownedPoles[1])){
		leftHand->goToTarget = bar;
		leftHand->heldPole = NULL;
		leftHand->currentAnim = leftHand->releaseAnim;
		leftHand->currentAnim->play();
	}
	else if (!rightHand->goToTarget && (bar == rightHand->ownedPoles[0] || bar == rightHand->ownedPoles[1])){
		rightHand->goToTarget = bar;
		rightHand->heldPole = NULL;
		rightHand->currentAnim = rightHand->releaseAnim;
		rightHand->currentAnim->play();
	}
}


Vector3 Opponent::correctedYTrans(Node* pole)
{
	return Vector3(pole->getTranslationX(), pole->getTranslationY() + HANDLE_Y_FROM_CENTER, pole->getTranslationZ());
}



void Opponent::checkAndChangeAnim()
{
	//if (currentAnimSet == NULL)
	//	return;

	//AnimSet set = *currentAnimSet;

	//Vector3 poleATrans = allPoles[set.pointA - 1]->getTranslation();
	//Vector3 poleBTrans = allPoles[set.pointB - 1]->getTranslation();
	//Vector3 leftHTrans = leftHand->getTranslation();
	//Vector3 rightHTrans = rightHand->getTranslation();

	//if (poleATrans.distance(leftHTrans) < 0.2f && poleBTrans.distance(rightHTrans) < 0.2f
	//	|| poleATrans.distance(rightHTrans) < 0.2 && poleBTrans.distance(leftHTrans) < 0.2f)
	//	return;  // hand are where you think they should be
	//else

	int lIndex = getPoleIndex(leftHand->node);
	int rIndex = getPoleIndex(rightHand->node);

	if (lIndex == -1 || rIndex == -1)
		return; // in transition

	if (currentAnimSet != NULL)
		if(currentAnimSet->pointA == lIndex && currentAnimSet->pointB == rIndex
		|| currentAnimSet->pointA == rIndex && currentAnimSet->pointB == lIndex)
		return; // matches.  nothing else to do here

	// put in the indexes and method will handle from here
	changeAnimSet(lIndex, rIndex);
}

int Opponent::getPoleIndex(Node* hand)
{
	for (unsigned int i = 0; i < allPoles.size(); ++i){
		float length = (correctedYTrans(allPoles[i]) - hand->getTranslation()).length();
		if ((correctedYTrans(allPoles[i]) - hand->getTranslation()).length() < 0.2f){
			char index = allPoles[i]->getId()[4]; // get's the 5th letter, which is the number
			
			return index - '0';

		}
	}

	return -1;
}

void Opponent::updateHand(Hand* hand)
{
	// update left hand
	if (hand->goToTarget){
		if (hand->currentAnim == hand->releaseAnim && !hand->currentAnim->isPlaying()){
			hand->currentAnim = NULL;
		}
		else if (hand->currentAnim == hand->releaseAnim && hand->currentAnim->isPlaying()){
			// let it play
		}
		else{
			Vector3 dir = correctedYTrans(hand->goToTarget) - hand->node->getTranslationWorld();
			dir.normalize();
			hand->node->translate(dir * HAND_MOVE_SPEED);
			if (handOnBar(hand->goToTarget) && hand->currentAnim != hand->grabAnim){
				hand->currentAnim = hand->grabAnim;
				hand->currentAnim->play();
			} //  && !hand->currentAnim->isPlaying()
			else if (handOnBar(hand->goToTarget) && hand->currentAnim == hand->grabAnim){
				hand->heldPole = hand->goToTarget;
				hand->goToTarget = NULL;
			}
			
		}

	}
	else if (hand->heldPole){
		hand->node->setTranslation(correctedYTrans(hand->heldPole));
	}

	if (fadeTarget != NEITHER)
		updateTransparency();
}

void Opponent::FadeToHands()
{
	fadeTarget = HANDS;
	charAlpha = 1;
	handAlpha = 0;
}
void Opponent::FadeToCharacter()
{
	fadeTarget = OPPONENT;
	handAlpha = 1;
	charAlpha = 0;
}

void Opponent::updateTransparency(){
	const float FADE_AMOUNT = 0.03f;
	
	if (fadeTarget == HANDS){
		handAlpha += FADE_AMOUNT;
		charAlpha -= FADE_AMOUNT;
		if (handAlpha >= 1 && charAlpha <= 0){
			fadeTarget = NEITHER;
			handAlpha = 1;
			charAlpha = 0;
		}
	}
	else if (fadeTarget == OPPONENT){
		charAlpha += FADE_AMOUNT;
		handAlpha -= FADE_AMOUNT;
		if (charAlpha >= 1 && charAlpha <= 0){
			fadeTarget = NEITHER;
			charAlpha = 1;
			handAlpha = 0;
		}
	}
	Model* charModel = (Model*)node->getDrawable();
	for (unsigned int i = 0; i < charModel->getMeshPartCount(); ++i)
		charModel->getMaterial(i)->getParameter("u_modulateAlpha")->setValue(charAlpha);

	((Model*)leftHand->node->getDrawable())->getMaterial()->getParameter("u_modulateAlpha")->setValue(handAlpha);
	((Model*)rightHand->node->getDrawable())->getMaterial()->getParameter("u_modulateAlpha")->setValue(handAlpha);
}