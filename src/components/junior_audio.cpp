#include "junior_audio.h"

#include "glm/glm.hpp"

void JuniorAudio::Create()
{
	if (junior != nullptr)
		junior->AddObserver(this);

	jump = resources->LoadAudioClip("assets/audio/20_SFX Jump.wav");
	land = resources->LoadAudioClip("assets/audio/18_SFX Hard Landing.wav");
	death = resources->LoadAudioClip("assets/audio/21_SFX Miss.wav");
	bite = resources->LoadAudioClip("assets/audio/15_SFX Bite.wav");
	climb = resources->LoadAudioClip("assets/audio/16_SFX Climbing.wav");
}

void JuniorAudio::Destroy()
{
	resources->UnloadAudioClip(jump);
	resources->UnloadAudioClip(land);
	resources->UnloadAudioClip(death);
	resources->UnloadAudioClip(bite);
	resources->UnloadAudioClip(climb);
}

void JuniorAudio::Update(float dt)
{
	auto isClimbing = junior->state == JuniorState::Climbing || junior->state == JuniorState::ClimbingStretched;

	if (isClimbing && !wasClimbing)
		climbSoundTimer = 0.9f;

	if (isClimbing)
	{
		climbSoundTimer += dt * 0.1f * glm::length(junior->getVelocity());
		if (climbSoundTimer >= 1.f)
		{
			climbSoundTimer = 0.f;
			audio->Play(climb);
		}
	}
	wasClimbing = isClimbing;
}

void JuniorAudio::OnNotify(JuniorEvent e)
{
	switch (e)
	{
	case JuniorEvent::Jump:
		audio->Play(jump);
		break;
	case JuniorEvent::Land:
		audio->Play(land);
		break;
	case JuniorEvent::Death:
		audio->Play(death);
		audio->Play(bite);
		audio->StopMusic();
		break;
	case JuniorEvent::StartClimb:
		break;
	case JuniorEvent::StopClimb:
		break;
	}
}
