#include "components/fruit.h"

#include "components/sprite_component.h"
#include "components/remove_after_time.h"

void Fruit::Create()
{
	_state = State::Hanging;
}

void Fruit::Update(float dt)
{
	switch (_state)
	{
	case Fruit::State::Hanging:
	{
		auto col = go->GetComponent<CollideComponent*>();
		for (auto collision : col->collisions)
		{
			if (collision.other->go->GetComponent<JuniorController*>())
			{
				_state = State::Falling;

				// Spawn dollars
				auto go = this->go->AddGameObject(this->go->position);
				auto s = go->AddComponent<SpriteComponent>();
				s->sprite = score400Sprite;

				auto rat = go->AddComponent<RemoveAfterTime>();
				rat->timer = 1.0f;

				Notify({ 400 });

				audio->Play(fallSound);
			}
		}
	}
	break;
	case Fruit::State::Falling:
	{
		go->position += dt * glm::vec2(0.f, 120.f);
	}
	break;
	}

	if (go->position.y > 300.f)
		go->Remove();
}

bool Fruit::IsFalling() const
{
	return _state == State::Falling;
}
