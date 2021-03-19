#pragma once

#include "SDL.h"
#include "glm/vec2.hpp"
#include "interfaces/resources.h"

struct Sprite
{
	Texture texture;
	SDL_Rect region = { 0, 0, 16, 16 };
	glm::bvec2 flip;
	glm::ivec2 offset;
};
