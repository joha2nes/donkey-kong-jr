#pragma once

#include <vector>
#include <queue>
#include <set>
#include "glm/vec2.hpp"

#include "data/observer.h"
#include "data/object_pool.h"

class AvancezLib;
class Component;
class Time;
class Input;
class Audio;
class Renderer;
class Resources;

class GameObject
{
	friend AvancezLib;
	friend Component;
public:

	std::set<GameObject*> children;
	
	std::vector<Component*> components;

	std::queue<std::pair<GameObject*, GameObject*>>* addedGameObjects;
	std::queue<GameObject*>* removedGameObjects;
	ObjectPool<GameObject>* gameObjectPool;

	AvancezLib* engine;
	Time* time;
	Input* input;
	Audio* audio;
	Renderer* renderer;
	Resources* resources;
	GameObject* root;

public:
	glm::vec2 position;
	bool enabled;
	bool used;

	virtual ~GameObject();

	template <class T>
	T* AddComponent()
	{
		T* t = new T();
		auto c = (Component*)t;
		components.push_back(c);
		c->go = this;
		c->enabled = true;
		c->time = time;
		c->input = input;
		c->audio = audio;
		c->renderer = renderer;
		c->resources = resources;
		c->root = root;
		return t;
	}

	template<typename T>
	T GetComponent()
	{
		for (Component* c : components)
		{
			T t = dynamic_cast<T>(c);
			if (t != nullptr)
				return t;
		}
		return nullptr;
	}

	void Init();
	void Update(float dt);
	void Destroy();

	GameObject* AddGameObject(glm::vec2 position);
	void Remove();
};