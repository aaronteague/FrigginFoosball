#include "Character.h"
#include "FSM.h"
#include "Table.h"
#include "Dependencies\LuaBridge\LuaBridge.h"
#include "LuaLoader.h"

bool Character::luaBinded = false;

Character::Character(Scene* _scene, Table* table)
:_scene(_scene)
, currentAnimSet(NULL), nextAnimSet(NULL)
, table(table)
{
	if (!luaBinded)
		luabridge::getGlobalNamespace(L)
		//	.beginNamespace("character")
		.deriveClass<Character, Entity>("Character")
		.addFunction("SetPosition", &Character::SetPosition)
		.addFunction("WalkToPosition", &Character::WalkToPosition)
		.addFunction("setAnimState", &Character::setAnimState)
		.addFunction("ShowCharacter", &Character::ShowCharacter)
		
		.endClass();
	//	.endNamespace();
		//.beginNamespace("vector")
		//.beginClass<Vector3>("Vector3")
		//.addConstructor<void(*)(float, float, float)>()
		//.endClass();
	//	.endNamespace();
	
	luaBinded = true;

	intendedPos.set(-1, -1, -1);
}

void Character::Update(const float& elapsedTime){
	stateMachine->Update((float&)elapsedTime, true);
//	checkAndChangeAnim();
//	updateAnimation();
}

void Character::Render(){
	// drawing is taken care of by main
}

bool Character::belongsToCharacter(int id, Node* bar)
{
	for (unsigned int i = 0; i < ownedPoles.size(); ++i)
	if (ownedPoles[i] == bar)
		return true;

	return false;
}

void Character::changeAnimSet(int pointA, int pointB)
{
	AnimSet* foundSet = NULL;

	for (unsigned int i = 0; i < animSetList.size(); ++i)
	if (pointA == animSetList[i].pointA && pointB == animSetList[i].pointB
		|| pointA == animSetList[i].pointB && pointB == animSetList[i].pointA){
		foundSet = &animSetList[i];
		break;
	}

	// if one is found change it
	if (foundSet){
		if (currentAnimSet == NULL){
			currentAnimSet = foundSet;
			currentAnimSet->enter->play();
			currentAnimSet->playingClip = currentAnimSet->enter;
		}
		else if (foundSet != NULL && currentAnimSet != NULL && nextAnimSet != currentAnimSet)
			nextAnimSet = foundSet;
	}
}
void Character::changeAnimSet(AnimSet* nextAnimState)
{
	if (currentAnimSet == NULL){
		currentAnimSet = nextAnimState;
		if (currentAnimSet->enter)
			currentAnimSet->playingClip = currentAnimSet->enter;
		else if (currentAnimSet->execute)
			currentAnimSet->playingClip = currentAnimSet->execute;
		else
			GP_ERROR("Problem, animState is null on both accounts");

		// change samplers and play
		Model* model = (Model*)node->getDrawable();
		model->getMaterial(EYE_PART)->getParameter("u_diffuseTexture")->setSampler(currentAnimSet->eyes);
		if (!jaw)
			model->getMaterial(MOUTH_PART)->getParameter("u_diffuseTexture")->setSampler(currentAnimSet->mouth);	
		currentAnimSet->playingClip->play();
	}
	else
		Character::nextAnimSet = nextAnimState;
}

