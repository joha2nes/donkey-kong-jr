#include "avancezlib.h"

#include <map>
#include <queue>

#include "glm/glm.hpp"

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "subsystems/audio_system.h"
#include "subsystems/render_system.h"

// TODO: improve
void AvancezLib::expensiveRemoveGameObject(GameObject* root, GameObject* goToRemove)
{
	if (root != nullptr)
	{
		auto it = std::find(root->children.begin(), root->children.end(), goToRemove);
		if (it != root->children.end())
		{
			auto go = *it;
			root->children.erase(it);
			destroyGameObject(go);
		}
		else
		{
			for (auto c : root->children)
				expensiveRemoveGameObject(c, goToRemove);
		}
	}
}

void AvancezLib::run(const char* title, glm::ivec2 resolution, unsigned int pixelSize)
{
	// add game objects
	while (!addedGameObjects.empty())
	{
		auto p = addedGameObjects.front();
		addedGameObjects.pop();

		p.first->children.insert(p.second);
		p.second->engine = this;
		p.second->Init();
		p.second->enabled = true;
	}

	float lastTime = GetElapsedTime();
	while (true)
	{
		float newTime = GetElapsedTime();
		unscaledDeltaTime = newTime - lastTime;

		// NOTE: hotfix to make sure the player doesnt fall through platforms
		unscaledDeltaTime = glm::min(unscaledDeltaTime, 0.1f);

		float dt = timeScale * unscaledDeltaTime;
		lastTime = newTime;

		processInput();
		renderSystem->Clear();

		// update game objects
		root->Update(dt);

		// add game objects
		while (!addedGameObjects.empty())
		{
			auto p = addedGameObjects.front();
			addedGameObjects.pop();

			p.first->children.insert(p.second);
			p.second->engine = this;
			p.second->Init();
			p.second->enabled = true;
		}

		// remove game objects
		auto quit = false;
		while (!removedGameObjects.empty())
		{
			auto go = removedGameObjects.front();
			removedGameObjects.pop();
			expensiveRemoveGameObject(root, go);
		
			if (go == root)
				quit = true;
		}

		renderSystem->Present();

		if (quit)
			break;
	}

	destroy();
}

bool AvancezLib::init(const char* title, glm::ivec2 resolution, unsigned int pixelSize)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pixelSize * resolution.x, pixelSize * resolution.y, 0);
	if (window == nullptr)
	{
		SDL_Log("Failed to create a window: %s", SDL_GetError());
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		SDL_Log("Failed to create a renderer: %s", SDL_GetError());
		SDL_Quit();
		return false;
	}

	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize TTF");
		SDL_Quit();
		return false;
	}

	auto imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		SDL_Log("Failed to initialize SDL Image");
		SDL_Quit();
		return false;
	}

	audioSystem = new AudioSystem();
	if (!audioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		SDL_Quit();
		return false;
	}

	renderSystem = new RenderSystem();
	renderSystem->Initialize(renderer, resolution, pixelSize);

	gameObjectPool.Create(512);

	return true;
}

void AvancezLib::processInput()
{
	for (auto it = keyStates.begin(); it != keyStates.end(); it++)
	{
		(*it).second.keyDown = false;
		(*it).second.keyUp = false;
	}

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				keyStates[event.key.keysym.scancode].key = true;
				keyStates[event.key.keysym.scancode].keyDown = true;
			}
			break;
		case SDL_KEYUP:
			keyStates[event.key.keysym.scancode].key = false;
			break;
		}
	}
}

void AvancezLib::destroy()
{
	gameObjectPool.Deallocate();

	audioSystem->Terminate();
	delete audioSystem;
	audioSystem = nullptr;
	
	renderSystem->Terminate();
	delete renderSystem;
	renderSystem = nullptr;
	
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

float AvancezLib::GetElapsedTime() const
{
	return SDL_GetTicks() / 1000.f;
}

float AvancezLib::GetUnscaledDeltaTime() const
{
	return unscaledDeltaTime;
}

void AvancezLib::SetTimeScale(float scale)
{
	timeScale = scale;
}

float AvancezLib::GetTimeScale() const
{
	return timeScale;
}

bool AvancezLib::GetKey(SDL_Scancode key)
{
	return keyStates[key].key;
}
bool AvancezLib::GetKeyDown(SDL_Scancode key)
{
	return keyStates[key].keyDown;
}
bool AvancezLib::GetKeyUp(SDL_Scancode key)
{
	return keyStates[key].keyUp;
}

void AvancezLib::destroyGameObject(GameObject* go)
{
	for (auto goC : go->children)
		destroyGameObject(goC);
	
	go->used = false;
	for (auto c : go->components)
	{
		c->Destroy();
		delete c;
		c = nullptr;
	}

	go->components.clear();
	go->children.clear();
}

AudioClip AvancezLib::LoadAudioClip(const char* filePath)
{
	return audioSystem->LoadAudioClip(filePath);
}

void AvancezLib::UnloadAudioClip(AudioClip clip)
{
	audioSystem->UnloadAudioClip(clip);
}

Music AvancezLib::LoadMusic(const char* filePath)
{
	return audioSystem->LoadMusic(filePath);
}

void AvancezLib::UnloadMusic(Music music)
{
	audioSystem->UnloadMusic(music);
}

Texture AvancezLib::LoadTexture(const char* filePath)
{
	return renderSystem->LoadTexture(filePath);
}

void AvancezLib::UnloadTexture(Texture t)
{
	renderSystem->UnloadTexture(t);
}
