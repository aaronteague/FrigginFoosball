#include "CharacterInteractWithItem.h"

#include "../Item.h"
#include "../FSM.h"
#include "../Character.h"
#include "../Menus/Inventory.h"

#include "PlayerControl.h"

CharacterInteractWithItem::CharacterInteractWithItem()
{
	item = NULL;
	backup = false;
}

void CharacterInteractWithItem::Enter(Entity* character, FSM* fsm, float& elapsedTime)
{
	Character* self = (Character*)character;
	if (item == NULL)
		item = (Item*)fsm->extraInfo;

	if (!item->inInteractZone(self->GetPosition()))
	{
		Vector2 direction = item->getInteractPoint() - self->GetPosition();
		direction.normalize();

		self->SetDirection(direction);
		self->SetSpeed(5);
	}
	else{
		self->SetSpeed(0);
 		bool directed = self->turnTo(item->getPointTowards());
		if (directed){
			
			MessageDispatcher::Instance()->dispatchMessage(0, self, item, FSM::DO_INTERACTION, self);
			fsm->proceed();
		}
	}


	
}
void CharacterInteractWithItem::Execute(Entity* character, FSM* fsm, float& elapsedTime)
{
	Character* self = (Character*)character;
	
	if (self->currentAnimClip == NULL || self->currentAnimClip == self->animations[Character::IDLE])
		fsm->proceed();
	else if (!self->currentAnimClip->isPlaying())
		fsm->proceed();

}
void CharacterInteractWithItem::Exit(Entity* character, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
bool CharacterInteractWithItem::OnMessage(Entity* character, Telegram& telegram, FSM* fsm)
{
	Character* self = (Character*)character;

	switch (telegram.msg){
	case FSM::PLAY_ANIMATION:{
		Character::Animations *animation = (Character::Animations*)telegram.extraInfo;
		self->play(*animation);
		
		}
		break;
	case FSM::TAKE_CROPS:{
		std::vector<Item*> cropPack = *(std::vector<Item*>*)telegram.extraInfo;
		for (unsigned int i = 0; i < cropPack.size(); ++i)
			self->inventory->addItem(cropPack[i]->GetIcon());
	//	fsm->SetCurrentState(new PlayerControl());
		}
		break;
	case FSM::TAKE_ROOM:
		self->desiredField = (PlayField*)telegram.extraInfo;		
		break;
	case FSM::BACK_UP:
		self->SetDirection(Vector2(0, -1));
		self->SetSpeed(3);
		backup = true;
		break;
	case FSM::COME_FORWARD:
		self->SetDirection(Vector2(0, 1));
		self->SetSpeed(3);
		break;
	case FSM::ROOM_READY:
		self->readyToTransition = true;
		fsm->proceed();
		break;
	}

	return true;
}