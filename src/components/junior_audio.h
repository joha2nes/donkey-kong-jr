#pragma once

#include "component.h"
#include "junior.h"

#include "interfaces/audio.h"

class JuniorAudio
	: Component
	, Observer<JuniorEvent>
{
	AudioClip jump;
	AudioClip land;
	AudioClip death;
	AudioClip bite;
	AudioClip climb;

	float climbSoundTimer;
	bool wasClimbing;

	void OnNotify(JuniorEvent);

	void Create();
	void Update(float dt);
	void Destroy();

public:
	JuniorController* junior;
};