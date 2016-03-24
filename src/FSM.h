#pragma once
#include "gameplay.h"
#include "State.h"

using namespace gameplay;


class Entity;
//class State;

// the message system to be used between entities
struct Telegram
{
	Entity* sender;
	Entity* receiver;
	int msg;
	double dispatchTime;
	void* extraInfo;
	Telegram(float delay, Entity *sender, Entity *receiver, int msg, void *extraInfo)
		:dispatchTime(Game::getGameTime() + delay), sender(sender), receiver(receiver),
		msg(msg), extraInfo(extraInfo){}
};

class FSM{
public:
	enum Message{
		GOAL_SCORED,
		START_ROUND,
		GAME_OVER,
		SPEED_POWERUP,
		BARRIER_POWERUP,
		MAGNET_POWERUP,
		TOUCH_DOWN,
		TOUCH_RELEASE,
		SET_TARGET,
		PEEK_UP,
		ZOOM_INTO_TARGET
	};

	void* extraInfo;
	std::vector<void*> nextExtraInfo;

	FSM(Entity* owner)
		:owner(owner), currentState(NULL)
		, previousState(NULL){}
	void SetCurrentState(State* s, void* extraInfo = NULL);

	void SetNextState(State* s, void* extraInfo = NULL){
		nextState.push_back(s);
		nextExtraInfo.push_back(extraInfo);
	}
	//	void SetGlobalState(State* s) { globalState = s; }
	//	void SetPreviousState(State* s){ previousState = s; }

	void Update(float &elapsedTime, bool onScreen);

	void proceed();
	bool isInState(const State& st);
	bool HandleMessage(Telegram& msg);
protected:


private:
	Entity* owner;
	State* currentState;
	//	State* upcomingState;
	std::vector<State*> nextState;
	State* previousState;
	void* previousExtraInfo;
	//	State* globalState;

	int progress;
	FSM(){}

	enum state_progress
	{
		ENTER,
		EXECUTE,
		EXIT
	};
};

class MessageDispatcher
{
public:
	// this is a singleton
	static MessageDispatcher* Instance();

	// send a message to another agent
	void dispatchMessage(double delay, Entity* sender
		, Entity* receiver, int msg, void* extraInfo);

	// for lua use
	void dispatchMessageEntity(double delay, Entity* sender
		, Entity* receiver, int msg, Entity* ExtraInfo){
		dispatchMessage(delay, sender, receiver, msg, ExtraInfo);
	}

	// for lua use
	void dispatchMessageNone(double delay, Entity* sender
		, Entity* receiver, int msg){
		dispatchMessage(delay, sender, receiver, msg, NULL);
	}

	void DispatchDelayedMessages();
private:
	std::vector<Telegram> PriorityQ;

	void Discharge(Entity* pReceiver, Telegram& msg);

	MessageDispatcher(){}
};