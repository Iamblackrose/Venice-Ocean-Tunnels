#include "Terrain.h"

Terrain::Terrain(const std::string& heightmapFile, Texture* t, float s, float hS): scaling(s), heightScaling(hS), texture(t){
	int width, height, channels, iteration{};
	SDL_Surface* image = IMG_Load(heightmapFile.c_str());
	width = image->h;
	height = image->w;
	channels = image->format->BytesPerPixel;
	unsigned char* imageData = (unsigned char*)image->pixels;

	std::vector<GE::Vertexx> vertices;
	float x, y, z, u, v;
	for (int row = 0; row < height; row++) {
		for (int column = 0; column < width; column++) {
			unsigned char redPixel = imageData[((row * width) + column) * channels];
			unsigned char greenPixel = imageData[((row * width) + column) * channels + 1];
			unsigned char bluePixel = imageData[((row * width) + column) * channels + 2];
			int meanPixelValue = (redPixel + greenPixel + bluePixel) / 3;

			x = (float)column / (float)(width - 1) * scaling;
			z = (float)row / (float)(height - 1) * scaling;
			y = meanPixelValue / 255.0f * heightScaling;

			u = (float)column / (float)(width - 1);
			v = (float)row / (float)(height - 1);

			vertices.emplace_back(x, y, z, u, v);
		}
	}

	std::vector<unsigned int> indices;

	unsigned int c, n;
	for (int row = 0; row < height - 1; row++) {
		for (int column = 0; column < width - 1; column++) {
			c = row * width + column;
			n = (row + 1) * width + column;

			indices.emplace_back(c);
			indices.emplace_back(n);
			indices.emplace_back(c + 1);

			indices.emplace_back(c + 1);
			indices.emplace_back(n);
			indices.emplace_back(n + 1);
		}
	}

	indexCount = indices.size();
	vbo = GE::createVBO(vertices);
	ibo = GE::createIBO(indices);
	SDL_FreeSurface(image);
}