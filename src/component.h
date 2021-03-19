#pragma once

#include <set>
#include <queue>

#include "glm/vec2.hpp"

#include "data/object_pool.h"
#include "data/subject.h"

#include "interfaces/audio.h"
#include "interfaces/input.h"
#include "interfaces/renderer.h"
#include "interfaces/resources.h"
#include "interfaces/time.h"

class GameObject;
class AvancezLib;

class Component
{
	friend GameObject;

protected:
	Time* time;
	Input* input;
	Audio* audio;
	Renderer* renderer;
	Resources* resources;
	GameObject* root;

public:
	GameObject* go;
	bool enabled;

	virtual ~Component() {}

	virtual void Create() {}
	virtual void Update(float dt) {}
	virtual void Destroy() {}
};

class CollideComponent;

struct Collision
{
	CollideComponent* other;
	glm::vec2 overlap;
};

class CollideComponent : public Component
{
public:
	glm::vec2 size = { 16, 16 };
	std::vector<Collision> collisions;

	void Update(float dt);

	glm::vec2 GetCenter() const;

	void TestPosition(std::vector<Collision>& collisions, const glm::vec2& pos);
};
