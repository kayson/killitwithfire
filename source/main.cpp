#ifdef __APPLE__
#include "glfw.h"
#include <GLUT/GLUT.h>
#elif defined _WIN32 || defined _WIN64
#include <time.h>
#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related
#endif

#include <stdlib.h>
#include <iostream>
#include <string>

#include "Input.h"
#include "Camera.h"
#include "presets/firePresetsTwoDimension.h"

#include "fire.h"
#include "fire3D.h"
#include "Water/Water2D.h"
#include "Water/Water3D.h"

#include "imageExporter.h"
#include "integrateEuler.h"

#include <omp.h>

#define FIRE 0
#define WATER2D 1
#define WATER3D 2


#define SIMULAITON FIRE
//#define SIMULATION WATER2D
//#define SIMULATION WATER3D

bool init();
void showFPS();
void update();
void render();

double TIME = 1.0/7000.0;
double fps;

double t0 = 0.0;

int frames = 0;
char titlestring[200];

int HEIGHT = 1080, WIDTH = 540;
bool running = false;

Fire3D *fire;
Water2D *water;
Water3D *water3d;

Input controller;
static Camera camera;

int main(int argc, char *argv[]) 
{
	if(!init())
	{
		return 1;
	}

	int frames = 0;
    unsigned int t = (unsigned int)time(NULL);
    srand(t);
#if SIMULATION == FIRE
	fire = new Fire3D(new FirePresetsTwoDimension());
#elif SIMULATION == WATER2D
    water = new Water2D();
#elif SIMULATION == WATER3D
	water3d = new Water3D();
#endif

	omp_set_nested(0);
	// Main loop
	while(running)
	{
		// Calculate and update the frames per second (FPS) display
		showFPS();
        glClear (GL_COLOR_BUFFER_BIT);


		// Draw the scene.
        update();

		render();

        glfwSwapBuffers();

        //ImageExporter::saveSequence(frames, WIDTH, HEIGHT);

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
    
	//Set up keylisteners
	controller.initListeners();
	glfwSetWindowSizeCallback( camera.reshape );
	glfwSwapInterval(0);//remove v-sync

	glClearColor (0.0f, 0, 0.0f, 0.0f);
	glShadeModel (GL_SMOOTH);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
    
	return true;
}

void calculateFPS()
{
	double t;
		
    
	// Get current time
	t = glfwGetTime();  // Gets number of seconds since glfwInit()
	// If one second has passed, or if this is the very first frame
	if( (t-t0) > 1.0 || frames == 0 )
	{
		fps = (double)frames / (t-t0);

		t0 = t;
		frames = 0;
	}
	frames ++;

}

void showFPS() 
{
	// draw seconds per frame
	double t = glfwGetTime();
	sprintf(titlestring, "Fire (%.1f s)", (t-t0));
	t0 = t;


	glfwSetWindowTitle(titlestring);
}

void update()
{
	controller.updateInput(fps); //update mouse and keyboard input

	//Update physics
#if SIMULATION == FIRE
	fire->runSimulation();
#elif SIMULATION == WATER2D
    water->runSimulation(0.03);
#elif SIMULATION == WATER3D
	water3d->runSimulation(0.03); 
#endif

}

void render(void){
	glRotatef(100.0f,1.0,0,0);
	camera.translateForCamera();
	
	/* Code for get mouse coordinate in world coordiantes?*/
    int x, y;
    glfwGetMousePos(&x, &y);
    GLint viewport[4]; //var to hold the viewport info
    GLdouble modelview[16]; //var to hold the modelview info
    GLdouble projection[16]; //var to hold the projection matrix info
    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info
    
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    winZ = 0;
    
    //get the world coordinates from the screen coordinates
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &Input::worldX, &Input::worldY, &Input::worldZ);
    

#if SIMULATION == FIRE
	fire->draw();
#elif SIMULATION == WATER2D
    water->draw();
#elif SIMULATION == WATER3D
	water3d->draw();
#endif
  
    
}

