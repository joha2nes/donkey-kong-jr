#pragma once

#include <set>
#include "observer.h"

template <typename T>
class Subject
{
	std::set<Observer<T>*> observers;
public:

	void AddObserver(Observer<T>* observer)
	{
		observers.insert(observer);
	}

	void RemoveObserver(Observer<T>* observer)
	{
		observers.erase(std::find(observers.begin(), observers.end(), observer));
	}

	void ClearObservers()
	{
		observers.clear();
	}

	void Notify(T value)
	{
		for (auto o : observers)
			o->OnNotify(value);
	}
};
