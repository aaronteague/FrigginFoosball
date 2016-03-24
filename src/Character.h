#pragma once

#include "Entity.h"
#include "gameplay.h"

class Table;



class Character : public Entity{
public:
	virtual void Update(const float& elapsedTime);
	void Render();
	void Clear();
	void SetPosition(Vector3 newPosition);
	void WalkToPosition(Vector3 pos, int waitTilFinished);
	void setAnimState(std::string state);
	void ShowCharacter();
	virtual Vector3 getLookPosition(){ return node->getTranslationWorld() + Vector3(0,0,10); }
protected:
	struct AnimSet{
		int pointA, pointB;
		AnimationClip *enter, *execute, *exit;
		AnimationClip *playingClip;
		AnimSet(AnimationClip* enter, AnimationClip* execute = NULL, AnimationClip* exit = NULL, int pointA = -1, int pointB = -1)
			: enter(enter), execute(execute), exit(exit), pointA(pointA), pointB(pointB), playingClip(NULL){}
	};

	Vector3 intendedPos; // let's change this to a tween later

	Scene* _scene = NULL;
	std::vector<Node*> ownedPoles;
	std::vector<Node*> allPoles;
	Table* table;

	AnimSet* currentAnimSet = NULL;
	AnimSet* nextAnimSet = NULL;

	std::vector<AnimSet> animSetList;

	bool belongsToCharacter(int id, Node* bar);
	

	void setupAnimSetList(Animation* _animation);
	void changeAnimSet(int pointA, int pointB);
	void changeAnimSet(AnimSet* nextAnimState);

	void updateAnimation();

	void checkForLuaResume();

	Character(Scene* _scene, Table* table);
private:
	static bool luaBinded;
	bool pausedLua = false;
	Vector3 targetPos;
};