#include "remove_after_time.h"

#include "game_object.h"

void RemoveAfterTime::Update(float dt)
{
	timer -= dt;
	if (timer <= 0.f)
		go->Remove();
}
