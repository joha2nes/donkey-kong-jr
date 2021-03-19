#pragma once

class Disposable;

template <typename T>
class Observer
{
public:
	virtual void OnNotify(T value) = 0;
};

template <typename T>
class LambdaObserver : public Observer<T>
{
	void (*lambda)(T);
public:
	LambdaObserver(void (*lambda)(T))
	{
		this->lambda = lambda;
	}

	void OnNotify(T value)
	{
		lambda(value);
	}
};
