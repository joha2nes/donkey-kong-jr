#include "sprite_grid.h"

#include "interfaces/renderer.h"

void SpriteGrid::Paint(Sprite* sprite, glm::ivec2 min, glm::ivec2 max)
{
	for (int yi = min.y; yi <= max.y; yi++)
	{
		if (yi < 0 || yi >= height)
			continue;

		for (int xi = min.x; xi <= max.x; xi++)
		{
			if (xi < 0 || xi >= width)
				continue;

			auto a = grid[yi];
			auto b = grid[xi];

			grid[yi][xi] = sprite;
		}
	}
}

void SpriteGrid::Update(float dt)
{
	for (int yi = 0; yi < 32; yi++)
	{
		for (int xi = 0; xi < 28; xi++)
		{
			auto sprite = grid[yi][xi];
			if (sprite == nullptr)
				continue;

			renderer->Draw(sprite, 0, { xi * 8, yi * 8 }, {});
		}
	}
}