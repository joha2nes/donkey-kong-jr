#pragma once

#include <map>
#include "SDL.h"
#include "SDL_ttf.h"

#include "interfaces/renderer.h"
#include "interfaces/resources.h"

struct Sprite;

class RenderSystem : public Renderer
{
public:
	static const int numLayers = 5;

private:
	SDL_Renderer* renderer;
	SDL_Texture* layers[numLayers];
	glm::ivec2 resolution;
	int pixelSize;
	TTF_Font* font;
	unsigned int availableTextureID;
	std::map<Texture, SDL_Texture*> loadedTextures;
	std::map<const char*, Texture> filePathTexture;

public:
	bool Initialize(SDL_Renderer* renderer, glm::ivec2 resolution, int pixelSize);
	void Terminate();

	void Clear();
	void Present();

	void Draw(const Sprite* sprite, int layer, const glm::ivec2& position, glm::bvec2 flip);
	void Draw(const std::string& text, int layer, const glm::ivec2& position, const glm::uvec3 color);
	void DrawLine(glm::vec2 a, glm::vec2 b);

	Texture LoadTexture(const char* filePath);
	void UnloadTexture(Texture);
};