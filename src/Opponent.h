#pragma once

#include "Character.h"
#include "gameplay.h"

class Table;

using namespace gameplay;

class Opponent : public Character{
	friend class Attack;
public:

	Opponent(Scene* _scene, Table* table);

	void Update(const float& elapsedTime);
	void Load(std::string file);

	void FadeToHands();
	void FadeToCharacter();

private:
	enum FADE_TARGET{
		NEITHER,
		HANDS,
		OPPONENT
	};
	struct Hand
	{
		Node* node = NULL;
		std::vector<Node*> ownedPoles;
		Node* goToTarget = NULL;
		Node* heldPole = NULL;
		AnimationClip* grabAnim = NULL;
		AnimationClip* releaseAnim = NULL;
		AnimationClip* currentAnim = NULL;
	};

	const float HAND_MOVE_SPEED = 0.3f;
	const float HANDLE_Y_FROM_CENTER = 6.0;

	Hand* leftHand = NULL;
	Hand* rightHand = NULL;

	int fadeTarget = NEITHER;
	float handAlpha = 0;
	float charAlpha = 1;

	//Node* leftHand = NULL;
	//Node* rightHand = NULL;
	//
	//std::vector<Node*> leftHandPoles;
	//std::vector<Node*> rightHandPoles;

	//Node* leftHandTarget = NULL;
	//Node* rightHandTarget = NULL;
	//Node* leftHandPole = NULL;
	//Node* rightHandPole = NULL;
	
	

	bool handOnBar(Node* bar);
	void moveHandTowardsBar(Node* bar, float elapsedTime);
	void putHandOnBar(Node* bar);
	
	Vector3 correctedYTrans(Node* pole);
	
	void checkAndChangeAnim();

	int getPoleIndex(Node* hand);
	void updateHand(Hand* hand);
	void updateTransparency();
};