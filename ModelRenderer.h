#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Terrain.h"

namespace GE {
	class ModelRenderer {
	public:
		ModelRenderer(Camera* c);

		virtual ~ModelRenderer();

		// Init method to create shaders and VBO
		void init();

		// Update method to update object state, e.g. animation
		void update();

		// Draw method to render the triangle
		void draw(Model* model, Texture* material, float pos_x, float pos_y, float pos_z);

		void drawTerrain(Terrain* terrain, float pos_x, float pos_y, float pos_z);

		// Release method to free up objects
		void destroy();

		// Accessors
		// Position
		float getPosX() {
			return pos_x;
		}
		float getPosY() {
			return pos_y;
		}
		float getPosZ() {
			return pos_z;
		}
		// Rotation
		float getRotX() {
			return rot_x;
		}
		float getRotY() {
			return rot_y;
		}
		float getRotZ() {
			return rot_z;
		}
		// Scale
		float getScaleX() {
			return scale_x;
		}
		float getScaleY() {
			return scale_y;
		}
		float getScaleZ() {
			return scale_z;
		}

		// Mutator methods
		void setPos(float x, float y, float z) {
			pos_x = x;
			pos_y = y;
			pos_z = z;
		}
		void setRot(float rx, float ry, float rz) {
			rot_x = rx;
			rot_y = ry;
			rot_z = rz;
		}
		void setScale(float sx, float sy, float sz) {
			scale_x = sx;
			scale_y = sy;
			scale_z = sz;
		}

		void setMaterial(Texture* mat) {
			material = mat;
		}

	private:
		// Member fields
		// Program object that contains the shaders
		GLuint programId;

		// Stores the attribute to select into the pipeline
		// Links the triangle certices to the pipeline
		GLint vertexPos3DLocation;

		// Link to vColour attribute which receives a colour and
		// passes to fColour for fragment shader
		GLint vertexUVLocation;

		// Stores the triangle VBO, gets moved from this code to GFX memory
		GLuint vboModel;

		// Location, rotation and scale variables
		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;
		float scale_x, scale_y, scale_z;

		float pi;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;

		Model* model;
		Camera* cam;

		Texture* material;
	};
}