#pragma once

#include <vector>

#include "data/sprite.h"

struct Frame
{
	float time;
	Sprite sprite;
};

class SpriteAnimation
{
	std::vector<Frame> frames;
	float duration;

public:
	SpriteAnimation();
	SpriteAnimation(const std::vector<Frame>& frames);

	Sprite* EvalFrameLoop(float time);
	Sprite* EvalFrame(float time);

	void AddFrame(Frame);

	float GetDuration() const;
};
