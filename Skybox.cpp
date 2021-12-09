#include "SkyboxRenderer.h"
#include "ShaderUtils.h"
#include <SDL_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

namespace GE {
	struct CubeVertex {
		float x, y, z;

		CubeVertex() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		CubeVertex(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}
	};

	const float SIDE = 500000.0f;

	CubeVertex cube[] = {
		// Front face
		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),

		// Back face
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, SIDE),

		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, SIDE),

		// Left face
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, -SIDE),

		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),

		// Right face
		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),

		// Top face
		CubeVertex(-SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE,  SIDE),

		// Bottom face
		CubeVertex(-SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),
	};

	void SkyboxRenderer::createCubemap(vector<string> filenames) {
		//creates the name of the cubemap and assigns it as the target
		glGenTextures(1, &skyboxCubeMapName);

		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMapName);

		//loading textures
		for (int faceNum = 0; faceNum < 6; faceNum++) {
			SDL_Surface* surfaceImage = IMG_Load(filenames[faceNum].c_str());

			//checks the load is ok
			if (surfaceImage == nullptr) {
				return;
			}

			GLenum format = surfaceImage->format->format;

			switch (format) {
			case SDL_PIXELFORMAT_RGBA32: format = GL_RGBA;
				break;
			case SDL_PIXELFORMAT_RGB24: format = GL_RGB;
				break;
				//more formats and easier parsing needed to be added here
			default:
				format = GL_RGB;
				break;
			}

			//copies pixel data from the sdl_surface to the opengl texture
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceNum, 0,
				format,
				surfaceImage->w, surfaceImage->h,
				0,
				format,
				GL_UNSIGNED_BYTE,
				surfaceImage->pixels);

			SDL_FreeSurface(surfaceImage);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void SkyboxRenderer::createCubeVBO() {
		//vertex buffers for the object
		glGenBuffers(1, &vboSkybox);
		glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);

		//transfer the vertices to the graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);
	}

	void SkyboxRenderer::createSkyboxProgram() {
		//shader code below with fragment source code following
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"out vec3 texCoord;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main() {\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"v = projection * view * v;\n"
			"gl_Position = v;\n"
			"texCoord = vertexPos3D;\n"
			"}\n"
		};

		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec3 texCoord;\n"
			"uniform samplerCube sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"fragmentColour = vec4(texture(sampler, texCoord).rgb, 1.0f);\n"
			"}\n"
		};

		bool result = compileProgram(V_ShaderCode, F_ShaderCode, &skyboxProgramId);

		if (!result) {
			cerr << "Failed to create Skyboc, check the console and likely the shaders ya doof" << endl;
			return;
		}

		vertexLocation = glGetAttribLocation(skyboxProgramId, "vertexPos3D");

		if (vertexLocation == -1) {
			cerr << "Problem getting the vertex3Dpos... ya doof" << endl;
		}

		viewUniformId = glGetUniformLocation(skyboxProgramId, "view");
		projectionUniformId = glGetUniformLocation(skyboxProgramId, "projection");
	}

	void SkyboxRenderer::draw(Camera* cam) {
		bool isDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);

		glDisable(GL_DEPTH_TEST);

		glm::mat4 camView = cam->getViewMatrix();
		glm::mat4 projection = cam->getProjectionMatrix();

		//zero out the translation to ensure correct orientation
		camView[3][0] = 0.0f;
		camView[3][1] = 0.0f;
		camView[3][2] = 0.0f;

		glUseProgram(skyboxProgramId);

		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(camView));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projection));

		glEnableVertexAttribArray(vertexLocation);

		glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);

		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, x));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMapName);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / sizeof(CubeVertex));

		glDisableVertexAttribArray(vertexLocation);

		glUseProgram(0);

		if (isDepthTestEnabled) {
			glEnable(GL_DEPTH_TEST);
		}
	}

	void SkyboxRenderer::destroy() {
		glDeleteProgram(skyboxProgramId);

		glDeleteBuffers(1, &vboSkybox);

		glDeleteTextures(1, &skyboxCubeMapName);
	}
}