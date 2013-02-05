#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related
#endif

#include <stdlib.h>
#include <iostream>
#include "levelset/LevelSet.h"
#include "Grid.h"
#include "Input.h"
#include "Camera.h"

#include "Helper.h"
#include "armadillo"
#include "presets/firePresetsTwoDimension.h"

#include "levelset/ImplicitFunctions.h"

using namespace arma;

bool init();
void showFPS();
void update(const double dt);
void render();

double TIME = 1.0/7000.0;

double t0 = 0.0;
int frames = 0;
char titlestring[200];

int HEIGHT = 600, WIDTH = 800;
bool running = false;

LevelSet levelSet;
Input controller;
Camera camera;

int main(int argc, char *argv[]) 
{
	if(!init())
	{
		return 1;
	}

	// Main loop
	while(running)
	{
		// Calculate and update the frames per second (FPS) display
		showFPS();

		update(TIME);

		// Draw the scene.
		render();

		// Check if the ESC key was pressed or the window was closed.
		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			running = GL_FALSE;
	}

	// Close the OpenGL window and terminate GLFW.
	glfwTerminate();

	return 0;
}

bool init() 
{
	running = true; // Main loop exits when this is set to GL_FALSE
    
	levelSet.fillLevelSet(implicitFunction::sphere);
	// Initialise GLFW
	glfwInit();

	// Open the OpenGL window
	if( !glfwOpenWindow(WIDTH, HEIGHT, 8,8,8,8, 32,0, GLFW_WINDOW) )
	{
		glfwTerminate(); // glfwOpenWindow failed, quit the program.
		return false;
	}

	//Fixar keylistener
	glfwSetKeyCallback( controller.keyButtonListener );
	glfwSetWindowSizeCallback( camera.reshape );
	glfwSetMouseButtonCallback( controller.mouseButtonListener);
	
	glfwSwapInterval(0);//Tar bort Vsync

	glClearColor (0.0f, 0, 0.0f, 0.0f);
	glShadeModel (GL_SMOOTH);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void showFPS() 
{
	double t, fps;
    
	// Get current time
	t = glfwGetTime();  // Gets number of seconds since glfwInit()
	// If one second has passed, or if this is the very first frame
	if( (t-t0) > 1.0 || frames == 0 )
	{
		fps = (double)frames / (t-t0);
		sprintf(titlestring, "Fire (%.1f FPS)", fps);

		glfwSetWindowTitle(titlestring);
		t0 = t;
		frames = 0;
	}
	frames ++;
}

void update(const double dt)
{
	controller.updateInput(); //updatera mus och tangentbord

	//Update physics
}

//renderar objekt
void render(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	camera.translateForCamera();

	levelSet.draw();
	glfwSwapBuffers();
}

