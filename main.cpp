#define _CRT_SECURE_NO_WARNINGS

#include "avancezlib.h"
#include "data/object_pool.h"

#include <set>
#include <vector>
#include <string>
#include <iostream>

#include "SDL.h"
#include "glm/glm.hpp"

#include "components/game/app.h"

int main(int argc, char** argv)
{
	AvancezLib().run<App>("DONKEY KONG JR.", { 224, 256 }, 3);
	return 0;
}