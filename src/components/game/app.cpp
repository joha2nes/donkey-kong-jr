#include "app.h"

#include "avancezlib.h"
#include "game_object.h"
#include "components/text.h"
#include "components/blinking_text.h"
#include "components/sprite_component.h"

#include "start_screen.h"
#include "game.h"
#include "data/utils.h"

void App::Create()
{
	currentScore = 0;

	state = State::StartScreen;

	spritesTexture = resources->LoadTexture("assets/sprites.png");
	levelTexture = resources->LoadTexture("assets/level.png");
	gameMusic = resources->LoadMusic("assets/audio/04_Stage 1 Vines.wav");

	// CURRENT SCORE UI
	{
		auto go = this->go->AddGameObject({ 8, 8 });
		auto text = go->AddComponent<Text>();
		text->text = "000000";

		currentScoreUI = text;
	}

	// 1UP HINT UI
	{
		auto go = this->go->AddGameObject({ 24, 0 });
		auto text = go->AddComponent<Text>();
		text->color = { 250, 0, 0 };
		text->text = "1UP";
		oneUpHint = go->AddComponent<BlinkingText>();
		oneUpHint->text = text;
		oneUpHint->enabled = false;
	}

	// HIGH SCORE TITLE UI
	{
		auto go = this->go->AddGameObject({ 72, 0 });
		auto text = go->AddComponent<Text>();
		text->text = "HIGH SCORE";
		text->color = { 250, 0, 0 };
	}

	// HIGH SCORE UI
	{
		auto go = this->go->AddGameObject({ 88, 8 });
		auto text = go->AddComponent<Text>();
		text->text = "000000";

		highScoreUI = text;
		utils::setScoreString(highScoreUI->text, highScore);
	}

	// START SCREEN
	{
		auto go = this->go->AddGameObject({});
		startScreen = go->AddComponent<StartScreen>();
		startScreen->enabled = true;
	}
}

void App::Update(float dt)
{
	switch (state)
	{
	case App::State::StartScreen:
	{
		if (startScreen->startGame)
		{
			state = State::Game;
			
			startScreen->go->Remove();

			auto go = this->go->AddGameObject({});
			game = go->AddComponent<Game>();
			game->AddObserver(this);
			game->levelTexture = levelTexture;
			game->spritesTexture = spritesTexture;
			game->music = gameMusic;

			oneUpHint->enabled = true;
			currentScore = 0;
			utils::setScoreString(currentScoreUI->text, currentScore);
		}

		if (input->GetKeyDown(SDL_SCANCODE_ESCAPE))
			//engine->quit();
			root->Remove();
	}
	break;
	case App::State::Game:
	{
		if (input->GetKeyDown(SDL_SCANCODE_ESCAPE) || game->endGame)
		{
			state = State::StartScreen;

			game->go->Remove();

			auto go = this->go->AddGameObject({});
			startScreen = go->AddComponent<StartScreen>();
		
			oneUpHint->enabled = false;

			audio->StopMusic();
		}

		if (game->wonLevel)
		{
			game->go->Remove();

			auto go = this->go->AddGameObject({});
			game = go->AddComponent<Game>();
			game->levelTexture = levelTexture;
			game->spritesTexture = spritesTexture;
			game->music = gameMusic;
			game->AddObserver(this);
			utils::setScoreString(currentScoreUI->text, currentScore);
		}
	}
	break;
	}
}

void App::Destroy()
{
	resources->UnloadTexture(spritesTexture);
	resources->UnloadTexture(levelTexture);
	resources->UnloadMusic(gameMusic);
}


void App::OnNotify(ScoreEvent e)
{
	currentScore += e.addScore;
	utils::setScoreString(currentScoreUI->text, currentScore);
	if (currentScore > highScore)
	{
		highScore = currentScore;
		utils::setScoreString(highScoreUI->text, highScore);
	}
}
