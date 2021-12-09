#pragma once

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include "Camera.h"

using namespace std;

namespace GE {
	class SkyboxRenderer {
	public:
		SkyboxRenderer(string front_fname, string back_fname,
			string right_fname, string left_fname,
			string top_fname, string bottom_fname) {
			vector<string> filenames;

			filenames.push_back(right_fname);
			filenames.push_back(left_fname);
			filenames.push_back(top_fname);
			filenames.push_back(bottom_fname);
			filenames.push_back(front_fname);
			filenames.push_back(back_fname);

			createCubemap(filenames);
			createCubeVBO();
			createSkyboxProgram();
		}

		~SkyboxRenderer() {}

		void draw(Camera* cam);

		void destroy();

	private:
		void createCubemap(vector<string> filenames);
		void createCubeVBO();
		void createSkyboxProgram();
	private:
		GLuint skyboxCubeMapName;

		GLuint skyboxProgramId;

		GLint vertexLocation;

		GLuint vboSkybox;

		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;
	};
}