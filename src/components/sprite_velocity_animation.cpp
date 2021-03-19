#include "sprite_velocity_animation.h"

#include "game_object.h"
#include "avancezlib.h"
#include "data/sprite_animation.h"

void SpriteVelocityAnimation::Create()
{
	lastPos = go->position;
}

void SpriteVelocityAnimation::Update(float dt)
{
	time += dt;

	auto vel = go->position - lastPos;
	lastPos = go->position;

	lastAnim =
		vel.x < 0 ? &left :
		vel.x > 0 ? &right :
		vel.y < 0 ? &up :
		vel.y > 0 ? &down : lastAnim;

	if (lastAnim != nullptr)
	{
		auto sprite = lastAnim->EvalFrameLoop(time);
		renderer->Draw(sprite, 2, go->position, {});
	}
}
