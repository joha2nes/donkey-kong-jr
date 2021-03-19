#pragma once

#include "component.h"
#include "data/path.h"

class RigidBody;

class BlueSnapjawBehavior : public Component
{
	int fromIndex;
	int toIndex;

	float distanceMoved;

	void AddWaterSplash();

public:
	GameObject* waterSplashParent;
	Path* path;

	void Create();
	void Update(float dt);
};

class RedSnapjawBehavior : public Component
{
	int fromIndex;
	int toIndex;

	float distanceMoved;

public:
	Path path;
	
	void Create();
	void Update(float dt);
};