#include "game.h"

#include "game_object.h"
#include "avancezlib.h"

#include "glm/glm.hpp"

#include "components/junior_animation.h"
#include "components/sprite_component.h"
#include "components/junior.h"
#include "components/fruit.h"
#include "components/snapjaw.h"
#include "components/hazard.h"
#include "components/loop_animation.h"
#include "components/blue_snapjaw_spawner.h"
#include "components/sprite_velocity_animation.h"
#include "components/text.h"
#include "components/blinking_text.h"
#include "components/sprite_grid.h"

#include "data/observer.h"
#include "data/subject.h"
#include "data/path.h"

#include "data/utils.h"
#include "components\junior_audio.h"

void Game::Create()
{
	bonus = 5000;
	bonusDecreaseTimer = 2.5f;
	endTimer = 4.f;
	endGame = false;
	wonLevel = false;

	audio->LoopMusic(music);

	woodSprite = Sprite{ levelTexture, { 0 * 8, 0, 8, 8 }, {}, {} };
	stemSprite = Sprite{ levelTexture, { 1 * 8, 0, 8, 8 }, {}, {} };
	grassSprite = Sprite{ levelTexture, { 2 * 8, 0, 8, 8 }, {}, {} };
	grassLSprite = Sprite{ levelTexture, { 3 * 8, 0, 8, 8 }, { true, false }, {} };
	grassRSprite = Sprite{ levelTexture, { 3 * 8, 0, 8, 8 }, {}, {} };
	waterSprite = Sprite{ levelTexture, { 4 * 8, 0, 8, 8 }, {}, {} };
	vine0Sprite = Sprite{ levelTexture, { 5 * 8, 0, 8, 8 }, {}, {} };
	vine1Sprite = Sprite{ levelTexture, { 6 * 8, 0, 8, 8 }, {}, {} };

	pickupFallSound = resources->LoadAudioClip("assets/audio/02_Coin.wav");

	// LEVEL
	{
		auto obj = go->AddGameObject({});
		background = obj->AddComponent<SpriteGrid>();
	}

	goalLiana = AddLiana({ 11, 3 }, 3);

	// PLAYER
	{
		auto go = this->go->AddGameObject({ 0, 228 });
		
		auto render = go->AddComponent<JuniorAnimation>();

		player = go->AddComponent<JuniorController>();
		player->goalLiana = goalLiana;

		auto paudio = go->AddComponent<JuniorAudio>();
		paudio->junior = player;

		auto collider = go->AddComponent<CollideComponent>();
		collider->size = { 16, 16 };
	}

	// key
	{
		auto go = this->go->AddGameObject({ 88, 17 });
		auto anim = go->AddComponent<LoopAnimation>();
		anim->animation = SpriteAnimation({
			{ 0.5, { spritesTexture, { 9 * 16, 4 * 16, 16, 16 }, {}, { -4, 0 } } },
			{ 0.5, { spritesTexture, { 10 * 16, 4 * 16, 16, 16 }, {}, { -4, 0 } } },
			});
	}

	// bonus ui
	{
		auto bonusFrame = this->go->AddGameObject({ 152, 24 });
		auto s = bonusFrame->AddComponent<SpriteComponent>();
		s->sprite = { spritesTexture, { 320, 5 * 16, 64, 32 }, {}, {} };

		auto bonusObj = this->go->AddGameObject({ 161, 32 });
		auto bonusText = bonusObj->AddComponent<Text>();
		bonusText->text = "0000";
		utils::setScoreString(bonusText->text, bonus);

		bonusUI = bonusText;
	}

	// MARIO
	{
		auto mario = this->go->AddGameObject({ 54, 56 });
		auto anim = mario->AddComponent<LoopAnimation>();
		anim->animation = SpriteAnimation({
			{ 0.5f, { spritesTexture, { 0 * 16, 48, 16, 16 }, { true, false }, {} } },
			{ 0.5f, { spritesTexture, { 1 * 16, 48, 16, 16 }, { true, false }, {} } },
			{ 0.5f, { spritesTexture, { 2 * 16, 48, 16, 16 }, { true, false }, {} } },
			{ 0.5f, { spritesTexture, { 3 * 16, 48, 16, 16 }, { true, false }, {} } },
			{ 0.5f, { spritesTexture, { 4 * 16, 48, 16, 16 }, { true, false }, {} } },
			});
	}

	// donkey kong
	{
		auto pos = glm::vec2{ 8, 40 };

		auto dk = this->go->AddGameObject(pos);
		auto dkAnim = dk->AddComponent<LoopAnimation>();
		dkAnim->animation = SpriteAnimation({
			{ 1.5f, { spritesTexture, { 0 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
			{ 1.5f, { spritesTexture, { 1 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
			{ 1.5f, { spritesTexture, { 2 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
			{ 1.5f, { spritesTexture, { 3 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
			{ 1.5f, { spritesTexture, { 4 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
			{ 1.5f, { spritesTexture, { 5 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
			{ 1.5f, { spritesTexture, { 6 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
			});

		auto cage = this->go->AddGameObject(pos);
		auto cageAnim = dk->AddComponent<LoopAnimation>();
		cageAnim->animation = SpriteAnimation({
			{ 1.5f, { spritesTexture, { 32 + 7 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
			{ 1.5f, { spritesTexture, { 32 + 8 * 48, 16, 48, 32 }, {}, { -4, 0 } } },
		});
	}

	// blue snapjaw spawner
	//if (false)
	{
		auto go = this->go->AddGameObject({ 100, 60 });

		auto spawner = go->AddComponent<BlueSnapjawSpawner>();
		spawner->spawnInterval = 3.f;

		spawner->path.Add({ 8.f * glm::vec2{ 9.f, 8.f } }); // blueSnapjawPath 0
		spawner->path.Add({ 8.f * glm::vec2{ 0.f, 8.f } });            // top0 1
		spawner->path.Add({ 8.f * glm::vec2{ 3.f, 8.f } });            // top1 2
		spawner->path.Add({ 8.f * glm::vec2{ 11.f, 8.f } });           // top2 3
		spawner->path.Add({ 8.f * glm::vec2{ 15.f, 8.f } });           // top3 4
		spawner->path.Add({ 8.f * glm::vec2{ 17.f, 8.f } });           // top4 5
		spawner->path.Add({ 8.f * glm::vec2{ 17.f, 9.f } });        // middle0 6
		spawner->path.Add({ 8.f * glm::vec2{ 18.f, 9.f } });        // middle1 7
		spawner->path.Add({ 8.f * glm::vec2{ 21.f, 9.f } });        // middle2 8
		spawner->path.Add({ 8.f * glm::vec2{ 24.f, 9.f } });        // middle3 9
		spawner->path.Add({ 8.f * glm::vec2{ 0.f, 30.f } });        // bottom0 10
		spawner->path.Add({ 8.f * glm::vec2{ 3.f, 30.f } });        // bottom1 11
		spawner->path.Add({ 8.f * glm::vec2{ 11.f, 30.f } });       // bottom2 12
		spawner->path.Add({ 8.f * glm::vec2{ 15.f, 30.f } });       // bottom3 13
		spawner->path.Add({ 8.f * glm::vec2{ 18.f, 30.f } });       // bottom4 14
		spawner->path.Add({ 8.f * glm::vec2{ 21.f, 30.f } });       // bottom5 15
		spawner->path.Add({ 8.f * glm::vec2{ 24.f, 30.f } });       // bottom6 16
		spawner->path.Connect(0, 1);
		spawner->path.Connect(0, 2);
		spawner->path.Connect(0, 3);
		spawner->path.Connect(0, 4);
		spawner->path.Connect(0, 5);
		spawner->path.Connect(1, 10);
		spawner->path.Connect(2, 11);
		spawner->path.Connect(3, 12);
		spawner->path.Connect(4, 13);
		spawner->path.Connect(5, 6);
		spawner->path.Connect(6, 7);
		spawner->path.Connect(6, 8);
		spawner->path.Connect(6, 9);
		spawner->path.Connect(7, 14);
		spawner->path.Connect(8, 15);
		spawner->path.Connect(9, 16);
	}

	// red snapjaw 0
	//if (false)
	{
		Path path;
		path.Add({ 8.f * glm::vec2{ 4.f, 14.f } }); // top0 0
		path.Add({ 8.f * glm::vec2{ 6.f, 14.f } }); // top1 1
		path.Add({ 8.f * glm::vec2{ 7.f, 14.f } }); // top2 2 
		path.Add({ 8.f * glm::vec2{ 4.f, 20.f } }); // mid0 3
		path.Add({ 8.f * glm::vec2{ 6.f, 20.f } }); // mid1 4 
		path.Add({ 8.f * glm::vec2{ 9.f, 20.f } }); // mid2 5
		path.Add({ 8.f * glm::vec2{ 6.f, 26.f } }); // btm0 6
		path.Connect(0, 1);
		path.Connect(1, 0);
		path.Connect(1, 2);
		path.Connect(1, 4);
		path.Connect(2, 1);
		path.Connect(3, 4);
		path.Connect(4, 1);
		path.Connect(4, 3);
		path.Connect(4, 5);
		path.Connect(4, 6);
		path.Connect(5, 4);
		path.Connect(6, 4);
		AddRedSnapjaw(path);
	}

	// red snapjaw 1
	//if (false)
	{
		Path path;
		path.Add({ 8.f * glm::vec2{ 16.f, 9.f } }); // top0
		path.Add({ 8.f * glm::vec2{ 18.f, 9.f } }); // top1
		path.Add({ 8.f * glm::vec2{ 21.f, 9.f } }); // top2
		path.Add({ 8.f * glm::vec2{ 23.f, 9.f } }); // top3
		path.Add({ 8.f * glm::vec2{ 18.f, 24.f } }); // btm0
		path.Add({ 8.f * glm::vec2{ 21.f, 22.f } }); // btm1
		path.Connect(0, 1);
		path.Connect(1, 0);
		path.Connect(1, 2);
		path.Connect(1, 4);
		path.Connect(2, 1);
		path.Connect(2, 3);
		path.Connect(2, 5);
		path.Connect(3, 2);
		path.Connect(4, 1);
		path.Connect(5, 2);
		AddRedSnapjaw(path);
	}

	// red snapjaw 2
	//if (false)
	{
		Path path;
		path.Add({ 8.f * glm::vec2{ 24.f, 6.f } }); // top0 0 
		path.Add({ 8.f * glm::vec2{ 27.f, 6.f } }); // top1 1
		path.Add({ 8.f * glm::vec2{ 22.f, 17.f } });// mid0 2
		path.Add({ 8.f * glm::vec2{ 24.f, 17.f } });// mid1 3
		path.Add({ 8.f * glm::vec2{ 27.f, 17.f } });// mid2 4
		path.Add({ 8.f * glm::vec2{ 24.f, 24.f } });// btm0 5
		path.Add({ 8.f * glm::vec2{ 27.f, 24.f } });// btm1 6
		path.Connect(0, 3);
		path.Connect(1, 4);
		path.Connect(2, 3);
		path.Connect(3, 0);
		path.Connect(3, 2);
		path.Connect(3, 4);
		path.Connect(4, 1);
		path.Connect(4, 3);
		path.Connect(4, 6);
		path.Connect(5, 3);
		path.Connect(6, 4);
		AddRedSnapjaw(path);
	}

	// LEVEL

	AddFruit(0, { 3, 12 });
	AddFruit(2, { 11, 12 });
	AddFruit(1, { 15, 20 });
	AddFruit(0, { 18, 13 });
	AddFruit(2, { 17, 3 });

	AddGrassPlatform({ -1, 30 }, 8);
	AddGrassPlatform({ 10, 28 }, 4);
	AddGrassPlatform({ 15, 29 }, 3);
	AddGrassPlatform({ 19, 28 }, 4);
	AddGrassPlatform({ 24, 27 }, 4);

	AddWoodPlatform({ 7, 5 }, 3);
	AddWoodPlatform({ 0, 9 }, 17);
	AddWoodPlatform({ 16, 10 }, 8);
	AddWoodPlatform({ 4, 15 }, 4);
	AddWoodPlatform({ 22, 18 }, 6);
	AddWoodPlatform({ 4, 21 }, 6);

	auto rseed = rand();
	srand(1);
	AddLiana({ 0, 10 }, 17);
	AddLiana({ 3, 10 }, 16);
	AddLiana({ 6, 16 }, 5);
	AddLiana({ 6, 22 }, 5);
	AddLiana({ 11, 10 }, 15);
	AddLiana({ 15, 10 }, 9);
	AddLiana({ 17, 3 }, 3);
	AddLiana({ 18, 11 }, 14);
	AddLiana({ 21, 11 }, 12);
	AddLiana({ 24, 6 }, 12);
	AddLiana({ 24, 19 }, 6);
	AddLiana({ 27, 6 }, 12);
	AddLiana({ 27, 19 }, 6);
	srand(rseed);

	AddWater();
}

void Game::Destroy()
{
	resources->UnloadAudioClip(pickupFallSound);
}

void Game::Update(float dt)
{
	if (wonLevel)
		return;

	if (player->IsDead())
	{
		time->SetTimeScale(0.f);

		endTimer -= time->GetUnscaledDeltaTime();
		if (endTimer <= 0.f)
		{
			endGame = true;
			audio->StopMusic();
		}
	}
	else
	{
		time->SetTimeScale(1.f);

		bonusDecreaseTimer -= dt;
		if (bonusDecreaseTimer <= 0.f)
		{
			bonusDecreaseTimer = 2.5f;
			bonus = glm::max(0, bonus - 100);
			utils::setScoreString(bonusUI->text, bonus);
		}
	}

	if (player->onGoal)
	{
		Notify({ bonus });
		wonLevel = true;
	}
}

void Game::OnNotify(ScoreEvent e)
{
	Notify({ e.addScore });
}

void Game::AddWater()
{
	background->Paint(&waterSprite, glm::ivec2(0, 31), glm::ivec2(27, 31));
}

void Game::AddWoodPlatform(glm::ivec2 start, unsigned int length)
{
	AddPlatform(&woodSprite, &woodSprite, &woodSprite, start, length);
}

void Game::AddGrassPlatform(glm::ivec2 start, unsigned int length)
{
	background->Paint(
		&stemSprite,
		start + 1,
		glm::ivec2(start.x + length - 2, 31)
	);
	AddPlatform(&grassLSprite, &grassSprite, &grassRSprite, start, length);
}

void Game::AddPlatform(Sprite* spriteL, Sprite* spriteM, Sprite* spriteR, glm::ivec2 start, unsigned int length)
{
	for (int i = 0; i < length; i++)
	{
		auto sprite =
			i == 0 ? spriteL :
			i == length - 1 ? spriteR :
			spriteM;

		auto coord = start + glm::ivec2(i, 0);
		auto go = this->go->AddGameObject(8 * coord);

		background->Paint(sprite, coord, coord);

		auto collider = go->AddComponent<CollideComponent>();
		collider->size = { 8, 8 };

		auto platform = go->AddComponent<Platform>();
	}
}

void Game::AddFruit(int spriteRegionOffsetX, glm::ivec2 coord)
{
	auto size = glm::ivec2(16);
	auto go = this->go->AddGameObject(8 * coord - size / 4);
	auto fruit = go->AddComponent<Fruit>();
	auto trigger = go->AddComponent<CollideComponent>();
	auto renderer = go->AddComponent<SpriteComponent>();

	fruit->score400Sprite = { spritesTexture, { 23 * 16, 4 * 16, 16, 16 }, {}, {} };

	trigger->size = size;

	renderer->sprite = Sprite{ spritesTexture, { spriteRegionOffsetX * 16, 4 * 16, 16, 16 }, {}, {} };

	fruit->fallSound = pickupFallSound;
	fruit->AddObserver(this);
}

Liana* Game::AddLiana(glm::ivec2 start, unsigned int length)
{
	auto go = this->go->AddGameObject(8 * start);

	auto col = go->AddComponent<CollideComponent>();
	col->size = 8 * glm::ivec2{ 1, length };

	auto liana = go->AddComponent<Liana>();

	for (int i = 0; i < length - 1; i++)
	{
		
		auto sprite = rand() % 12 == 0 ? &vine1Sprite : &vine0Sprite;
		auto coord = start + glm::ivec2{ 0, i };
		background->Paint(sprite, coord, coord);
	}
	auto coord = start + glm::ivec2{ 0, length - 1 };
	background->Paint(&vine1Sprite, coord, coord);

	return liana;
}

void Game::AddRedSnapjaw(const Path& path)
{
	auto go = this->go->AddGameObject(path.nodes[0].pos);

	auto b = go->AddComponent<RedSnapjawBehavior>();
	b->path = path;

	auto s = go->AddComponent<SpriteVelocityAnimation>();
	s->left.AddFrame({ 0.07f, { spritesTexture, { 8 * 16, 3 * 16, 16, 16 }, {}, { 0, -8 } } });
	s->left.AddFrame({ 0.07f, { spritesTexture, { 9 * 16, 3 * 16, 16, 16 }, {}, { 0, -8 } } });
	s->right.AddFrame({ 0.07f, { spritesTexture, { 8 * 16, 3 * 16, 16, 16 }, { true, false }, { -4, -8 } } });
	s->right.AddFrame({ 0.07f, { spritesTexture, { 9 * 16, 3 * 16, 16, 16 }, { true, false}, { -4, -8 } } });
	s->up.AddFrame({ 0.07f, { spritesTexture, { 10 * 16, 3 * 16, 16, 16 }, { false, true }, { -4, 0 } } });
	s->up.AddFrame({ 0.07f, { spritesTexture, { 11 * 16, 3 * 16, 16, 16 }, { false, true }, { -4, 0 } } });
	s->down.AddFrame({ 0.07f, { spritesTexture, { 10 * 16, 3 * 16, 16, 16 }, {}, { -4, -4 } } });
	s->down.AddFrame({ 0.07f, { spritesTexture, { 11 * 16, 3 * 16, 16, 16 }, {}, { -4, -4 } } });

	auto col = go->AddComponent<CollideComponent>();
	col->size = { 8, 8 };

	auto h = go->AddComponent<Hazard>();
}
