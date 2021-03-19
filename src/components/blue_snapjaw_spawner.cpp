#include "blue_snapjaw_spawner.h"

#include "avancezlib.h"
#include "game_object.h"

#include "components/snapjaw.h"
#include "components/loop_animation.h"
#include "components/hazard.h"
#include "components/sprite_velocity_animation.h"

#include "data/sprite.h"
#include "data/sprite_animation.h"

void BlueSnapjawSpawner::Create()
{
	spawnTimer = spawnInterval;
}

void BlueSnapjawSpawner::Update(float dt)
{
	spawnTimer -= dt;

	if (spawnTimer <= 0.f)
	{
		spawnTimer = spawnInterval;

		auto go = this->go->AddGameObject({ 100, 56 });

		auto sj = go->AddComponent<BlueSnapjawBehavior>();
		sj->path = &path;
		sj->waterSplashParent = this->go;

		auto c = go->AddComponent<CollideComponent>();
		c->size = { 8, 8 };

		auto s = go->AddComponent<SpriteVelocityAnimation>();
		auto tex = resources->LoadTexture("assets/sprites.png");

		s->left = SpriteAnimation({
			{ 0.07f, { tex, { 12 * 16, 3 * 16, 16, 16 }, {}, { 0, -8 } } },
			{ 0.07f, { tex, { 13 * 16, 3 * 16, 16, 16 }, {}, { 0, -8 } } },
			});
		s->right = SpriteAnimation({
			{ 0.07f, { tex, { 12 * 16, 3 * 16, 16, 16 }, { true, false }, { 0, -8 } } },
			{ 0.07f, { tex, { 13 * 16, 3 * 16, 16, 16 }, { true, false }, { 0, -8 } } },
			});
		s->up = SpriteAnimation({
			{ 0.07f, { tex, { 14 * 16, 3 * 16, 16, 16 }, { false, true }, { -4, 0 } } },
			{ 0.07f, { tex, { 15 * 16, 3 * 16, 16, 16 }, { false, true }, { -4, 0 } } },
			});
		s->down = SpriteAnimation({
			{ 0.07f, { tex, { 14 * 16, 3 * 16, 16, 16 }, {}, { -4, -4 } } },
			{ 0.07f, { tex, { 15 * 16, 3 * 16, 16, 16 }, {}, { -4, -4 } } },
			});

		auto h = go->AddComponent<Hazard>();
	}
}