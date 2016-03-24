#pragma once
#include "Entity.h"

using namespace gameplay;

class PlayField;

class Menu : public Entity{
public:
//	virtual bool touchPosition(Vector2 position) = 0;

	virtual void Update(const float& elapsedTime) = 0;
	virtual void Render() = 0;

	virtual void prepSelf() = 0;
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene) = 0;

	virtual bool touchPress(Vector2 position) = 0;
	virtual void touchMove(Vector2 position) = 0;
	virtual void touchRelease(Vector2 position) = 0;

	virtual bool NeedsInitialized();

	void SetPlayField(PlayField* playField){ Menu::playField = playField; }

	
protected:
	std::string textureF;

	SpriteBatch* _menuBatch;
	gameplay::Rectangle menuRec;
	gameplay::Rectangle menuImgRec;
	PlayField* playField;

	Vector2 touchDown;
	Vector2 currentTouchPosition;
	Vector2 screenSize;
	Vector2 scale;

	Menu(const Vector2 &screenSize);
private:
};