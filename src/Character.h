#pragma once

#include "Entity.h"
#include "gameplay.h"

class Table;
class LuaLoader;



class Character : public Entity{
public:
	virtual void Update(const float& elapsedTime);
	void Render();
	
	void SetPosition(Vector3 newPosition);
	void WalkToPosition(Vector3 pos);
	void setAnimState(std::string state);
	void ShowCharacter();
	virtual Vector3 getLookPosition(){ return node->getTranslationWorld() + Vector3(0,0,10); }
protected:
	enum MESH_PARTS
	{
		BODY_PART, 
		SHIRT_PART,
		EYE_PART,
		MOUTH_PART
	};
	struct AnimSet{
		int pointA, pointB;
		AnimationClip *enter, *execute, *exit;
		AnimationClip *playingClip;
		Texture::Sampler* mouth;
		Texture::Sampler* eyes;
		AnimSet(AnimationClip* enter, AnimationClip* execute = NULL, AnimationClip* exit = NULL, Texture::Sampler* mouth = NULL, Texture::Sampler* eyes = NULL)
			: enter(enter), execute(execute), exit(exit), eyes(eyes), mouth(mouth), playingClip(NULL){}
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
	

	void setupAnimSetList(Animation* _animation, LuaLoader *loader);
	void changeAnimSet(int pointA, int pointB);
	void changeAnimSet(AnimSet* nextAnimState);

	void updateAnimation();

	void checkForLuaResume();

	Character(Scene* _scene, Table* table);

protected:
	enum SAMPLERS{
		E_NEUTRAL,
		M_NEUTRAL,
		E_HAPPY,
		M_HAPPY,
		E_ANGRY,
		M_ANGRY
	};
	std::string currentFile;
	std::vector<Texture::Sampler*> samplerList;
	bool jaw = false;
	void Clear();
private:
	static bool luaBinded;
	bool pausedLua = false;
	Vector3 targetPos;
};