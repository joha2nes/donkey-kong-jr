#pragma once

#include "component.h"

#include "data/sprite.h"

class StartScreen : public Component
{
public:
	enum class State { Title, InsertCoin };

private:
	State state;
	Sprite titleSprite;
	float time;

public:
	bool startGame;

	void Create();
	void Update(float dt);
};