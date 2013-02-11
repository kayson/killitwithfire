#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related
#endif

#include <stdlib.h>
#include <iostream>

#include "Grid.h"
#include "levelset/LevelSet.h"
#include "Input.h"
#include "Camera.h"
#include "Vector3.h"

#include "presets/firePresetsTwoDimension.h"

#include "levelset/ImplicitFunctions.h"

#include "Discretization.h"
#include "CentralDiff.h"
#include "UpwindDiff.h"
#include "Divergence.h"
#include "Gradient.h"

#include "fire.h"

bool init();
void showFPS();
void update();
void render();

double TIME = 1.0/7000.0;

double t0 = 0.0;
int frames = 0;
char titlestring[200];

int HEIGHT = 600, WIDTH = 800;
bool running = false;

Fire *fire;
Input controller;
static Camera camera;

int main(int argc, char *argv[]) 
{
	if(!init())
	{
		return 1;
	}
	
	fire = new Fire(new FirePresetsTwoDimension());
	// Main loop
	while(running)
	{
		// Calculate and update the frames per second (FPS) display
		showFPS();

		update();

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
    
	controller.setCamera(&camera);
	// Initialise GLFW
	glfwInit();

	// Open the OpenGL window
	if( !glfwOpenWindow(WIDTH, HEIGHT, 8,8,8,8, 32,0, GLFW_WINDOW) )
	{
		glfwTerminate(); // glfwOpenWindow failed, quit the program.
		return false;
	}

	//Fixar keylistener
	controller.initListeners();
	glfwSetWindowSizeCallback( camera.reshape );
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

void update()
{
	controller.updateInput(); //updatera mus och tangentbord

	//Update physics
	fire->runSumulation();
}

//renderar objekt
void render(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	camera.translateForCamera();

	fire->draw();

	glfwSwapBuffers();
}

