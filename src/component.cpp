#include "component.h"

#include "game_object.h"
#include "avancezlib.h"
#include "interfaces/renderer.h"

#include "data/subject.h"

float minAbs(float a, float b)
{
	return abs(a) < abs(b) ? a : b;
}

void CollideComponent::Update(float dt)
{
	collisions.clear();
	TestPosition(collisions, go->position);

	//auto a = go->position;
	//auto b = go->position + glm::vec2(size.x, 0.f);
	//auto c = go->position + size;
	//auto d = go->position + glm::vec2(0.f, size.y);
	//renderer->DrawLine(a, b);
	//renderer->DrawLine(b, c);
	//renderer->DrawLine(c, d);
	//renderer->DrawLine(d, a);
}

glm::vec2 CollideComponent::GetCenter() const
{
	return go->position + size / 2.f;
}

static void testPosition(std::vector<Collision>& collisions, GameObject* go, GameObject* other, const glm::vec2& size, const glm::vec2& pos)
{
	if (other != go && other->enabled)
	{
		auto col = other->GetComponent<CollideComponent*>();
		if (col != nullptr)
		{
			auto minA = pos;
			auto maxA = pos + size;
			auto minB = other->position;
			auto maxB = other->position + col->size;

			if (minA.x < maxB.x &&
				maxA.x > minB.x &&
				minA.y < maxB.y &&
				maxA.y > minB.y)
			{
				auto dx = minAbs(minA.x - maxB.x, maxA.x - minB.x);
				auto dy = minAbs(minA.y - maxB.y, maxA.y - minB.y);
				auto dx_ = abs(dx) <= abs(dy) ? dx : 0.f;
				auto dy_ = abs(dy) <= abs(dx) ? dy : 0.f;

				auto overlap = glm::vec2(dx_, dy_);

				collisions.push_back(Collision{ col, overlap });
			}
		}

		for (auto c : other->children)
			testPosition(collisions, go, c, size, pos);
	}
}
void CollideComponent::TestPosition(std::vector<Collision>& collisions, const glm::vec2& pos)
{
	testPosition(collisions, go, root, size, pos);
}
