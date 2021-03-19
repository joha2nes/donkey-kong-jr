#pragma once

#include "component.h"
#include "glm/glm.hpp"

class Text;

class BlinkingText : public Component
{
	float time;
	glm::uvec3 color;
public:
	Text* text;

	void Create();
	void Update(float dt);
};