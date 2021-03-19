#pragma once

template <typename TEnv>
class StateMachine;

template <typename TEnv>
class State
{
	friend StateMachine<TEnv>;

	State* nextState;

public:

	virtual void Enter() {}
	virtual void Update(float dt) { }
	virtual void Exit() {}

protected:
	TEnv env;

	void Switch(State* state)
	{
		nextState = state;
	}
};

template <typename TEnv>
class StateMachine
{
	State<TEnv>* currentState;

public:
	StateMachine(TEnv env, State<TEnv>* initState)
	{
		currentState = initState;
		currentState->env = env;
		currentState->Enter();
	}
	~StateMachine()
	{
		delete currentState;
	}

	void Update(float dt)
	{
		currentState->Update(dt);
		if (currentState->nextState != nullptr)
		{
			currentState->Exit();
			auto oldState = currentState;
			currentState = currentState->nextState;
			currentState->env = oldState->env;
			delete oldState;
			currentState->Enter();
		}
	}
};
