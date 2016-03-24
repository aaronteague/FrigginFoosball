#include "NeighborIdle.h"

#include "../Neighbor.h"
#include "../FSM.h"
#include "../Neighbor.h"
#include "../PlayField.h"

// other states
#include "CharacterGoToTile.h"
#include "NeighborTalkToPlayer.h"
#include "NeighborWait.h"


NeighborIdle::NeighborIdle(){}

void NeighborIdle::Enter(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	srand(Game::getAbsoluteTime());
	fsm->proceed();
	
}

void NeighborIdle::Execute(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	// we aren't gonna do much yet.  we may make him move around a
	// few tiles later
	Neighbor* self = (Neighbor*)neighbor;
	const int chanceOfMove = 1000;
	const int moveRange = 10;

	if (rand() % chanceOfMove == 1){
		// move somewhere close
		//Vector2 test = self->GetPosition();
		int x = self->GetPosition().x + (rand() % (moveRange * 2) - moveRange);
		int y = self->GetPosition().y + (rand() % (moveRange * 2) - moveRange);
		if (self->playField->isFreeTile(x, y)){
			Tile* tile = self->playField->getTilePtr(x, y);
			
			fsm->SetCurrentState(new CharacterGoToTile(), tile);
		}
	}

}

void NeighborIdle::Exit(Entity* neighbor, FSM* fsm, float& elapsedTime)
{
	fsm->proceed();
}

bool NeighborIdle::OnMessage(Entity* neighbor, Telegram& telegram, FSM* fsm)
{
	Neighbor* self = (Neighbor*)neighbor;
	fsm->extraInfo = telegram.extraInfo;

	switch (telegram.msg)
	{
	case FSM::WAIT:

		fsm->SetCurrentState(new NeighborWait());
		break;
		// more later?
	case FSM::TALK:
		fsm->SetCurrentState(new NeighborTalkToPlayer(), telegram.sender);
		break;
	}
	return true;
}