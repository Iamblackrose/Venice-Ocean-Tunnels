#include "GameEngine.h"
#include <iostream>

namespace GE {
	bool keyStates[4] = { 0,0,0,0 };

	int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;


	GameEngine::GameEngine() {
		w = 1280;
		h = 1024;
		windowflags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	}
	GameEngine::GameEngine(int _w, int _h) {
		w = _w;
		h = _h;
		windowflags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	}
	GameEngine::~GameEngine() {

	}
	bool GameEngine::init(bool vsync) {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Unable to initialise SDL! SDL error: " << SDL_GetError() << std::endl;
			return false;
		}
		// Set the OpenGL version for the program
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // OpenGL 3+
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1); // OpenGL 3.1
		// Set the type of profile which is core meaning modern OpenGL
		// means no legacy features for backwards compatibility
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Create the window frame and features
		// Fixed size window created for OpenGL to render to
		window = SDL_CreateWindow("Game", 50, 50, w, h, windowflags);
		// Check window was created: http://www.cplusplus.com/reference/cassert/assert/
		assert(window);

		// Create the OpenGL context and link to the window object
		// Context represents OpenGL for program such as objects and drawing
		glContext = SDL_GL_CreateContext(window);
		// Check the context object was created
		assert(glContext);
		// Check we are setup with the correct GL version
		std::cout << "glGetString (GL_VERSION) returns: " << glGetString(GL_VERSION) << std::endl;

		// Initialise GLEW. GLEW solves a problem with OpenGL on windows
		// OpenGL version supported on Windows is 1.1 (from 1997)
		// GLEW binds latest extensions that can be used
		GLenum status = glewInit();
		// Check GLEW initialised
		if (status != GLEW_OK) {
			std::cerr << "Error initialising GLEW! Error: " << glewGetErrorString(status) << std::endl;
			return false;
		}

		// Try to turn on VSync, if requested
		if (vsync) {
			if (SDL_GL_SetSwapInterval(1) != 0) {
				std::cerr << "Warning: unable to set VSync! Error: " << SDL_GetError() << std::endl;
				return false;
			}
		}

		// Create camera object
		cam = new Camera(glm::vec3(0.0f, 0.0f, 5.0f),	// cam position
			glm::vec3(0.0f, 0.0f, 0.0f),				// cam look at
			glm::vec3(0.0f, 1.0f, 0.0f),				// cam up direction
			90, w / h, 0.1f, 5000000.0f);					// fov, aspect ratio, near and far clip planes

		m = loadFromFile(".\\assets/Cyberpunk1.obj");
		m2 = loadFromFile(".\\assets/Cyberpunk2.obj");
		m3 = loadFromFile(".\\assets/Cyberpunk3.obj");
		m4 = loadFromFile(".\\assets/Cyberpunk4.obj");
		m5 = loadFromFile(".\\assets/Cyberpunk5.obj");
		m6 = loadFromFile(".\\assets/Cyberpunk6.obj");
		m7 = loadFromFile(".\\assets/Cyberpunk7.obj");
		m8 = loadFromFile(".\\assets/Cyberpunk8.obj");
		m9 = loadFromFile(".\\assets/Cyberpunk9.obj");
		m10 = loadFromFile(".\\assets/Cyberpunk10.obj");
		m11 = loadFromFile(".\\assets/Cyberpunk11.obj");
		m12 = loadFromFile(".\\assets/Cyberpunk12.obj");
		m13 = loadFromFile(".\\assets/Cyberpunk13.obj");
		m14 = loadFromFile(".\\assets/Cyberpunk14.obj");
		m15 = loadFromFile(".\\assets/Cyberpunk15.obj");

		mat = new Texture(".\\assets/aleeygorund_albedo.jpg");
		mat2 = new Texture(".\\assets/bodyboat_albedo.jpg");
		mat3 = new Texture(".\\assets/water_albedo.jpg");
		mat4 = new Texture(".\\assets/wall_albedo.jpg");
		mat5 = new Texture(".\\assets/building3_albedo.jpg");
		mat6 = new Texture(".\\assets/building2_albedo.jpg");
		mat7 = new Texture(".\\assets/building_albedo.jpg");
		mat8 = new Texture(".\\assets/tunnel_albedo.jpg");
		mat9 = new Texture(".\\assets/banner1_albedo.jpg");
		mat10 = new Texture(".\\assets/ground_albedo.jpg");
		mat11 = new Texture(".\\assets/plant1_albedo.jpg");
		mat12 = new Texture(".\\assets/Dome1_albedo.jpg");
		mat13 = new Texture(".\\assets/Bottle_albedo.jpg");
		mat14 = new Texture(".\\assets/lambert3_albedo.jpg");
		mat15 = new Texture(".\\assets/pot_albedo.jpg");

		mr = new ModelRenderer(cam);

		mr->setScale(0.05f, 0.05f, 0.05f);
		mr->init();

		skybox = new SkyboxRenderer("front.jpg", "back.jpg", "right.jpg", "left.jpg", "top.jpg", "bottom.jpg");

		terrainTexture = new Texture("assets/terrain-texture.png");
		terrain = new Terrain("assets/terrain-heightmap.png", terrainTexture, 200.0f, 6.0f);

		bbTex = new Texture(".\\tree.png");

		bb = new Billboard(bbTex);

		bb->setScaleX(100.0f);
		bb->setScaleY(100.0f);

		bb->setZ(-100.0f);

		bbr = new BillboardRenderer();

		bbr->init();


		//Error pulling stuff
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "Error: " << err << std::endl;
		}

		return true;
	}

	// Check if the window was closed and SDL_QUIT message was resultant
	bool GameEngine::keep_running() {
		// Need to do this to keep the event queue up to date
		SDL_PumpEvents();

		SDL_Event evt;

		// Check for quit event
		if (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
			// If user quit the program, then return false
			return false;
		}

		// If no SDL_QUIT event in event queue then keep running
		return true;
	}

	void GameEngine::processInput() {
		const float camSpeed = 0.5f;
		const float mouseSens = 0.25f;

		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);

		float diffx = mouse_x - cam->getOldMouseX();
		float diffy = cam->getOldMouseY() - mouse_y;

		cam->setYaw((cam->getYaw() + diffx) * mouseSens);
		cam->setPitch((cam->getPitch() + diffy) * mouseSens);

		glm::vec3 direction;
		direction.x = cos(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		direction.y = sin(glm::radians(cam->getPitch()));
		direction.z = sin(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		cam->setTarget(glm::normalize(direction));

		SDL_Event e;

		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode)	{
				case SDL_SCANCODE_UP:
					keyStates[UP] = true;
					break;
				case SDL_SCANCODE_DOWN:
					keyStates[DOWN] = true;
					break;
				case SDL_SCANCODE_LEFT:
					keyStates[LEFT] = true;
					break;
				case SDL_SCANCODE_RIGHT:
					keyStates[RIGHT] = true;
					break;
				}
			}
			else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
					keyStates[UP] = false;
					break;
				case SDL_SCANCODE_DOWN:
					keyStates[DOWN] = false;
					break;
				case SDL_SCANCODE_LEFT:
					keyStates[LEFT] = false;
					break;
				case SDL_SCANCODE_RIGHT:
					keyStates[RIGHT] = false;
					break;
				}
			}
		}

		if (keyStates[UP]) {
			cam->setPos(cam->getPos() + cam->getTarget() * camSpeed);
		}
		if (keyStates[DOWN]) {
			cam->setPos(cam->getPos() - cam->getTarget() * camSpeed);
		}
		if (keyStates[LEFT]) {
			cam->setPos(cam->getPos() - glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed);
		}
		if (keyStates[RIGHT]) {
			cam->setPos(cam->getPos() + glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed);
		}

		cam->updateCamMatrices();

		cam->setOldMouseX(w / 2);
		cam->setOldMouseY(h / 2);

	}

	// Update method which updates the game logic
	// Used to invoke GE object update methods
	void GameEngine::update() {
		mr->update();

		//mr2->setPos(0.0f, sinf(SDL_GetTicks()*0.001) - 2, 0.0f);
	}

	// Draw method. Used to render scenes to the window frame
	// For now, just clears the background
	void GameEngine::draw() {
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox->draw(cam);

		// Render the VBOs
		mr->draw(m, mat, 0.0f, 0.0f, 0.0f);
		mr->draw(m2, mat2, 0.0f, sinf(SDL_GetTicks() * 0.001) - 2, 0.0f);
		mr->draw(m3, mat3, 0.0f, 0.0f, 0.0f);
		mr->draw(m4, mat4, 0.0f, 0.0f, 0.0f);
		mr->draw(m5, mat5, 0.0f, 0.0f, 0.0f);
		mr->draw(m6, mat6, 0.0f, 0.0f, 0.0f);
		mr->draw(m7, mat7, 0.0f, 0.0f, 0.0f);
		mr->draw(m8, mat8, 0.0f, 0.0f, 0.0f);
		mr->draw(m9, mat9, 0.0f, 0.0f, 0.0f);
		mr->draw(m10, mat10, 0.0f, 0.0f, 0.0f);
		mr->draw(m11, mat11, 0.0f, 0.0f, 0.0f);
		mr->draw(m12, mat12, 0.0f, 0.0f, 0.0f);
		mr->draw(m13, mat13, 0.0f, 0.0f, 0.0f);
		mr->draw(m14, mat14, 0.0f, 0.0f, 0.0f);
		mr->draw(m15, mat15, 0.0f, 0.0f, 0.0f);

		mr->drawTerrain(terrain, 0.0f, -50.0f, 0.0f);

		bbr->draw(bb, cam);

		SDL_GL_SwapWindow(window);
	}

	// Shutdown method closes OpenGL and SDL
	// When additional objects are added, ensure these are also freed safely
	void GameEngine::shutdown() {
		// Release object renderers
		mr->destroy();

		skybox->destroy();
		
		// Release memory associate with camera and primitive renderers
		delete m;
		delete m2;
		delete m3;
		delete m4;
		delete m5;
		delete m6;
		delete m7;
		delete m8;
		delete m9;
		delete m10;
		delete m11;
		delete m12;
		delete m13;
		delete m14;
		delete m15;


		delete skybox;

		delete mr;

		delete mat;
		delete mat2;
		delete mat3;
		delete mat4;
		delete mat5;
		delete mat6;
		delete mat7;
		delete mat8;
		delete mat9;
		delete mat10;
		delete mat11;
		delete mat12;
		delete mat13;
		delete mat14;
		delete mat15;


		delete cam;

		delete bbr;
		delete bb;
		delete bbTex;

		//SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_Quit();
	}

	// Helped to set text in the window title bar
	void GameEngine::setwindowtitle(const char* new_title) {
		SDL_SetWindowTitle(window, new_title);
	}

	// Helper function to display program information
	// Part of the namespace, but not the GameEngine class
	void display_info_message(const char* msg) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Basic Game Engine", msg, nullptr);
	}
}