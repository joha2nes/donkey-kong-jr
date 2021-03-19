#include "render_system.h"

#include "SDL_image.h"
#include "SDL_ttf.h"
#include "glm/glm.hpp"

#include "data/sprite.h"

bool RenderSystem::Initialize(SDL_Renderer* renderer, glm::ivec2 resolution, int pixelSize)
{
	this->renderer = renderer;
	this->resolution = resolution;
	this->pixelSize = pixelSize;

	for (int i = 0; i < numLayers; i++)
	{
		layers[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, resolution.x, resolution.y);
		SDL_SetTextureBlendMode(layers[i], SDL_BLENDMODE_BLEND);
	}

	font = TTF_OpenFont("assets/ARCADE.ttf", 8);

	availableTextureID = 0;

	return true;
}

void RenderSystem::Terminate()
{
	// remove render layers
	for (int i = 0; i < numLayers; i++)
	{
		SDL_DestroyTexture(layers[i]);
		layers[i] = nullptr;
	}

	// remove loaded textures
	for (auto p : loadedTextures)
	{
		SDL_DestroyTexture(p.second);
		p.second = nullptr;
	}
	loadedTextures.clear();

	filePathTexture.clear();

	// remove font
	TTF_CloseFont(font);
}

void RenderSystem::Clear()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	SDL_SetRenderTarget(renderer, nullptr);
	SDL_RenderClear(renderer);

	for (int i = 0; i < numLayers; i++)
	{
		SDL_SetRenderTarget(renderer, layers[i]);
		SDL_RenderClear(renderer);
	}
}

void RenderSystem::Present()
{
	SDL_SetRenderTarget(renderer, nullptr);

	SDL_Rect src{ 0, 0, resolution.x, resolution.y };
	SDL_Rect dst = SDL_Rect{ 0, 0, pixelSize * resolution.x, pixelSize * resolution.y };

	for (int i = 0; i < numLayers; i++)
		SDL_RenderCopy(renderer, layers[i], &src, &dst);

	SDL_RenderPresent(renderer);
}

void RenderSystem::Draw(const Sprite* sprite, int layer, const glm::ivec2& position, glm::bvec2 flip)
{
	SDL_SetRenderTarget(renderer, layers[glm::clamp(layer, 0, numLayers-1)]);

	auto r = sprite->region;
	SDL_Rect src{ r.x, r.y, r.w, r.h };
	SDL_Rect dst{
		(position.x + sprite->offset.x),
		(position.y + sprite->offset.y),
		r.w,
		r.h
	};
	auto flipx = (flip.x + sprite->flip.x) % 2;
	auto flipy = (flip.y + sprite->flip.y) % 2;
	auto rflip = SDL_RendererFlip((SDL_FLIP_HORIZONTAL * flipx) | (SDL_FLIP_VERTICAL * flipy));
	SDL_RenderCopyEx(renderer, loadedTextures[sprite->texture], &src, &dst, 0.0, NULL, rflip);
}

void RenderSystem::Draw(const std::string& text, int layer, const glm::ivec2& position, const glm::uvec3 color)
{
	SDL_SetRenderTarget(renderer, layers[glm::clamp(layer, 0, numLayers - 1)]);

	auto surface = TTF_RenderText_Solid(font, text.c_str(), { (Uint8)color.r, (Uint8)color.g, (Uint8)color.b });
	auto texture = SDL_CreateTextureFromSurface(renderer, surface);

	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

	SDL_Rect dst{ position.x, position.y, w, h };
	SDL_RenderCopy(renderer, texture, nullptr, &dst);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void RenderSystem::DrawLine(glm::vec2 a, glm::vec2 b)
{
	SDL_SetRenderTarget(renderer, layers[numLayers - 1]);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}

Texture RenderSystem::LoadTexture(const char* filePath)
{
	Texture tex;
	auto it = filePathTexture.find(filePath);
	if (it != filePathTexture.end())
	{
		tex = (*it).second;
	}
	else
	{
		tex = availableTextureID++;

		auto loadedSurface = IMG_Load(filePath);
		if (loadedSurface == nullptr)
			SDL_Log("Failed to load texture: %s", filePath);

		auto sdltex = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);

		loadedTextures.insert({ tex, sdltex });
		filePathTexture.insert({ filePath, tex });

		SDL_Log("Allocated texture: %s (%s)", std::to_string(tex).c_str(), filePath);
	}
	return tex;
}

void RenderSystem::UnloadTexture(Texture tex)
{
	auto it = loadedTextures.find(tex);
	if (it != loadedTextures.end())
	{
		auto sdltex = (*it).second;
		loadedTextures.erase(it);
		SDL_DestroyTexture(sdltex);
		const char* path = nullptr;
		for (auto p : filePathTexture)
		{
			if (p.second == tex)
			{
				path = p.first;
				break;
			}
		}
		filePathTexture.erase(path);

		SDL_Log("Deallocated texture: %s (%s)", std::to_string(tex).c_str(), path);
	}
}