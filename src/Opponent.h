#pragma once

#include "Character.h"
#include "gameplay.h"

class Table;

using namespace gameplay;

class Opponent : public Character{
	friend class Attack;
	friend class OppAttack;
public:

	Opponent(Scene* _scene, Table* table);

	void Update(const float& elapsedTime);
	void Load(std::string file);

	void FadeToHands();
	void FadeToCharacter();
	void SetDifficulty(float difficulty);
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
		~Hand()
		{
			if (currentAnim->isPlaying())
				currentAnim->stop();
			if (grabAnim)grabAnim->release();
			if (releaseAnim)releaseAnim->release();
			if (node->getScene())node->getScene()->removeNode(node);
			node->setEnabled(false);
	//		node->release();
			node = NULL; // memory leak....
			ownedPoles.clear();

		}
	};

	const float HAND_MOVE_SPEED = 0.3f;
	const float HANDLE_Y_FROM_CENTER = 6.0;

	Hand* leftHand = NULL;
	Hand* rightHand = NULL;

	int fadeTarget = NEITHER;
	float handAlpha = 0;
	float charAlpha = 1;
	float difficulty = 1.0f;

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
	void putHandOnBar(Node* bar, bool immediate = false);
	
	
	Vector3 correctedYTrans(Node* pole);
	
	void checkAndChangeAnim();

	int getPoleIndex(Node* hand);
	void updateHand(Hand* hand);
	void updateTransparency();
};