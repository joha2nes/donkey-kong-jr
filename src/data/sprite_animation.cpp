#include "sprite_animation.h"

#include <vector>

SpriteAnimation::SpriteAnimation()
{
	duration = 0.f;
}

SpriteAnimation::SpriteAnimation(const std::vector<Frame>& frames)
	: frames(frames)
{
	duration = 0.f;
	for (int i = 0; i < frames.size(); i++)
		duration += frames[i].time;
}

Sprite* SpriteAnimation::EvalFrameLoop(float time)
{
	return EvalFrame(fmod(time, duration));
}

Sprite* SpriteAnimation::EvalFrame(float time)
{
	for (int i = 0; i < frames.size(); i++)
	{
		if (time < frames[i].time)
			return &frames[i].sprite;
		time -= frames[i].time;
	}
	return &frames[frames.size() - 1].sprite;
}

void SpriteAnimation::AddFrame(Frame f)
{
	frames.push_back(f);
	duration += f.time;
}

float SpriteAnimation::GetDuration() const
{
	return duration;
}
