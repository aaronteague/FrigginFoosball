#include "CharacterGoToTile.h"
#include "../Tile.h"
#include "../FSM.h"
#include "../Character.h"
#include "../PlayField.h"
//#include "../Dependencies\Box2D\Box2D.h"
#include "Box2D/Box2d.h"
// other states
#include "PlayerFollowPoint.h"
#include "NeighborWait.h"

void CharacterGoToTile::Enter(Entity* character, FSM* fsm, float& elapsedTime)
{
	stateTimer = 0;

	Character* self = (Character*)character;
	if (path.size() == 0){
		
		targetTile = (Tile*)fsm->extraInfo;
		Vector2 sPos = self->GetPosition();
		Vector2 ePos = targetTile->position;

		path = self->playField->getPath(sPos.x, sPos.y, ePos.x, ePos.y);


		currentVelocity = path.back();
		currentVelocity *= 10;
	//	self->play(Character::RUN);
	}

	if (self->turnTo(path[path.size() - 1]))
		fsm->proceed();
}

void CharacterGoToTile::Execute(Entity* character, FSM* fsm, float& elapsedTime)
{
	stateTimer += elapsedTime;
	if (stateTimer > maxTimeForState) // if we timeout
		fsm->proceed();

	// let's seek
	Character* self = (Character*)character;

	self->turnTo(path.back());
	self->SetSpeed(7);


	float distance = (path.back() - self->GetPosition()).length();
	if (distance < 0.3){
		if (path.size() > 1)
			path.pop_back();
		else
			fsm->proceed();
	}
	
	// check for stuck
	if (self->GetPosition() == lastPos)
		fsm->proceed();
	lastPos = self->GetPosition();
}

void CharacterGoToTile::Exit(Entity* character, FSM* fsm, float& elapsedTime)
{
	Character* self = (Character*)character;
	self->play(Character::IDLE);
	self->SetSpeed(0);
	fsm->proceed();
}

bool CharacterGoToTile::OnMessage(Entity* character, Telegram& telegram, FSM* fsm)
{
	//fsm->extraInfo = telegram.extraInfo;
	switch (telegram.msg)
	{
	case FSM::FOLLOW_POINT:
		fsm->SetCurrentState(new PlayerFollowPoint(), telegram.extraInfo);
		break;
	case FSM::WAIT:
		fsm->SetCurrentState(new NeighborWait());
		break;
	}
	return true;
}