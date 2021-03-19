#pragma once

#include "component.h"

#include "avancezlib.h"

class SpriteGrid : public Component
{
public:
	static const int width = 28;
	static const int height = 32;

private:
	Sprite* grid[height][width];

public:
	void Paint(Sprite* sprite, glm::ivec2 min, glm::ivec2 max);
	void Update(float dt);
};
