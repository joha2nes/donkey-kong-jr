#pragma once

#include "../component.h"

#include "data/sprite.h"
#include "data/sprite_animation.h"

class LoopAnimation : public Component
{
	float time;
public:
	bool useUnscaledTime;
	SpriteAnimation animation;

	void Create();
	void Update(float dt);
};