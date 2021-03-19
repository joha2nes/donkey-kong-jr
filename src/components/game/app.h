#pragma once

#include "component.h"

#include "components/fruit.h" 

class StartScreen;
class Game;
class Text;
class BlinkingText;
class Audio;

class App
	: public Component
	, public Observer<ScoreEvent>
{
public:
	enum class State { StartScreen, Game };

private:
	State state;
	StartScreen* startScreen;
	Game* game;

	int currentScore;
	int highScore;
	Text* currentScoreUI;
	Text* highScoreUI;
	BlinkingText* oneUpHint;

	Texture levelTexture;
	Texture spritesTexture;
	Music gameMusic;

public:
	void Create();
	void Update(float dt);
	void Destroy();

	void OnNotify(ScoreEvent);
};