#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

#include <map>
#include <string>

#include "game_object.h"

#include "component.h"
#include "interfaces/renderer.h"
#include "interfaces/resources.h"
#include "interfaces/input.h"
#include "interfaces/time.h"

class AvancezLib;
class AudioSystem;
class RenderSystem;
class Audio;
class Scene;

class AvancezLib
	: public Input
	, public Resources
	, public Time
{
	void run(const char* title, glm::ivec2 resolution, unsigned int pixelSize);

public:
	template <class TComponent>
	void run(const char* title, glm::ivec2 resolution, unsigned int pixelSize)
	{
		init(title, resolution, pixelSize);

		root = new GameObject();
		root->enabled = true;
		root->addedGameObjects = &addedGameObjects;
		root->removedGameObjects = &removedGameObjects;
		root->gameObjectPool = &gameObjectPool;
		root->engine = this;
		root->time = this;
		root->input = this;
		root->audio = (Audio*)audioSystem;
		root->renderer = (Renderer*)renderSystem;
		root->resources = this;
		root->root = root;
		auto comp = root->AddComponent<TComponent>();
		comp->Create();

		run(title, resolution, pixelSize);
	}

public:

	// Destroys the avancez library instance
	void destroy();

	// Creates the main window. Returns true on success.
	bool init(const char* title, glm::ivec2 resolution, unsigned int pixelSize);

	void processInput();

	// Time
	float GetElapsedTime() const;
	float GetUnscaledDeltaTime() const;
	void SetTimeScale(float scale);
	float GetTimeScale() const;

	// Input
	bool GetKey(SDL_Scancode key);
	bool GetKeyDown(SDL_Scancode key);
	bool GetKeyUp(SDL_Scancode key);

	// Resources
	AudioClip LoadAudioClip(const char* filePath);
	void UnloadAudioClip(AudioClip);
	Music LoadMusic(const char* filePath);
	void UnloadMusic(Music);
	Texture LoadTexture(const char* filePath);
	void UnloadTexture(Texture);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	float timeScale = 1.f;
	float unscaledDeltaTime;

	struct KeyState
	{
		bool key;
		bool keyDown;
		bool keyUp;
	};

	std::map<SDL_Scancode, KeyState> keyStates;
	Scene* _currentScene;

	std::queue<std::pair<GameObject*, GameObject*>> addedGameObjects;
	std::queue<GameObject*> removedGameObjects;
	ObjectPool<GameObject> gameObjectPool;

	AudioSystem* audioSystem;
	RenderSystem* renderSystem;

	void expensiveRemoveGameObject(GameObject* root, GameObject* goToRemove);
	void destroyGameObject(GameObject*);

public:
	GameObject* root;
};
