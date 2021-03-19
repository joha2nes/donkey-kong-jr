#pragma once

#include "component.h"
#include "data/path.h"

class BlueSnapjawSpawner : public Component
{
	float spawnTimer;

public:
	float spawnInterval = 3.5f;
	Path path;

	void Create();
	void Update(float dt);
};