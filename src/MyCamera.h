#pragma once

#include "gameplay.h"
#include "Entity.h"

using namespace gameplay;

class Table;
struct lua_State;

extern lua_State *L;

class MyCamera : public Entity{
	friend class CameraFollow; /**<@ref*/
	friend class CameraStage;/**<@ref*/
	friend class CameraOvermap;
public:
	/** constructor*/
	MyCamera(Scene* _scene, float screenWidth, float screenHeight);

	/** converts position from screen space to world space, colliding with ground plane
	@param itemToCheck we are about to remove this parameter
	@param touchPosition the position the finger touches onto the screen*/
	Vector2 screenToFieldCoordinates(Vector2 touchPosition);

	/** checks if player is touching node
	@param x screen touched x-axis
	@param y screen touched y-axis*/
	bool isTouching(Node* node, int x, int y, const Vector3 dimensions);
	Ray* getPickRay(int x, int y);

	/** checks if player is touching node
	@param x screen touched x-axis
	@param y screen touched y-axis*/
	bool isTouching(Node* node, int x, int y, const float radius);

	/** updats the logic*/
	virtual void Update(const float& elapsedTime);

	/** sets the target node to follow*/
//	void SetTarget(Table* target);

	void SetPosition(Vector3 newPosition);
	void LookAt(Entity* target);
	void Follow(Entity* target);
	void SetState(std::string state);
	void PanAround(Entity* target, float speed, float radius);
	void PeekUp(int milliseconds);
	void ZoomIntoTarget(int milliseconds);

	float panSpeed = -1;
	float panRadius = -1;
	float curAngle;

	/** gets the target node that we have been following*/
//	const Node* GetTarget(){ return target; }

	/** gets the node of the camera*/
	Node* GetNode(){ return node; }

	/** just overiding the default*/
	virtual void prepSelf(){}

	/** just overiding the default
	@param node the node to add to game
	@param tex the texture to add to the game
	@param _scene the scene that we could put into*/
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene){}
private:
	const float DEFAULT_HEIGHT = 12.0f;
	const float DEFAULT_Y_OFFSET = -12.77f;
	

//	Node* node;
	Ray pickRay;
	gameplay::Rectangle viewportRec;
	const Table* table;
	Node* followNode = NULL;
	Vector3 followOffset;
	Entity* target;
	Vector3 defaultPos;

	static bool luaBinded;

	virtual void Render(){};
};