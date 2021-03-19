#include "loop_animation.h"

#include "avancezlib.h"
#include "data/sprite.h"
#include "data/sprite_animation.h"

void LoopAnimation::Create()
{
	time = 0.f;
}
void LoopAnimation::Update(float dt)
{
	renderer->Draw(
		animation.EvalFrameLoop(time),
		1,
		go->position,
		{}
	);

	time += useUnscaledTime ? Component::time->GetUnscaledDeltaTime() : dt;
}