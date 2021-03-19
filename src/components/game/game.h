#pragma once

#include "component.h"
// NOTE: include for Scoring interface
#include "components/fruit.h"
#include "data/sprite.h"

class Path;
class SpriteGrid;
class Liana;
class JuniorController;
class Text;

struct NewPath;

class Game
	: public Component
	, public Observer<ScoreEvent>
	, public Subject<ScoreEvent>
{
	Path* blueSnapjawPath;
	int bonus;
	float bonusDecreaseTimer;
	Text* bonusUI;
	JuniorController* player;
	Liana* goalLiana;
	SpriteGrid* background;

	Sprite waterSprite;
	Sprite grassSprite;
	Sprite grassLSprite;
	Sprite grassRSprite;
	Sprite stemSprite;
	Sprite vine0Sprite;
	Sprite vine1Sprite;
	Sprite woodSprite;

	AudioClip pickupFallSound;

	float endTimer;

public:
	Texture levelTexture;
	Texture spritesTexture;

	Music music;
	bool endGame;
	bool wonLevel;

	// Component
	void Create();
	void Update(float dt);
	void Destroy();

	// Scoring
	void OnNotify(ScoreEvent);

	void AddWater();
	void AddWoodPlatform(glm::ivec2 start, unsigned int length);
	void AddGrassPlatform(glm::ivec2 start, unsigned int length);
	void AddPlatform(Sprite* spriteL, Sprite* spriteM, Sprite* spriteR, glm::ivec2 start, unsigned int length);
	void AddFruit(int spriteRegionOffsetX, glm::ivec2 coord);
	Liana* AddLiana(glm::ivec2 start, unsigned int length);
	void AddRedSnapjaw(const Path& path);
};