#include "sprite_component.h"

#include "avancezlib.h"
#include "component.h"
#include "game_object.h"

void SpriteComponent::Update(float dt)
{
	renderer->Draw(&sprite, 3, go->position, {});
}
