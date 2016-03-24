#include "FSM.h"
#include "State.h"
#include "Entity.h"

bool FSM::HandleMessage(Telegram &msg)
{
	currentState->OnMessage(msg.receiver, msg, this);
	return true;
}

void FSM::proceed(){
	//	progress++; 
	if (progress == EXIT){
		progress = ENTER;
		//if (previousState != NULL)
		//	delete(previousState);
		//previousState = currentState;

		if (nextState.size() > 0){
			if (previousState != NULL)
				delete(previousState);
			previousState = currentState;
			previousExtraInfo = extraInfo;
			currentState = nextState[0];
			extraInfo = nextExtraInfo[0];
			nextState.erase(nextState.begin());
			nextExtraInfo.erase(nextExtraInfo.begin());
		}

		else{
			delete(currentState);
			currentState = previousState;
			previousState = NULL;
			extraInfo = previousExtraInfo;
			previousExtraInfo = NULL;
		}
	}

	else if (progress == ENTER){
		progress = EXECUTE;
	}
	else if (progress == EXECUTE)
	{
		progress = EXIT;
	}
}

void FSM::Update(float &elapsedTime, bool onScreen)
{

	// if a global state exist, call its execute method
	//		if (globalState) globalState->Execute(owner, this);
	// same for teh current state
	switch (progress)
	{
	case ENTER:
		if (currentState) currentState->Enter(owner, this, elapsedTime);
		break;
	case EXECUTE:
		if (currentState) currentState->Execute(owner, this, elapsedTime);
		break;
	case EXIT:
		if (currentState) currentState->Exit(owner, this, elapsedTime);
		break;
	}
}

void MessageDispatcher::dispatchMessage(double delay,
	Entity* sender,
	Entity* receiver,
	int msg,
	void* extraInfo)
{
	// create telegram
	Telegram telegram(delay, sender, receiver, msg, extraInfo);
	if (delay <= 0.0){
		Discharge(receiver, telegram);
	}
	else{

		PriorityQ.push_back(telegram);
	}
}

void MessageDispatcher::DispatchDelayedMessages()
{
	// get current time
	double CurrentTime = Game::getGameTime();

	// now peek at the queue to see if any telegrams need dispatching.
	// remove all telegrams from the front of the queue that have gon
	// past their sell-by datex


	for (unsigned int i = 0; i < PriorityQ.size(); i++)
	if (PriorityQ[i].dispatchTime <= CurrentTime){
		// read the telegram from the front of the queue
		Telegram telegram = *PriorityQ.begin();
		// find the recipient
		Entity* receiver = telegram.receiver;
		// send the telegram to the recipient
		Discharge(receiver, telegram);
		// remove it from the queue		
		PriorityQ.erase(PriorityQ.begin() + i);
	}

}

void MessageDispatcher::Discharge(Entity* pReceiver, Telegram& msg){
	pReceiver->HandleMessage(msg);
}

// global messageDispatcher
MessageDispatcher* messageDispatcher = NULL;

MessageDispatcher* MessageDispatcher::Instance(){
	if (!messageDispatcher)
		messageDispatcher = new MessageDispatcher();
	return messageDispatcher;
}

void FSM::SetCurrentState(State* s, void* extraInfo){
	// RECOVER THESE AFTER SUCCESSFUL COMPILE
	if (currentState != NULL){
		for (unsigned int i = 0; i < nextState.size(); ++i)
			delete(nextState[i]);
		nextState.clear();

		nextState.push_back(s);

		//		for (unsigned int i = 0; i < nextExtraInfo.size(); ++i)
		//			delete(nextExtraInfo[i]);
		nextExtraInfo.clear();

		nextExtraInfo.push_back(extraInfo);
		//	
		progress = EXIT;

	}
	else{
		currentState = s;
		progress = ENTER;
		FSM::extraInfo = extraInfo;
	}


}