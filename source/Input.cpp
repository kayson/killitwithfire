#include "Input.h"
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

}

void GLFWCALL keyButtonListener( int key, int action )
{
	
}
