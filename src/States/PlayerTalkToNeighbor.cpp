#include "PlayerTalkToNeighbor.h"

#include "../Player.h"
#include "../FSM.h"
#include "../Neighbor.h"

// other states
#include "PlayerControl.h"

PlayerTalkToNeighbor::PlayerTalkToNeighbor()
{
	targetNeigh = NULL;
}

void PlayerTalkToNeighbor::Enter(Entity* player, FSM* fsm, float& elapsedTime)
{
	Player* self = (Player*)player;

	if (targetNeigh == NULL){
		targetNeigh = (Neighbor*)fsm->extraInfo;
		MessageDispatcher::Instance()->dispatchMessage(0, self, targetNeigh, FSM::WAIT, self);
	}

	Vector2 difference = targetNeigh->GetPosition() - self->GetPosition();

	if (difference.length() < 2){
		bool directed = self->turnTo(targetNeigh->GetPosition());
		self->SetSpeed(0);
		if (directed){
			
			MessageDispatcher::Instance()->dispatchMessage(0, self, targetNeigh, FSM::TALK, self);
			fsm->proceed();
		}
	}
	else{
	//	self->SetDirection(difference.normalize());
		self->turnTo(targetNeigh->GetPosition());
		self->SetSpeed(5);
	}
	
	
}

void PlayerTalkToNeighbor::Execute(Entity* player, FSM* fsm, float& elapsedTime)
{

}

void PlayerTalkToNeighbor::Exit(Entity* player, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}

bool PlayerTalkToNeighbor::OnMessage(Entity* player, Telegram& telegram, FSM* fsm)
{
	Player* self = (Player*)player;
	//fsm->extraInfo = telegram.extraInfo;

	switch (telegram.msg)
	{
	case FSM::DONE_TALKING:
		fsm->SetCurrentState(new PlayerControl());
		break;
	}
	return true;
}