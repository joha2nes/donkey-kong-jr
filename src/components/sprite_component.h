#pragma once

#include "component.h"

#include "data/sprite.h"

class SpriteComponent : public Component
{
public:
	Sprite sprite;
	
	void Update(float dt);
};