#include "liana.h"

#include "game_object.h"

void Liana::Create()
{
	collider = go->GetComponent<CollideComponent*>();
}
