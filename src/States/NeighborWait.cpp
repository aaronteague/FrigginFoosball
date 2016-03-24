#include "NeighborWait.h"

#include "../Neighbor.h"
#include "../FSM.h"
#include "../PlayField.h"
#include "../Menus/Dialogue.h"

// other states
#include "NeighborTalkToPlayer.h"

NeighborWait::NeighborWait()
{
	timer = 0;
}

void NeighborWait::Enter(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	Neighbor* self = (Neighbor*)neighbor;
	
	// be sure to reset the timer
	timer = 0;

	fsm->proceed();
}

void NeighborWait::Execute(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	timer += elapsedTime;
	if (timer > WAIT_TIME)
		fsm->proceed(); // stop waiting
}

void NeighborWait::Exit(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	Neighbor* self = (Neighbor*)neighbor;

	fsm->proceed();
}

bool NeighborWait::OnMessage(Entity* neighbor, Telegram& telegram, FSM* fsm)
{
	Neighbor* self = (Neighbor*)neighbor;
	//fsm->extraInfo = telegram.extraInfo;

	switch (telegram.msg)
	{
	case FSM::TALK:
		fsm->SetCurrentState(new NeighborTalkToPlayer(), telegram.extraInfo);
		break;

	}
	return true;
}