#pragma once
#include "../Screen.h"


using namespace gameplay;

class MyCamera;

class MainMenu : public Screen
{
public:
	MainMenu(Vector2 screenSize, Game* game);
	void Update(const float& elapsedTime);
	void Render();
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	~MainMenu();
private:
	Scene* _scene = NULL;
	MyCamera* camera = NULL;

	Node* wall;
	Node* board;
	Node* campaign;
	Node* quickGame;
	Node* about;

	static bool luaBinded;
};