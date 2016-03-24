#include "PlayerFollowPoint.h"

#include "../Player.h"
#include "../FSM.h"
#include "../PlayField.h"
#include "../Menus/Inventory.h"


// other states
#include "PlayerControl.h"


PlayerFollowPoint::PlayerFollowPoint(){

}

void PlayerFollowPoint::Enter(Entity* player, FSM* fsm, float& elapsedTime)
{
	
	Player* self = (Player*)player;
	point = (Vector2*)fsm->extraInfo;

	if (self->turnTo(*point))
		fsm->proceed();
}
void PlayerFollowPoint::Execute(Entity* player, FSM* fsm, float& elapsedTime)
{
	Player* self = (Player*)player;
	Vector2 direction(*point);
	direction.subtract(self->GetPosition());
	float distance = direction.length();
	direction.normalize();
	self->turnTo(*point);
//	player->SetDirection(direction);
	const float lengthForMaxSpeed = 7.0f;
	const float maxSpeed = 5.0f;
	const float runSpeed = 3.0f;
	const float stopThreshold = 0.2f;
	if (distance > lengthForMaxSpeed)
		distance = lengthForMaxSpeed;
	if (distance > stopThreshold){
		float speed = distance / lengthForMaxSpeed * maxSpeed;
	//	if (speed > runSpeed)
	//		self->play(Player::RUN);
	//	else
	//		self->play(Player::WALK);
		self->SetSpeed(speed);
	}
	else{
		self->SetSpeed(0);
	//	self->play(Player::IDLE);
	}
}
void PlayerFollowPoint::Exit(Entity* player, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}
bool PlayerFollowPoint::OnMessage(Entity* player, Telegram& telegram, FSM* fsm){
	Player* self = (Player*)player;
	//fsm->extraInfo = telegram.extraInfo;

	switch (telegram.msg)
	{
	case FSM::FOLLOW_POINT:
		
		break;
	case FSM::STOP_FOLLOWING_POINT:
	//	fsm->proceed(); // revert back to player control
		fsm->SetCurrentState(new PlayerControl());
		break;
	case FSM::TAP_HOLD_ITEM:
	//	fsm->proceed();
		fsm->SetCurrentState(new PlayerControl());
		self->playField->setFocusedMenu((Menu*)self->GetInventory());
		self->GetInventory()->holdItem((Item*)telegram.extraInfo);
		break;
	case FSM::DOUBLE_TAPPED_CHARACTER:
	//int i = 7; // check this out
		break;
	default:
		fsm->proceed(); // revert back to player control if any other messages happen
		
	}
	return true;
}