void Character::updateAnimation()
{
	if (currentAnimSet == NULL)
		return;

	if (currentAnimSet->playingClip == currentAnimSet->enter
		&& !currentAnimSet->playingClip->isPlaying()){  // on enter and stopped playing
		currentAnimSet->playingClip->stop();

		if (currentAnimSet->execute == NULL || currentAnimSet->exit == NULL){// this means it was a single animation set
			currentAnimSet->playingClip = NULL;

			currentAnimSet = nextAnimSet; // pass new set

			if (currentAnimSet != NULL){// if passed set is not NULL
				currentAnimSet->enter->play();
				currentAnimSet->playingClip = currentAnimSet->enter;
			}
		}
		else{
			currentAnimSet->execute->play();
			currentAnimSet->playingClip = currentAnimSet->execute;
			nextAnimSet = NULL;
		}
	}
	else if (currentAnimSet->playingClip == currentAnimSet->execute
		&& nextAnimSet != NULL){  // on execute and ready for a new state
		currentAnimSet->playingClip->stop();
		if (currentAnimSet->exit != NULL){
			currentAnimSet->exit->play();
			currentAnimSet->playingClip = currentAnimSet->exit;
		}
		else{
			currentAnimSet->playingClip = NULL;
			currentAnimSet = NULL;
			if (nextAnimSet != NULL){
				currentAnimSet = nextAnimSet;
				currentAnimSet->enter->play();
				currentAnimSet->playingClip = currentAnimSet->enter;
			}
		}
		nextAnimSet = NULL;
	}
	else if (currentAnimSet->playingClip == currentAnimSet->exit
		&& !currentAnimSet->playingClip->isPlaying()){// on exit and stopped playing
		currentAnimSet->playingClip->stop();
		currentAnimSet->playingClip = NULL;
		currentAnimSet = nextAnimSet;

		if (currentAnimSet != NULL){// if not passed NULL
			currentAnimSet->enter->play();
			currentAnimSet->playingClip = currentAnimSet->enter;
		}
	}

}

void Character::setupAnimSetList(Animation* _animation, LuaLoader *loader)
{
	using namespace std;

	//struct IndexPair{
	//	int a;
	//	int b;
	//	IndexPair(int pointA, int pointB) :a(pointA), b(pointB){}
	//};

	//vector<IndexPair> pairList{ IndexPair(3, 7), IndexPair(3, 8)
	//	, IndexPair(5, 7), IndexPair(5, 8) };

	// yes i know, it is messy
	//for (unsigned int i = 0; i < pairList.size(); ++i)
	//	animSetList.push_back(
	//	AnimSet(
	//	_animation->getClip(string("To_" + to_string(pairList[i].a) + "_" + to_string(pairList[i].b)).c_str())
	//	, _animation->getClip(string("Action_" + to_string(pairList[i].a) + "_" + to_string(pairList[i].b)).c_str())
	//	, _animation->getClip(string("From_" + to_string(pairList[i].a) + "_" + to_string(pairList[i].b)).c_str())
	//	, pairList[i].a
	//	, pairList[i].b
	//	))

	//	;

	const int NUM_OF_SAMPLERS = 6;
	samplerList.resize(NUM_OF_SAMPLERS);

	
	samplerList[E_NEUTRAL] = Texture::Sampler::create(loader->getString("EYE_NEUTRAL").c_str());
	
	samplerList[E_HAPPY] = Texture::Sampler::create(loader->getString("EYE_HAPPY").c_str());
	
	samplerList[E_ANGRY] = Texture::Sampler::create(loader->getString("EYE_ANGRY").c_str());
	

	if (!jaw){
		samplerList[M_NEUTRAL] = Texture::Sampler::create(loader->getString("MOUTH_NEUTRAL").c_str());
		samplerList[M_HAPPY] = Texture::Sampler::create(loader->getString("MOUTH_HAPPY").c_str());
		samplerList[M_ANGRY] = Texture::Sampler::create(loader->getString("MOUTH_ANGRY").c_str());
	}


	// load other  animStates
	animSetList.push_back(AnimSet(NULL, _animation->getClip("Walk"), NULL, samplerList[M_NEUTRAL], samplerList[E_NEUTRAL]));
	animSetList.push_back(AnimSet(NULL, _animation->getClip("Wave"), NULL, samplerList[M_NEUTRAL], samplerList[E_NEUTRAL]));
	animSetList.push_back(AnimSet(_animation->getClip("Happy"), NULL, NULL, samplerList[M_HAPPY], samplerList[E_HAPPY]));
	animSetList.push_back(AnimSet(_animation->getClip("Angry"), NULL, NULL, samplerList[M_ANGRY], samplerList[E_ANGRY]));
	animSetList.push_back(AnimSet(_animation->getClip("Victory"), NULL, NULL, samplerList[M_HAPPY], samplerList[E_HAPPY]));
	animSetList.push_back(AnimSet(_animation->getClip("Defeat"), NULL, NULL, samplerList[M_ANGRY], samplerList[E_ANGRY]));
	animSetList.push_back(AnimSet(NULL, _animation->getClip("Idle"), NULL, samplerList[M_NEUTRAL], samplerList[E_NEUTRAL]));

	//animSetList.push_back(AnimSet(NULL, _animation->getClip("Walk"), NULL, samplerList[M_NEUTRAL], samplerList[E_NEUTRAL]));
	//animSetList.push_back(AnimSet(NULL, _animation->getClip("Idle"), NULL, samplerList[M_NEUTRAL], samplerList[E_NEUTRAL]));
	//animSetList.push_back(AnimSet(_animation->getClip("From_Drop_Ball"), NULL, NULL, samplerList[M_NEUTRAL], samplerList[E_NEUTRAL]));
	//animSetList.push_back(AnimSet(NULL, _animation->getClip("Come_In"), NULL, samplerList[M_HAPPY], samplerList[E_HAPPY]));
	//animSetList.push_back(AnimSet(_animation->getClip("Defeat"), NULL, NULL, samplerList[M_ANGRY], samplerList[E_ANGRY]));
	//animSetList.push_back(AnimSet(_animation->getClip("Victory"), NULL, NULL, samplerList[M_HAPPY], samplerList[E_HAPPY]));

//	changeAnimSet(&animSetList[0]);

}

