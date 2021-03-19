#pragma once

#include "SDL.h"

class Input
{
public:
	virtual bool GetKey(SDL_Scancode key) = 0;
	virtual bool GetKeyDown(SDL_Scancode key) = 0;
	virtual bool GetKeyUp(SDL_Scancode key) = 0;
};