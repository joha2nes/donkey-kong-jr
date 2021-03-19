#pragma once

typedef unsigned int AudioClip;
typedef unsigned int Music;

class Audio
{
public:
	virtual void Play(AudioClip) = 0;
	virtual void PlayMusic(Music) = 0;
	virtual void LoopMusic(Music) = 0;
	virtual void PauseMusic() = 0;
	virtual void StopMusic() = 0;
};
