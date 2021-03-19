#include "text.h"

#include "interfaces/renderer.h"
#include "game_object.h"

void Text::Update(float dt)
{
	renderer->Draw(text.c_str(), 4, go->position, color);
}
