#pragma once

#include "component.h"
#include "data/sprite_animation.h"

class SpriteVelocityAnimation : public Component
{
	float time;
	glm::vec2 lastPos;
	SpriteAnimation* lastAnim;
public:
	SpriteAnimation up;
	SpriteAnimation down;
	SpriteAnimation left;
	SpriteAnimation right;

	void Create();
	void Update(float dt);
};