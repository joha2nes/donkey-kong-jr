#pragma once

#include "component.h"

#include <string>
#include "glm/vec3.hpp"

class Text : public Component
{
public:
	std::string text = "Text";
	glm::uvec3 color = { 255, 255, 255 };

	void Update(float dt);
};