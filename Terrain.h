#pragma once
#include <SDL_image.h>
#include <string>
#include "RenderUtils.h"
#include <vector>
#include <iostream>
#include "Texture.h"
using namespace GE;

class Terrain {
public:
	Terrain(const std::string& heightmapFile, Texture* t , float s, float hS);

	unsigned int getVBO() { return vbo; }
	unsigned int getIBO() { return ibo; }

	Texture* getTexture() { return texture; }

	unsigned int getIndexCount() { return indexCount; }

private:
	Texture* texture;
	unsigned int vbo, ibo, indexCount;

	float scaling;
	float heightScaling;
};