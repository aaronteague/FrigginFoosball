#include "NeighborTalkToPlayer.h"

#include "../Neighbor.h"
#include "../FSM.h"
#include "../PlayField.h"
#include "../Menus/Dialogue.h"
#include "../FSM.h"

// other states
#include "NeighborIdle.h"

bool NeighborTalkToPlayer::luaBinded = false;

NeighborTalkToPlayer::NeighborTalkToPlayer()
{
	if (!luaBinded)
	{
		char* file = FileSystem::readAll("res/luaFiles/initialDialogue.lua");
		int status = luaL_dostring(L, file);

		luabridge::getGlobalNamespace(L)
			.beginNamespace("convo")
			.beginClass<NeighborTalkToPlayer>("Dialogue")
			.addFunction("setemotion", &NeighborTalkToPlayer::setEmotion)
			.addFunction("say", &NeighborTalkToPlayer::say)
			.addFunction("popupquestion", &NeighborTalkToPlayer::popUpQuestion)
			.addFunction("getPlayerName", &NeighborTalkToPlayer::getPlayerName)
			.addFunction("getNeighborName", &NeighborTalkToPlayer::getNeighborName)
			.addFunction("getItemName", &NeighborTalkToPlayer::getItemName)
			.addFunction("finish", &NeighborTalkToPlayer::finish)
			
			.endClass()
			.endNamespace();
		luaBinded = true;
	}
}

void NeighborTalkToPlayer::Enter(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	Neighbor* self = (Neighbor*)neighbor;
	dialogue = self->dialogue;
	targetCharacter = (Character*)fsm->extraInfo;

	if (!dialogue->dialogueFileLoaded){
		char* file = FileSystem::readAll(dialogue->dialogueFile.c_str());
		int status = luaL_dostring(L, file);
		dialogue->dialogueFileLoaded = true;
	}

//	Vector2 difference = targetCharacter->GetPosition() - self->GetPosition();
	bool directed = self->turnTo(targetCharacter->GetPosition());

	if (directed){
		// initiate conversation
		std::string condition("Mood");
		std::string type("Neutral");
		luabridge::setGlobal(L, this, "character");
		luabridge::LuaRef v = luabridge::getGlobal(L, "doDialogue");
		v(self->name, condition, type);

		finished = false;

		self->playField->addMenu(self->dialogue);

		MessageDispatcher::Instance()->dispatchMessage(0, self, (Entity*)self->playField->getCamera(), FSM::ZOOM_IN, self);
		fsm->proceed();
	}
}

void NeighborTalkToPlayer::Execute(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	self = (Neighbor*)neighbor;
	if (text.size() > 0){
		if (answers.size() == 0)
			dialogue->say(fontSize, textSpeed, text);
		else
			dialogue->say(fontSize, textSpeed, text, answers);
		text.clear();
		answers.clear();
	}

	self->updateMouth(dialogue->getCurrentLetter());
	self->setMood(dialogue->getMood());

	// check dialogue status, proceed if ready
	if (dialogue->readyToProceed()){
		if (dialogue->answers.size() == 0){
			luabridge::LuaRef resume = luabridge::getGlobal(L, "resume");
			resume(self->GetName());
		}
		else{
			luabridge::LuaRef answer = luabridge::getGlobal(L, "answer");
			answer(self->GetName(), dialogue->getResponse());
		}
	}

	if (finished)
		fsm->proceed();
}

void NeighborTalkToPlayer::Exit(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	self = (Neighbor*)neighbor;

	self->playField->removeMenu(self->dialogue);

	MessageDispatcher::Instance()->dispatchMessage(0, self, targetCharacter, FSM::DONE_TALKING, self);
	MessageDispatcher::Instance()->dispatchMessage(0, self, (Entity*)self->playField->getCamera(), FSM::ZOOM_OUT, self);
	fsm->SetCurrentState(new NeighborIdle());
	fsm->proceed();
}

bool NeighborTalkToPlayer::OnMessage(Entity* neighbor, Telegram& telegram, FSM* fsm)
{
	self = (Neighbor*)neighbor;
	//fsm->extraInfo = telegram.extraInfo;

	//switch (telegram.msg)
	//{

	//}
	return true;
}

void NeighborTalkToPlayer::say(float fontSize, float textSpeed, bool question, std::string dialogue)
{
	// i dont' thinik we need the "question" variable
	NeighborTalkToPlayer::fontSize = fontSize;
	NeighborTalkToPlayer::textSpeed = textSpeed;
	NeighborTalkToPlayer::text = dialogue;
	
}

void NeighborTalkToPlayer::popUpQuestion(std::string one, std::string two, std::string three, std::string four, std::string five, std::string six, std::string seven)
{
	if (one.compare("null") != 0)
		answers.push_back(one);
	if (two.compare("null") != 0)
		answers.push_back(two);
	if (three.compare("null") != 0)
		answers.push_back(three);
	if (four.compare("null") != 0)
		answers.push_back(four);
	if (five.compare("null") != 0)
		answers.push_back(five);
	if (six.compare("null") != 0)
		answers.push_back(six);
	if (seven.compare("null") != 0)
		answers.push_back(seven);
}

void NeighborTalkToPlayer::finish()
{
	finished = true;
}
void NeighborTalkToPlayer::setEmotion(int emotion, int setToLetter)
{
	dialogue->setEmotion(emotion, setToLetter);
}