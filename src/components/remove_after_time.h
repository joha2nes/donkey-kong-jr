#pragma once

#include "component.h"

class RemoveAfterTime : public Component
{
public:
	float timer = 3.f;
	void Update(float dt);
};
