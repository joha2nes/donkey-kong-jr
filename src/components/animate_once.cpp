#include "animate_once.h"

#include "glm/glm.hpp"

#include "game_object.h"
#include "data/sprite_animation.h"
#include "interfaces/renderer.h"

void AnimateOnce::Create()
{
	time = 0.f;
}

void AnimateOnce::Update(float dt)
{
	time = glm::min(time + dt, animation->GetDuration());

	auto sprite = animation->EvalFrame(time);

	renderer->Draw(sprite, 3, go->position, {});

	if (time == animation->GetDuration())
		go->Remove();
}