#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related
#include <stdlib.h>
#include <iostream>
#include "Grid.h"

bool init();
void showFPS();
void update(const double dt);
void render();
void GLFWCALL reshape( int width, int height );
void input();
void mouseInput();
void keyInput();
void GLFWCALL mouseButtonListener(int button, int action);
void GLFWCALL keyButtonListener( int key, int action );

double TIME = 1.0/7000.0;

double t0 = 0.0;
int frames = 0;
char titlestring[200];

int HEIGHT = 800, WIDTH = 800;
bool running = false;

Grid grid(10, 10);

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
    
	// Initialise GLFW
	glfwInit();

	// Open the OpenGL window
	if( !glfwOpenWindow(WIDTH, HEIGHT, 8,8,8,8, 32,0, GLFW_WINDOW) )
	{
		glfwTerminate(); // glfwOpenWindow failed, quit the program.
		return false;
	}

	//Fixar keylistener
	glfwSetKeyCallback( keyButtonListener );
	glfwSetWindowSizeCallback( reshape );
	glfwSetMouseButtonCallback( mouseButtonListener);
	
	glfwSwapInterval(0);//Tar bort Vsync


	glClearColor (0.0f, 0, 0.0f, 0.0f);
	glShadeModel (GL_SMOOTH);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Light material
	/*glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat mat_diffuse[] = { 1.0f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	GLfloat light_direction[] = { 0.3f, 0.3f, 1.0f, 0.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_direction);*/

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
		sprintf(titlestring, "Bollhav system (%.1f FPS)", fps);
		//double e = balls.calcEnergy();
		//sprintf(titlestring, "Bollhav system (%.1f Energi Diff)", e-ie);
		glfwSetWindowTitle(titlestring);
		t0 = t;
		frames = 0;
	}
	frames ++;
}

void update(const double dt)
{
	input(); //updatera mus och tangentbord

	//Update physics
}

//renderar objekt
void render(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	grid.draw();

	glfwSwapBuffers();
}

//Fixar vyn
void GLFWCALL reshape( int width, int height )
{
	HEIGHT = height;
	WIDTH = width;

    if(HEIGHT<=0) HEIGHT=1; // Safeguard against iconified/closed window

    //glViewport (0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);
	glViewport (0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
	gluOrtho2D(-10.0f, 10.0f, 0.0f, 20.0f); 
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void input()
{
	mouseInput();
	keyInput();
}

void mouseInput()
{
}

void keyInput()
{

}

void GLFWCALL mouseButtonListener(int button, int action)
{
	// Tar reda på vart i fönstret man klickar
	int x = 0, y = 0;
	glfwGetMousePos(&x, &y);

	GLint viewport[4]; //var to hold the viewport info
	GLdouble modelview[16]; //var to hold the modelview info
	GLdouble projection[16]; //var to hold the projection matrix info
	GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
	GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
	glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
	glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	winZ = 0;
		
	//get the world coordinates from the screen coordinates
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);

	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
	}
}

void GLFWCALL keyButtonListener( int key, int action )
{
	
}
