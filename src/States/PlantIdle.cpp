#include "PlantIdle.h"


#include "../FSM.h"
#include "../Items/Plant.h"
//#include "../Items/Crop.h"
#include "../Character.h"

PlantIdle::PlantIdle()
{

}

void PlantIdle::Enter(Entity* plant, FSM* fsm, float& elapsedTime)
{
//	interactee = (Entity*)fsm->extraInfo;
	fsm->proceed();

}
void PlantIdle::Execute(Entity* plant, FSM* fsm, float& elapsedTime)
{
	Plant* self = (Plant*)plant;
	if (self->currentAnimClip != NULL && !self->currentAnimClip->isPlaying())
	if (self->currentAnimClip == self->animations[Plant::ACTION]){
		
		for (unsigned int i = 0; i < self->attachPoints.size(); ++i)
		if (self->attachPoints[i].crop != NULL){
			cropPackage.push_back(self->attachPoints[i].crop);
			self->attachPoints[i].crop = NULL;
		}
		MessageDispatcher::Instance()->dispatchMessage(0, self, interactee, FSM::TAKE_CROPS, &cropPackage);
		self->playAnimation(self->animations[Plant::IDLE]);
	}
		// send message of items to give to interactee ("Character")

}
void PlantIdle::Exit(Entity* plant, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
bool PlantIdle::OnMessage(Entity* plant, Telegram& telegram, FSM* fsm)
{
	Plant* self = (Plant*)plant;

	switch (telegram.msg){
	case FSM::DO_INTERACTION:
		interactee = telegram.sender;
		if (self->calculateStage(self->age) == self->stageList.size() - 1){
			self->playAnimation(self->animations[Plant::ACTION]);
			// change to "jump and grab later
			animInfo = Character::Animations::HARVEST_TREE;
			MessageDispatcher::Instance()->dispatchMessage(0, self, telegram.sender, FSM::PLAY_ANIMATION, &animInfo);
		}
		break;
	}
	return true;
}