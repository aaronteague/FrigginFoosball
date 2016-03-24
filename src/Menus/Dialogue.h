#pragma once

#include "../Menu.h"
#include "Gameplay.h"

using namespace gameplay;

struct Emotion{
	int moodToSet;
	unsigned int letterToLaunch;
};

class Dialogue : public Menu
{
	friend class NeighborTalkToPlayer;
public:
	enum Moods{
		ANGRY,
		HAPPY,
		NORMAL,
		INDIFFERENT,
		SAD,
		SHY,
		SURPRISED
	};
	
	std::vector<Emotion> emotions;

	Dialogue(const Vector2 &screenSize, std::string dialogueFile);

	virtual void Update(const float& elapsedTime);
	virtual void Render();

	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	virtual bool touchPress(Vector2 position);
	virtual void touchMove(Vector2 position);
	virtual void touchRelease(Vector2 position);

	

private:
	// some static privates so we don't use more memory than needed
	static SpriteBatch* _staticBatch;
	static Font* _font;
	static gameplay::Rectangle* _dialogueBox;
	static gameplay::Rectangle* _innerDialogueBox;
	static gameplay::Rectangle* _dialogueImgBox;
	static gameplay::Rectangle* _answerImgBox;

	std::vector<gameplay::Rectangle> answerBox;

	bool dialogueFileLoaded;
	std::string dialogueFile;
	int currentMood;

	bool fastForward;
	bool proceed;
	float textSpeed;
	float textSize;
	std::string dialogueGoal;
	std::string currentText;
	std::vector<std::string> answers;
	std::string focusedAnswer;
	float letterTimer;

	
	void setCurrentMood(std::vector<Emotion> &emotionSet);

	// these functions will be accessible to the states
	char getCurrentLetter();
	void say(float fontSize, float textSpeed, std::string dialogue);
	void say(float fontSize, float textSpeed, std::string dialogue, std::vector<std::string> answers);
	int getMood(){ return currentMood; }
	void setEmotion(int emotion, int letterToSet);

	virtual bool NeedsInitialized();
	bool readyToProceed();
	std::string getResponse(){ return focusedAnswer; };
	
};