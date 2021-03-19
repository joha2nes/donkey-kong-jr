#pragma once

#include <map>

#include "interfaces/audio.h"
#include "SDL_mixer.h"

class AudioSystem : public Audio
{
	std::map<AudioClip, Mix_Chunk*> loadedChunks;
	std::map<Music, Mix_Music*> loadedMusic;
	unsigned int availableAudioClipID;
	unsigned int availableMusicID;

public:
	bool Initialize();
	void Terminate();

	AudioClip LoadAudioClip(const char* filePath);
	void UnloadAudioClip(AudioClip);
	Music LoadMusic(const char* filePath);
	void UnloadMusic(Music);

	void Play(AudioClip);
	void PlayMusic(Music);
	void PauseMusic();
	void StopMusic();
	void LoopMusic(Music);
};