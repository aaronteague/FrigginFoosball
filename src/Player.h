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
private:
	MyCamera* camera;
	Scene* _scene;
};