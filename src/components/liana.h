#pragma once

#include "component.h"

class Liana : public Component
{
public:
	const CollideComponent* collider;

	void Create();
};
