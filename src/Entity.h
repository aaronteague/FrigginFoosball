#pragma once

#include "gameplay.h"

using namespace gameplay;

class FSM;
struct Telegram;
struct lua_State;

extern lua_State *L;

class Entity{
public:
	enum ShaderType{
		COLORED, /**< colored*/
		TEXTURED, /**< textured*/
		TEXTURED_SPECULAR,
		TEXTURED_SHADELESS,
		TEXTURED_NORMAL, /**< textured and has normal map */
		TEXTURED_ANIMATED, /**< textured and is animated */
		TEXTURED_ANIMATED_NORMAL, /**< textured and animated and is normaled*/
		SHADOW
	};
	
	virtual void Update(const float& elapsedTime) = 0;
	virtual void Render() = 0;

	virtual Vector3 getLookPosition(){ return node->getTranslation(); }

	void HandleMessage(Telegram& msg);

	Node* getNode();

	static Material* buildMaterial(Scene* _scene, Texture* tex, ShaderType shadeType, bool transparent, int numOfJoints);
	static Effect* buildEffect(ShaderType shadeType, const std::string& defines);
	static std::string to_string(int number);
	static void applyNormalMap(Material* material, Texture* tex);
//	static Vector2 screenToFieldCoordinates(int x, int y, Scene* _scene, Vector2 screenSize);
protected:
	FSM* stateMachine;

	static std::vector<Effect*> effectList;
	static bool luaBinded;

	Entity();
	Node* node = NULL;
	bool eitherMatch(const PhysicsRigidBody::CollisionPair& pair, Node* check);
private:

};