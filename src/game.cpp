#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "meshmanager.h"
#include "texturemanager.h"
#include "entity.h"
#include "menu.h"
#include "bulletmanager.h"
#include "level.h"
#include "checkcollisions.h"
#include <cmath>

//some globals

float angle = 0;
RenderToTexture* rt = NULL;

EntityMesh* cielo = NULL;

Game* Game::instance = NULL;
const Uint8* Game::keystate = NULL;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things created here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	//keystate = NULL;

	mouse_locked = false;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;
    
    //SDL_SetWindowSize(window, 50, 50);

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	
	//Joystick
	SDL_Joystick* myJoystick = NULL;
	myJoystick = openJoystick(0);
	controller = new PlayerController();
	controller->myJoystick = myJoystick;

	//IA_manager
	motherIA = new IA_Manager();

	//create our cameras
	gameCamera = new Camera();
	gameCamera->lookAt(Vector3(0.f, 525.f, 100.f), Vector3(0.f, 500.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	gameCamera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 20000.f); //set the projection, we want to be perspective
	controller->camera = gameCamera;
	
	freeCamera = new Camera();
	freeCamera->lookAt(Vector3(0.f, 525.f, 100.f), Vector3(0.f, 500.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	freeCamera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 20000.f); //set the projection, we want to be perspective

	currentCamera = gameCamera;

	//create root and entities
	root = new Entity();
	Level* level = new Level();
	const char* levelFile = "level1.txt";
	if (!level->loadLevel(root, controller, motherIA, levelFile))
		std::cout << "Failed to load level" << levelFile << std::endl;

	cielo = new EntityMesh();
	cielo->name = "cielo";
	cielo->meshName = "cielo";
	cielo->textureNames.push_back("cielo");

	/*
	Menu* menu = new Menu();
	menus.push_back(menu);
	*/

	Entity::default_shader = new Shader;
	if( !Entity::default_shader->load("data/shaders/simple.vs","data/shaders/simple.fs") )
	{
		std::cout << "Shader not found or error" << std::endl;
		exit(0);
	}
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	cameraMode = false;
}

//what to do when the image has to be drawn
void Game::render(void)
{
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the clear color (the background color)
	glClearColor(0.11, 0.20, 0.25, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Put the camera matrices on the stack of OpenGL (only for fixed rendering)
	gameCamera->set();
	freeCamera->set();
	currentCamera->set();

	//Draw out world
	//drawGrid(500); //background grid
	
	cielo->model.setTranslation(currentCamera->eye.x, currentCamera->eye.y, currentCamera->eye.z);

	glDisable(GL_DEPTH_TEST);
	cielo->render(currentCamera);
	glEnable(GL_DEPTH_TEST);
	root->render(currentCamera);

	BulletManager::getInstance()->render(currentCamera);
	/*else //render using fixed pipeline (DEPRECATED, use for debug only)
	{
		glPushMatrix();
		m.multGL();
		getMesh("p38")->render();
		glPopMatrix();
	}*/
    
    glDisable( GL_BLEND );

	//example to render the FPS every 10 frames
	//drawText(2,2, std::to_string(fps), Vector3(1,1,1), 2 );

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{

	float speed = seconds_elapsed * 600; //the speed is defined by the seconds_elapsed so it goes constant

	if (cameraMode) {
		if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
		{
			freeCamera->rotate(mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			freeCamera->rotate(mouse_delta.y * 0.005f, freeCamera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		//async input to move the camera around
		if (keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) freeCamera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) freeCamera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) freeCamera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) freeCamera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

		//to navigate with the mouse fixed in the middle
		if (mouse_locked)
		{
			int center_x = (int)floor(window_width*0.5f);
			int center_y = (int)floor(window_height*0.5f);
			//center_x = center_y = 50;
			SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
																	 //SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen

			this->mouse_position.x = (float)center_x;
			this->mouse_position.y = (float)center_y;
		}
	}

	else {
		BulletManager::getInstance()->update(seconds_elapsed);
		CheckCollisions::getInstance()->checkAll();
		root->update(seconds_elapsed);
		controller->update(seconds_elapsed);
		motherIA->update(seconds_elapsed);
	}

	
	angle += (float)seconds_elapsed * 10.0f;
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
		case SDLK_c: {
			if (cameraMode) {
				cameraMode = false, currentCamera = gameCamera;
			}
			else cameraMode = true, currentCamera = freeCamera;
		}
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    
	/*
    Uint32 flags = SDL_GetWindowFlags(window);
    if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
    {
        width *= 2;
        height *= 2;
    }
	*/

	glViewport( 0,0, width, height );
	currentCamera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}