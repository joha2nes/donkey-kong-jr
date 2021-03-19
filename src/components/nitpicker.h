#pragma once

#include "component.h"

class NitpickerBehavior : public Component
{
	std::vector<float> path;
public:
	void Create();
	void Update(float dt);
};