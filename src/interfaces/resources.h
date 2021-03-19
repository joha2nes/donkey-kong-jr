#pragma once

#include "audio.h"

struct Sprite;

typedef unsigned int Texture;

class Resources
{
public:
	virtual AudioClip LoadAudioClip(const char* filePath) = 0;
	virtual void UnloadAudioClip(AudioClip) = 0;

	virtual Music LoadMusic(const char* filePath) = 0;
	virtual void UnloadMusic(Music) = 0;

	virtual Texture LoadTexture(const char* filePath) = 0;
	virtual void UnloadTexture(Texture) = 0;
};