#pragma once

#include "Character.h"
#include "gameplay.h"

class Table;
class MyCamera;

class Player : public Character{
public:
	Player(Scene *_scene, Table* table, MyCamera* camera);
	void Update(const float& elapsedTime);
	void Load(std::string file);

	virtual void FadeToCharacter();
	void MakeInvisible(){ if (node->getScene())node->getScene()->removeNode(node); }
private:
	MyCamera* camera;
	Scene* _scene;
};