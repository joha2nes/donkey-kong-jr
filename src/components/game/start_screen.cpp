#include "start_screen.h"

#include "glm/glm.hpp"

// NOTE: included for Time
#include "avancezlib.h"

#include "interfaces/renderer.h"
#include "interfaces/input.h"
#include "data/sprite.h"

void StartScreen::Create()
{
	state = State::Title;
	titleSprite = { resources->LoadTexture("assets/title.png"), { 0, 0, 224, 256 }, {}, {} };
	startGame = false;
}

void StartScreen::Update(float dt)
{
	time += Component::time->GetUnscaledDeltaTime();

	state = glm::fract(0.1f * time) < 0.5f ? State::Title : State::InsertCoin;

	switch (state)
	{
	case State::Title:
	{
		renderer->Draw(&titleSprite, 0, {}, {});
	}
	break;
	case State::InsertCoin:
	{
		renderer->Draw("  INSERT COIN  ", 1, { 48, 8*6 }, { 0, 0, 250 });
		renderer->Draw("PLAYER     COIN", 1, { 48, 8*8 }, { 250, 250, 0 });
		renderer->Draw("  1         1  ", 1, { 48, 8 * 10 }, { 0, 0, 250 });
		renderer->Draw("  2         2  ", 1, { 48, 8 * 12 }, { 0, 0, 250 });
		// TODO: draw high scores
		renderer->Draw("CREDIT 00", 1, { 144, 248 }, { 0, 250, 0 });
	}
	break;
	}

	if (input->GetKeyDown(SDL_SCANCODE_RETURN))
		startGame = true;
}
