#pragma once

#include "component.h"

class SpriteAnimation;

class AnimateOnce : public Component
{
	float time;

public:
	SpriteAnimation* animation;

	void Create();
	void Update(float dt);
};