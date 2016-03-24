#include "Dialogue.h"
#include "../FSM.h"

// static variables to make things work smoothly
gameplay::SpriteBatch* Dialogue::_staticBatch = NULL;
gameplay::Font* Dialogue::_font = NULL;
gameplay::Rectangle* Dialogue::_dialogueBox = NULL;
gameplay::Rectangle* Dialogue::_innerDialogueBox = NULL;
gameplay::Rectangle* Dialogue::_dialogueImgBox = NULL;
gameplay::Rectangle* Dialogue::_answerImgBox = NULL;


Dialogue::Dialogue(const Vector2 &screenSize, std::string dialogueFile)
	:Menu(screenSize)
{
	// the more tweekable values here
	textureF = "res/Menu/Dialogue.png";
	Dialogue::dialogueFile = dialogueFile;
	if (_dialogueImgBox == NULL)
		_dialogueImgBox = new gameplay::Rectangle(0, 1317, 2048, 731);
	if (_answerImgBox == NULL)
		_answerImgBox = new gameplay::Rectangle(1101, 1029, 947, 288);

	int width = 600 * scale.y;
	int height = 200 * scale.y;
	int x = screenSize.x / 2 - width / 2;
	int y = screenSize.y - height;
	if (_dialogueBox == NULL)
		_dialogueBox = new gameplay::Rectangle(x, y, width, height);
	//if (_innerDialogueBox == NULL)
	//	_innerDialogueBox = new gameplay::Rectangle
	//	(_dialogueBox->x + 10 * scale.x
	//	, _dialogueBox->y + 10 * scale.x
	//	, _dialogueBox->width - 20 * scale.x
	//	, _dialogueBox->height - 20 * scale.x);
	if (_innerDialogueBox == NULL)
		_innerDialogueBox = new gameplay::Rectangle
		((int)(_dialogueBox->x + 13 * scale.x)
		, (int)(_dialogueBox->y + 13 * scale.x)
		, (int)(_dialogueBox->width - 13 * scale.x)
		, (int)(_dialogueBox->height - 13 * scale.x));

	// work on this some
	const int MAX_NUM_OF_ANSWERS = 7;
	for (int i = 0; i < MAX_NUM_OF_ANSWERS; ++i)
		answerBox.push_back(gameplay::Rectangle
		(x + width - 200 * scale.y
		, _dialogueBox->y - (1+i) * 50* scale.y
		, 200 * scale.y
		, 50 * scale.y));

	proceed = false;
	fastForward = false;
	letterTimer = 0;
	dialogueFileLoaded = false;
	currentMood = NORMAL;
}

void Dialogue::Update(const float& elapsedTime)
{
	letterTimer += elapsedTime + elapsedTime * fastForward;

	//if (currentText.size() >= dialogueGoal.size() && answers.size() > 0){

	//}
	float slowDown = 3.0f;
	if (letterTimer > textSpeed * slowDown && currentText.size() < dialogueGoal.size())
		currentText.push_back(dialogueGoal[currentText.size()]);
		
	



	if (emotions.size() > 0 && currentText.size() >= emotions[0].letterToLaunch)
		setCurrentMood(emotions);
}
void Dialogue::Render()
{
	_staticBatch->start();
	// draw them answers
	if (currentText.size() == dialogueGoal.size())
	for (unsigned int i = 0; i < answers.size(); ++i)
		_staticBatch->draw(answerBox[i], *_answerImgBox);
	_staticBatch->draw(*_dialogueBox, *_dialogueImgBox, Vector4(1,1,1,0.7f));
	_staticBatch->finish();

	_font->start();
	if (currentText.size() == dialogueGoal.size())
	for (unsigned int i = 0; i < answers.size(); ++i)
		_font->drawText(answers[i].c_str(), answerBox[i], Vector4(0, 0, 0, 1), _font->getSize() * scale.y * textSize, Font::Justify::ALIGN_HCENTER);
	_font->drawText((currentText).c_str(), *_innerDialogueBox, gameplay::Vector4(0,0,0, 1), _font->getSize() * scale.y * textSize);	
	_font->finish();
}

void Dialogue::prepSelf()
{
	sendToLists(textureF, "none", "none", this);
}
void Dialogue::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	if (_staticBatch == NULL)
		_staticBatch = SpriteBatch::create(tex);
	if (_font == NULL)
		_font = Font::create("res/Menu/dialogue_font.gpb");
}

bool Dialogue::touchPress(Vector2 position)
{
//	int test1 = currentText.size();
//	int test2 = dialogueGoal.size();
	if (currentText.size() >= dialogueGoal.size()){
		proceed = true;

		for (unsigned int i = 0; i < answers.size(); ++i)
			if (answerBox[i].contains(position.x, position.y))
			{
				focusedAnswer = answers[i];
				break;
			}	
	}
	else
		fastForward = true;

	// we will return true by default so it blocks
	// lower menus from having chances to move
	return true;
}
void Dialogue::touchMove(Vector2 position){} // just a formality my dear

void Dialogue::touchRelease(Vector2 position)
{
	fastForward = false;
}

bool Dialogue::NeedsInitialized()
{
	// returns if _menuBatch needs initialized
	return _staticBatch == NULL;
}

void Dialogue::say(float fontSize, float textSpeed, std::string dialogue)
{
	currentText.clear();
	answers.clear();
	focusedAnswer.clear();
	Dialogue::dialogueGoal = dialogue;
	Dialogue::textSize = fontSize;
	Dialogue::textSpeed = textSpeed;

	letterTimer = 0;
	proceed = false;
}
void Dialogue::say(float fontSize, float textSpeed, std::string dialogue, std::vector<std::string> answers)
{
	say(fontSize, textSpeed, dialogue);
	Dialogue::answers = answers;
}

bool Dialogue::readyToProceed(){ 
	bool gotAnswerIfNeeded = false;
	if (answers.size() == 0 || focusedAnswer.size() > 0)
		gotAnswerIfNeeded = true;
	return currentText.size() == dialogueGoal.size() && proceed && gotAnswerIfNeeded; 
}

void Dialogue::setEmotion(int emotion, int letterToSet)
{
	Emotion emo;
	emo.moodToSet = emotion;
	emo.letterToLaunch = letterToSet;
	emotions.push_back(emo);
}

void Dialogue::setCurrentMood(std::vector<Emotion> &emotionSet)
{
	//std::string emo = emotionSet[0].moodToSet;
	//if (emo.compare("Happy") == 0)
	//	currentMood = HAPPY;
	//else if (emo.compare("Angry") == 0)
	//	currentMood = ANGRY;
	//else if (emo.compare("Sad") == 0)
	//	currentMood = SAD;
	//else if (emo.compare("Indifferent") == 0)
	//	currentMood = INDIFFERENT;
	//else if (emo.compare("Shy") == 0)
	//	currentMood = SHY;
	//else if (emo.compare("Surprised") == 0)
	//	currentMood = SURPRISED;
	//else if (emo.compare("Normal") == 0)
	//	currentMood = NORMAL;
	currentMood = emotionSet[0].moodToSet;

	emotions.erase(emotions.begin());
	
}

char Dialogue::getCurrentLetter()
{ 
	if (currentText.size() > 0)
		return currentText[currentText.size() - 1];
	else
		return '\0';
}