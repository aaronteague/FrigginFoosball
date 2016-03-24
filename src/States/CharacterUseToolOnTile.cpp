#include "CharacterUseToolOnTile.h"

#include "CharacterInteractWithItem.h"

#include "../Item.h"
#include "../FSM.h"
#include "../Character.h"
#include "../Tile.h"
#include "../PlayField.h"

CharacterUseToolOnTile::CharacterUseToolOnTile()
{
	targetTile = NULL;
}

void CharacterUseToolOnTile::Enter(Entity* character, FSM* fsm, float& elapsedTime)
{
	Character* self = (Character*)character;
	if (targetTile == NULL)
		targetTile = (Tile*)fsm->extraInfo;

	Vector2 pos(targetTile->position.x + 0.5f, targetTile->position.y + 0.5f);
	
	Vector2 difference = pos - self->GetPosition();
	float distance = difference.length();
	
	difference.normalize();
	Vector2 direction = difference;
	self->SetDirection(direction);
	self->SetSpeed(5);

	if (distance < 1){
		self->SetSpeed(0);
		std::string toolName = self->equipedItem->GetName();
		if (toolName.compare("Shovel") == 0)
			self->play(Character::DIG);
		else if (toolName.compare("Hoe") == 0)
			self->play(Character::TILL);
		else if (toolName.compare("Water_Can") == 0)
			self->play(Character::WATER);
		fsm->proceed();
	}
}
void CharacterUseToolOnTile::Execute(Entity* character, FSM* fsm, float& elapsedTime)
{
	Character* self = (Character*)character;

	if (!self->currentAnimClip->isPlaying()){
		

		std::string toolName = self->equipedItem->GetName();
		if (toolName.compare("Shovel") == 0)
			self->playField->digTile(targetTile);
		else if (toolName.compare("Hoe") == 0)
			self->playField->tillTile(targetTile);
		else if (toolName.compare("Water_Can") == 0)
			self->playField->waterTile(targetTile);

		fsm->proceed();
	}
}
void CharacterUseToolOnTile::Exit(Entity* character, FSM* fsm, float& elapsedTime)
{
	Character* self = (Character*)character;
	self->currentAnimClip = self->animations[Character::IDLE];
	fsm->proceed();
}
bool CharacterUseToolOnTile::OnMessage(Entity* character, Telegram& telegram, FSM* fsm)
{
	Character* self = (Character*)character;

//	switch (telegram.msg){
	
//	}

	return true;
}