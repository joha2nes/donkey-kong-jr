#include "snapjaw.h"

#include "game_object.h"
#include "data/path.h"
#include "glm/glm.hpp"
#include "fruit.h"
#include "animate_once.h"

void BlueSnapjawBehavior::Create()
{
	fromIndex = 0;
	toIndex = path->nodes[fromIndex].links[glm::abs(rand() % path->nodes[fromIndex].links.size())];

	go->position = path->nodes[fromIndex].pos;
}

void BlueSnapjawBehavior::Update(float dt)
{
	auto from = path->nodes[fromIndex];
	auto to = path->nodes[toIndex];

	auto speed = 60.f;

	distanceMoved += dt * speed;

	auto dist = glm::distance(from.pos, to.pos);
	auto t = glm::clamp(distanceMoved / dist, 0.f, 1.f);
	auto pos = glm::mix(from.pos, to.pos, t);
	go->position = pos;

	if (t == 1.f)
	{
		distanceMoved = 0.f;

		if (to.links.size() == 0)
		{
			AddWaterSplash();
			go->Remove();
		}
		else
		{
			fromIndex = toIndex;
			toIndex = to.links[glm::abs(rand() % to.links.size())];
		}
	}

	auto col = go->GetComponent<CollideComponent*>();
	for (auto c : col->collisions)
	{
		auto fruit = c.other->go->GetComponent<Fruit*>();
		if (fruit != nullptr && fruit->IsFalling())
		{
			// TODO: animation and stop time
			go->Remove();
		}
	}
}

void BlueSnapjawBehavior::AddWaterSplash()
{
	auto obj = waterSplashParent->AddGameObject(go->position);
	auto c = obj->AddComponent<AnimateOnce>();
	auto tex = resources->LoadTexture("assets/sprites.png");
	c->animation = new SpriteAnimation({
		{ 0.1, { tex, { 3 * 16, 4 * 16, 16, 16 }, {}, { -4, 0 } } },
		{ 0.1, { tex, { 4 * 16, 4 * 16, 16, 16 }, {}, { -4, 0 } } },
	});
}

void RedSnapjawBehavior::Create()
{
	fromIndex = 0;
	toIndex = path.nodes[fromIndex].links[glm::abs(rand() % path.nodes[fromIndex].links.size())];
		
	go->position = path.nodes[fromIndex].pos;
}

void RedSnapjawBehavior::Update(float dt)
{
	auto from = path.nodes[fromIndex];
	auto to = path.nodes[toIndex];

	auto direction = to.pos - from.pos;
	auto speed =
		direction.y < 0.f ? 10.f :
		direction.y > 0.f ? 40.f :
		20.f;

	distanceMoved += dt * speed;

	auto dist = glm::length(direction);
	auto t = glm::clamp(distanceMoved / dist, 0.f, 1.f);
	auto pos = glm::mix(from.pos, to.pos, t);
	go->position = pos;

	if (t == 1.f)
	{
		distanceMoved = 0.f;

		if (to.links.size() == 0)
			go->Remove();
		else
		{
			fromIndex = toIndex;
			toIndex = to.links[glm::abs(rand() % to.links.size())];
		}
	}

	auto col = go->GetComponent<CollideComponent*>();
	for (auto c : col->collisions)
	{
		auto fruit = c.other->go->GetComponent<Fruit*>();
		if (fruit != nullptr && fruit->IsFalling())
		{
			// TODO: animation and stop time
			go->Remove();
		}
	}
}
