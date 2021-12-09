#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

namespace GE {
	class Texture {
	public:
		Texture(std::string filename) {
			width = 0;
			height = 0;

			textureName = 0;

			loadTexture(filename);
		}

		~Texture() {

		}

		int getWidth() {
			return width;
		}

		int getHeight() {
			return height;
		}

		Uint32 getFormat() {
			return format;
		}

		GLuint getTextureName() {
			return textureName;
		}

	private:
		void loadTexture(std::string filename);

		int width;
		int height;

		Uint32 format;

		GLuint textureName;

	};
}