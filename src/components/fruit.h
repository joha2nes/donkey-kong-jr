#pragma once

#include "component.h"
#include "data/observer.h"
#include "data/empty.h"
#include "components/junior.h"

#include "interfaces/resources.h"
#include "data/sprite.h"

struct ScoreEvent
{
	int addScore;
};

class Fruit
	: public Component
	, public Subject<ScoreEvent>
{
	enum class State { Hanging, Falling };

	State _state;

	void Create();
	void Update(float dt);

public:
	AudioClip fallSound;
	Sprite score400Sprite;

	bool IsFalling() const;
};