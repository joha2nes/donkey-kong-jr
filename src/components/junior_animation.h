#pragma once

#include "component.h"

#include "data/sprite_animation.h"

class JuniorController;

class JuniorAnimation : Component
{
	float time;
	JuniorController* player;

	SpriteAnimation moveRight;
	SpriteAnimation moveLeft;
	SpriteAnimation jumpLeft;
	SpriteAnimation jumpRight;
	SpriteAnimation climbAnimation;
	SpriteAnimation climb2Animation;
	SpriteAnimation deathAnimation;

	void Create();
	void Update(float dt);
};

