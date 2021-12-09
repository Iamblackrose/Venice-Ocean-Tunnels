#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Camera.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "SkyboxRenderer.h"
#include "Terrain.h"
#include "Billboard.h"
#include "BillboardRenderer.h"

namespace GE {
	class GameEngine {
	public:
		GameEngine();						// Class default constructor
		GameEngine(int _w, int _h);			// Class overloaded constructor
		virtual ~GameEngine();				// Virtual destructor
		bool init(bool vsync = false);		// Object initialisation, vsync default off
		bool keep_running();				// Window closed?
		void processInput();
		void update();						// Update Game Logic
		void draw();						// Render the frame
		void shutdown();					// Release objects and close safely

		void setwindowtitle(const char*);
		bool fullscreen = false;			// Logic handle for fullscreen mode
		int w, h;							// Window width and height
		int windowflags;					// Hold info on how to display the window
	private:
		// Privately encapsulated data
		// SDL window object to represent the frame
		SDL_Window* window;

		// SDL GL context to represent OpenGL in the program
		SDL_GLContext glContext;

		// Camera
		Camera* cam;

		// Object renderers
		
		ModelRenderer* mr;

		Model* m;
		Model* m2;
		Model* m3;
		Model* m4;
		Model* m5;
		Model* m6;
		Model* m7;
		Model* m8;
		Model* m9;
		Model* m10;
		Model* m11;
		Model* m12;
		Model* m13;
		Model* m14;
		Model* m15;

		Texture* mat;
		Texture* mat2;
		Texture* mat3;
		Texture* mat4;
		Texture* mat5;
		Texture* mat6;
		Texture* mat7;
		Texture* mat8;
		Texture* mat9;
		Texture* mat10;
		Texture* mat11;
		Texture* mat12;
		Texture* mat13;
		Texture* mat14;
		Texture* mat15;


		SkyboxRenderer* skybox;

		Texture* terrainTexture;
		Terrain* terrain;


		Texture* bbTex;
		Billboard* bb;
		BillboardRenderer* bbr;

	};

	// Helper Function
	void display_info_message(const char*);
}