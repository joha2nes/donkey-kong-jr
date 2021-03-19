#include "game_object.h"
#include "component.h"
#include "avancezlib.h"

void GameObject::Init()
{
	for (auto c : components)
		c->Create();

	enabled = true;
}

void GameObject::Update(float dt)
{
	if (!enabled)
		return;

	for (auto c : components)
	{
		if (c->enabled)
			c->Update(dt);
	}

	for (auto c : children)
		c->Update(dt);
}

void GameObject::Destroy()
{
	used = false;

	for (auto c : components)
	{
		c->Destroy();
		delete c;
		c = nullptr;
	}
	components.clear();
}

GameObject::~GameObject()
{
	SDL_Log("GameObject::~GameObject");
}

GameObject* GameObject::AddGameObject(glm::vec2 position)
{
	auto go = gameObjectPool->FirstAvailable();
	go->engine = engine;
	go->gameObjectPool = gameObjectPool;
	go->addedGameObjects = addedGameObjects;
	go->removedGameObjects = removedGameObjects;
	go->used = true;
	go->enabled = true;
	go->position = position;
	go->time = time;
	go->audio = audio;
	go->input = input;
	go->renderer = renderer;
	go->resources = resources;
	go->root = root;
	addedGameObjects->push({ this, go });
	return go;
}

void GameObject::Remove()
{
	removedGameObjects->push(this);
}
