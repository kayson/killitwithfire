#include "Input.h"
#include "helper.h"


#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related
#endif


GLdouble Input::worldX = 0;
GLdouble Input::worldY = 0;
GLdouble Input::worldZ = 0;


void GLFWCALL keyButtonListener( int key, int action );
void GLFWCALL mouseButtonListener(int button, int action);

static Camera *sCam;
Input::Input(void)
{
	glfwSetKeyCallback(keyButtonListener);
	glfwSetMouseButtonCallback(mouseButtonListener);
}


Input::~Input(void)
{
}

void Input::initListeners()
{
	glfwSetKeyCallback(keyButtonListener);
	glfwSetMouseButtonCallback(mouseButtonListener);
}

void moveCamera(DirectionEnums DIR, double dt)
{
	Vector3 dP(0,0,0);
	if(DIR == FORWARD)
	{
		dP.z = 1.0;
	}
	else if(DIR == BACKWARD)
	{
		dP.z = -1.0;
	}
	else if(DIR == LEFT)
	{
		dP.x = -1.0;
	}
	else if(DIR == RIGHT)
	{
		dP.x = 1.0;
	}
	else if(DIR == UP)
	{
		dP.y = 1.0;
	}
	else if(DIR == DOWN)
	{
		dP.y = -1.0;
	}
	else
	{
		std::cout << "Direction not implemented in moveCamera @Input.cpp!";
	}
	if(dt != 0.0)
		dP *= sCam->getCameraSpeed()/(dt * 1.0/60.0);
	else
		dP *= sCam->getCameraSpeed();
	sCam->moveCamera(dP);

}

void Input::setCamera(Camera *cam)
{
	camera = (cam);
	sCam = cam;
}

void Input::updateInput(double dt)
{
	mouseInput();
	keyInput(dt);
}

void Input::mouseInput()
{
    

    if(glfwGetMouseButton(0)){

        glfwGetMousePos(&xpos, &ypos);

        //std::cout << xpos << " " << ypos << std::endl;
        
        
        
    }
    
    
}

int CheckButton(int c)
{
	return glfwGetKey(c);
}

void Input::keyInput(double dt)
{

	if(CheckButton('W') == GLFW_PRESS)
		moveCamera(UP, dt);
	if(CheckButton('A') == GLFW_PRESS)
		moveCamera(LEFT, dt);
	if(CheckButton('S') == GLFW_PRESS)
		moveCamera(DOWN, dt);
	if(CheckButton('D') == GLFW_PRESS)
		moveCamera(RIGHT, dt);
}

void GLFWCALL mouseButtonListener(int button, int action)
{
    

    int x, y;
    glfwGetMousePos(&x, &y);
    
    if ((GLFW_MOUSE_BUTTON_1 == button)) {
        
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

    
    }
    
    
}

void GLFWCALL keyButtonListener( int key, int action )
{
	
}
