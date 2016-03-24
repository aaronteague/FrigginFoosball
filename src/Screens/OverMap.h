#pragma once
#include "../Screen.h"

using namespace gameplay;

class MyCamera;

class OverMap : public Screen
{
public:
	OverMap(Vector2 screenSize, Game* game);
	void Update(const float& elapsedTime);
	void Render();
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	void addLevelFlier(int index, Scene* scene, bool animate = false);

	~OverMap();
private:


	struct Level{
		Vector2 pos;
		std::string image;
		std::string text;
		std::string levelFile;
		Node* node;
		Node* shadow;
		Node* pin;
		AnimationClip* clip = NULL;
		float nodeAlpha;
		float pinAlpha;
		float shadowAlpha;
	};

	Scene* _scene = NULL;
	MyCamera* camera = NULL;


	Font* font = NULL;

	int progress;

	Level* targetAnimate = NULL;

	float touchDownTime;
	Vector2 touchDownPos;


	//	SpriteBatch* overmapBatch = NULL;

	std::vector<Level> levelList;

	static bool luaBinded;
	Vector2 screenSize;
	Game* game;

	bool gestureTapEvent(int x, int y);
};