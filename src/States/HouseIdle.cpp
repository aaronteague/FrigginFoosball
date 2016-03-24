#include "HouseIdle.h"

#include "../Items/House.h"
#include "../FSM.h"
#include "../Character.h"
#include "../Room.h"

HouseIdle::HouseIdle(){}

void HouseIdle::Enter(Entity* house, FSM* fsm, float& elapsedTime)
{
//	House* self = (House*)house;
//	self->playAnimation(self->animations[0]);
	fsm->proceed();
}
void HouseIdle::Execute(Entity* house, FSM* fsm, float& elapsedTime)
{
	House* self = (House*)house;
	if (self->currentAnimClip != NULL && !self->currentAnimClip->isPlaying()){
		MessageDispatcher::Instance()->dispatchMessage(0, self, interactee, FSM::ROOM_READY, NULL);
		self->currentAnimClip = NULL;
	}
}
void HouseIdle::Exit(Entity* house, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
bool HouseIdle::OnMessage(Entity* house, Telegram& telegram, FSM* fsm)
{
	House* self = (House*)house;

	switch (telegram.msg){
	case FSM::DO_INTERACTION:
		interactee = (Character*)telegram.sender;
		self->playAnimation(self->animations[0]);
		self->openRoom();
		MessageDispatcher::Instance()->dispatchMessage(0, self, interactee, FSM::TAKE_ROOM, self->room);
		MessageDispatcher::Instance()->dispatchMessage(0, self, interactee, FSM::BACK_UP, NULL);
		MessageDispatcher::Instance()->dispatchMessage(500, self, interactee, FSM::COME_FORWARD, NULL);

		// load up inside of house

		// do animation here
		// tell house to change bounds to have space for walk
		// tell character to do opendoor animation and walk forward
		// when player reaches middle area, set to inside
		break;
	}
	return true;
}