void Character::SetPosition(Vector3 newPosition)
{
	node->setTranslation(newPosition);
}

void Character::WalkToPosition(Vector3 pos)
{
	setAnimState("Walk");
//	pausedLua = waitTilFinished;
	targetPos = pos;

	if (pausedLua){
	//	luabridge::LuaRef pause = luabridge::getGlobal(L, "pause");
	//	pause();
	}
}

void Character::checkForLuaResume() // check for walk
{
	if (targetPos.length() > 0.0f){
		// walk along the path
	//	if (!currentAnimSet->playingClip->isPlaying())
		//	setAnimState("Walk");

		Vector3 direction = targetPos - node->getTranslation();
		direction.normalize();

		const float SPEED = 0.1f;
		node->translate(direction * SPEED);

		if ((targetPos - node->getTranslation()).length() < 0.3f) // break walk
		{
			node->setTranslation(targetPos);
			setAnimState("Wave");
			targetPos.set(0, 0, 0);
			if (pausedLua){
			//	pausedLua = false;
			//	luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
			//	resume();
			}
		}
	}
}

void Character::setAnimState(std::string state)
{
	AnimSet* focusedSet = NULL;
	for (unsigned int i = 0; i < animSetList.size(); ++i)
	if (animSetList[i].enter && state.compare(animSetList[i].enter->getId()) == 0
		|| animSetList[i].execute && state.compare(animSetList[i].execute->getId()) == 0)
		focusedSet = &animSetList[i];

	if (focusedSet == NULL)
		GP_ERROR(std::string("sorry, but " + state + " is not an animState ID").c_str());

	if (currentAnimSet){
		currentAnimSet->playingClip->stop();
		currentAnimSet->playingClip = NULL;
	}

	currentAnimSet = focusedSet;
	nextAnimSet = NULL;
		
		if (currentAnimSet->enter)
			currentAnimSet->playingClip = currentAnimSet->enter;
		else if (currentAnimSet->execute)
			currentAnimSet->playingClip = currentAnimSet->execute;
		else
			GP_ERROR("Problem, animState is null on both accounts");

		currentAnimSet->playingClip->play();



}

//void Character::Clear()
//{
//	if (!node)
//		return;
//
//	_scene->removeNode(node);
//	SAFE_RELEASE(node);
//	animSetList.clear();
//	currentAnimSet = NULL;
//	nextAnimSet = NULL;
//}

void Character::ShowCharacter()
{
	_scene->addNode(node);
}

void Character::Clear()
{
	if (node){
	

		ownedPoles.clear();
		allPoles.clear();
		for (unsigned i = 0; i < animSetList.size(); ++i){
			AnimSet* set = &animSetList[i];
			// reference in node already, it will take care of it i think
	//		if (set->enter)set->enter->release();
	//		if (set->execute)set->execute->release();
	//		if (set->exit)set->exit->release();
		}
		animSetList.clear();

		for (unsigned i = 0; i < samplerList.size(); ++i)
//		if (samplerList[i])
//			samplerList[i]->release();
		samplerList.clear();

		if (node->getScene())
			node->getScene()->removeNode(node);
		node->release();
		node = NULL;
		currentAnimSet = NULL;
	}
}