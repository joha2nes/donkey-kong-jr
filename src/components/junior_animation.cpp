#include "junior_animation.h"

#include "component.h"
#include "components/junior.h"

#include "data/sprite.h"

void JuniorAnimation::Create()
{
	time = 0.f;
	player = go->GetComponent<JuniorController*>();

	auto spritesTexture = resources->LoadTexture("assets/sprites.png");
	moveLeft = SpriteAnimation({
		{ 0.07f, { spritesTexture, { 2 * 32, 0, 32, 16 }, {}, { -5, 0 } } },
		{ 0.07f, { spritesTexture, { 3 * 32, 0, 32, 16 }, {}, { -5, 0 } } },
		{ 0.07f, { spritesTexture, { 4 * 32, 0, 32, 16 }, {}, { -5, 0 } } },
		});
	moveRight = SpriteAnimation({
		{ 0.07f, { spritesTexture, { 2 * 32, 0, 32, 16 }, { true, false }, { -10, 0 } } },
		{ 0.07f, { spritesTexture, { 3 * 32, 0, 32, 16 }, { true, false }, { -10, 0 } } },
		{ 0.07f, { spritesTexture, { 4 * 32, 0, 32, 16 }, { true, false }, { -10, 0 } } },
		{ 0.07f, { spritesTexture, { 3 * 32, 0, 32, 16 }, { true, false }, { -10, 0 } } },
		});
	jumpLeft = SpriteAnimation({
		{ 0.07f, { spritesTexture, { 5 * 32, 0, 32, 16 }, {}, { -10, 0 } } },
		});
	jumpRight = SpriteAnimation({
		{ 0.07f, { spritesTexture, { 5 * 32, 0, 32, 16 }, { true, false }, { -5, 0 } } },
		});
	climbAnimation = SpriteAnimation({
		{ 0.05f, { spritesTexture, { 8 * 32, 0, 32, 16 }, {}, { -15, 0 } } },
		{ 0.20f, { spritesTexture, { 7 * 32, 0, 32, 16 }, {}, { -15, 0 } } },
		});
	climb2Animation = SpriteAnimation({
		{ 0.14f, { spritesTexture, { 10 * 32, 0, 32, 16 }, {}, { -4, 0 } } },
		{ 0.14f, { spritesTexture, { 10 * 32, 0, 32, 16 }, { true, false }, { -4, 0 } } },
		});
	deathAnimation = SpriteAnimation({
		{ 0.2f, { spritesTexture, { 12 * 32, 0, 32, 16 }, {}, { -4, 0 } } },
		{ 0.2f, { spritesTexture, { 13 * 32, 0, 32, 16 }, {}, { -4, 0 } } },
		});
}

void JuniorAnimation::Update(float _)
{
	auto dt = Component::time->GetUnscaledDeltaTime();
	auto vel = player->getVelocity();

	time +=
		player->IsDead() ? dt :
		player->state == JuniorState::Climbing ? (vel.y < 0.f) * dt :
		player->state == JuniorState::ClimbingStretched ? (vel.y != 0.f) * dt :
		player->state == JuniorState::Running ? (vel.x != 0.f) * dt :
		player->state == JuniorState::Jumping ? 0.f :
		dt;

	auto facingLeft = player->getDirection() == Direction::Left;

	auto anim =
		player->IsDead() ? &deathAnimation :
		player->state == JuniorState::Climbing ? &climbAnimation :
		player->state == JuniorState::ClimbingStretched ? &climb2Animation :
		player->state == JuniorState::Jumping ? (facingLeft ? &jumpLeft : &jumpRight) :
		player->state == JuniorState::Running ? (facingLeft ? &moveLeft : &moveRight) :
		nullptr;

	auto sprite = anim->EvalFrameLoop(time);

	auto flip = player->state == JuniorState::Climbing ?
		glm::bvec2(player->GetClimbSide() == ClimbSide::Left, false) :
		glm::bvec2(false);

	renderer->Draw(sprite, 3, go->position + (glm::vec2)flip * glm::vec2{ 14.f, 0.f}, flip);
}
