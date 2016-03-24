#include "PlayerControl.h"

#include "../Player.h"
#include "../FSM.h"
#include "../PlayField.h"
#include "../Menus/Inventory.h"
//#include "Neighbor.h"
#include "../Tile.h"
#include "../Item.h"


// other states
#include "CharacterGoToTile.h"
#include "PlayerFollowPoint.h"
#include "CharacterGoToTile.h"
#include "PlayerTalkToNeighbor.h"
#include "CharacterInteractWithItem.h"
#include "CharacterUseToolOnTile.h"

//////////////// Player Control State /////////////////////////////////////

//PlayerControl* playerControl = NULL;

PlayerControl::PlayerControl()
{
	leftPressed = false;
	rightPressed = false;
	upPressed = false;
	downPressed = false;
}

//PlayerControl* PlayerControl::Instance(){
//	if (!playerControl)
//	{
//		playerControl = new PlayerControl();
//	}
//	return playerControl;
//}


void PlayerControl::Enter(Entity* player, FSM* fsm, float& elapsedTime)
{	
	Player* self = (Player*)player;

	self->play(self->IDLE);

	fsm->proceed();
}
void PlayerControl::Execute(Entity* player, FSM* fsm, float& elapsedTime)
{
	Player* self = (Player*)player;
	float x = 0;
	float y = 0;
	x += rightPressed;
	x -= leftPressed;
	y += upPressed;
	y -= downPressed;

	Vector2 direction(x, y);

	if (direction.length() > 0){
		direction.normalize();
		self->SetDirection(direction);
		self->SetSpeed(5);
	//	self->play(self->RUN);
		// test
		Vector3 trans = dynamic_cast<Model*>(self->bodyNode->getDrawable())->getSkin()->getRootJoint()->getTranslationWorld();
		int i = 7;
	}
	else{
		self->SetSpeed(0);
//		self->play(self->IDLE);
	}

	
}
void PlayerControl::Exit(Entity* player, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
bool PlayerControl::OnMessage(Entity* player, Telegram& telegram, FSM* fsm){
	Player* self = (Player*)player;
	//fsm->extraInfo = telegram.extraInfo;
	
	switch (telegram.msg)
	{
	case FSM::UP_PRESSED:
		
		upPressed = true;
		break;
	case FSM::UP_RELEASED:
		upPressed = false;
		break;
	case FSM::DOWN_PRESSED:
		downPressed = true;
		break;
	case FSM::DOWN_RELEASED:
		downPressed = false;
		break;
	case FSM::LEFT_PRESSED:
		leftPressed = true;
		break;
	case FSM::LEFT_RELEASED:
		leftPressed = false;
		break;
	case FSM::RIGHT_PRESSED:
		rightPressed = true;
		break;
	case FSM::RIGHT_RELEASED:
		rightPressed = false;
		break;

	case FSM::FOLLOW_POINT:
	//	fsm->proceed();
		
		fsm->SetCurrentState(new PlayerFollowPoint(), telegram.extraInfo);
		break;
	case FSM::DOUBLE_TAPPED_TILE:
	//	fsm->proceed();
		if (self->equipedItem != NULL && self->equipedItem->GetType().compare("Tool") == 0)
			fsm->SetCurrentState(new CharacterUseToolOnTile(), telegram.extraInfo);
		else
			fsm->SetCurrentState(new CharacterGoToTile(), telegram.extraInfo);
		break;
	case FSM::TAP_HOLD_ITEM:
		
	//	player->playField->setFocusedMenu((Menu*)player->GetInventory());
		self->GetInventory()->holdItem((Item*)telegram.extraInfo);
		break;
	case FSM::DOUBLE_TAPPED_CHARACTER:
		// let's see if this works
		if (telegram.extraInfo != self){  // if didn't click on myself
			
			// find the tile right in front of neighbor
			Character* neigh = (Character*)telegram.extraInfo;
			// next two to check for garbage
			Node* test = neigh->GetNode();
			test->getId();
			//////////////////////////////
			Vector2 pos = neigh->GetPosition();
			Vector2 direction = (self->GetPosition() - neigh->GetPosition());
			direction.normalize();
			Vector2 newPoint = pos + direction * 2;
			Tile* tile = self->playField->getTilePtr(newPoint.x, newPoint.y);
			
		//	fsm->SetCurrentState(new CharacterGoToTile(), (void*)tile);
			fsm->SetCurrentState(new PlayerTalkToNeighbor(), telegram.extraInfo);
		}
		break;
	case FSM::DOUBLE_TAPPED_ITEM:
	{
		Item* item = (Item*)telegram.extraInfo;

		if (item->GetType().compare("Plant") == 0 && self->equipedItem != NULL && self->equipedItem->GetName().compare("Water_Can") == 0){
			Tile *tile = self->playField->getTilePtr(item->GetPosition().x, item->GetPosition().y);
			fsm->SetCurrentState(new CharacterUseToolOnTile(), tile);
		}
		else
			fsm->SetCurrentState(new CharacterInteractWithItem(), item);
	}
		break;
	case FSM::PLAY_ANIMATION:{
			Player::Animations *anim = (Player::Animations*)telegram.extraInfo;
			self->play(*anim);
		}
		break;
	case FSM::TAKE_ROOM:
		self->desiredField = (PlayField*)telegram.extraInfo;
		break;
	}
	return true;
}


