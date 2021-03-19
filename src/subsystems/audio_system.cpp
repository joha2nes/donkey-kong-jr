#include "audio_system.h"

#include <map>
#include <cassert>
#include <string>
#include "SDL.h"
#include "SDL_mixer.h"

bool AudioSystem::Initialize()
{
	availableAudioClipID = 0;
	availableMusicID = 0;
	return Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 512) >= 0;
}

void AudioSystem::Terminate()
{
	for (auto p : loadedChunks)
	{
		Mix_FreeChunk(p.second);
		p.second = nullptr;
	}
	loadedChunks.clear();
	Mix_Quit();
}

AudioClip AudioSystem::LoadAudioClip(const char* filePath)
{
	auto chunk = Mix_LoadWAV(filePath);
	assert(chunk != nullptr);
	auto clip = availableAudioClipID++;
	loadedChunks[clip] = chunk;

	SDL_Log("Allocated audio clip: %s (%s)", std::to_string(clip).c_str(), filePath);

	return clip;
}

void AudioSystem::UnloadAudioClip(AudioClip clip)
{
	auto chunk = loadedChunks[clip];
	loadedChunks.erase(clip);
	Mix_FreeChunk(chunk);

	SDL_Log("Deallocated audio clip: %s", std::to_string(clip).c_str());
}

Music AudioSystem::LoadMusic(const char* filePath)
{
	auto mus = Mix_LoadMUS(filePath);
	assert(mus != nullptr);
	auto music = availableMusicID++;
	loadedMusic[music] = mus;

	SDL_Log("Allocated music: %s (%s)", std::to_string(music).c_str(), filePath);

	return music;
}

void AudioSystem::UnloadMusic(Music music)
{
	auto mus = loadedMusic[music];
	loadedMusic.erase(music);
	Mix_FreeMusic(mus);

	SDL_Log("Deallocated music: %s", std::to_string(music).c_str());
}

void AudioSystem::Play(AudioClip clip)
{
	Mix_PlayChannel(-1, loadedChunks[clip], 0);
}

void AudioSystem::PlayMusic(Music music)
{
	Mix_PlayMusic(loadedMusic[music], 0);
}

void AudioSystem::PauseMusic()
{
	Mix_PauseMusic();
}

void AudioSystem::StopMusic()
{
	Mix_PauseMusic();
	Mix_RewindMusic();
}

void AudioSystem::LoopMusic(Music music)
{
	Mix_PlayMusic(loadedMusic[music], -1);
}
