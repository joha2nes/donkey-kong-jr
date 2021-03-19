#pragma once

#include <string>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct Sprite;

class Renderer
{
public:
	virtual void Draw(const Sprite* sprite, int layer, const glm::ivec2& position, glm::bvec2 flip) = 0;
	virtual void Draw(const std::string& text, int layer, const glm::ivec2& position, const glm::uvec3 color) = 0;
	virtual void DrawLine(glm::vec2 a, glm::vec2 b) = 0;
};