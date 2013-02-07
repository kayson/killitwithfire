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

void moveCamera(DirectionEnums DIR)
{
	vec dP = zeros(3,1);
	if(DIR == FORWARD)
	{
		dP[2] = 1.0;
	}
	else if(DIR == BACKWARD)
	{
		dP[2] = -1.0;
	}
	else if(DIR == LEFT)
	{
		dP[0] = -1.0;
	}
	else if(DIR == RIGHT)
	{
		dP[0] = 1.0;
	}
	else if(DIR == UP)
	{
		dP[1] = 1.0;
	}
	else if(DIR == DOWN)
	{
		dP[1] = -1.0;
	}
	else
	{
		cout << "Direction not implemented in moveCamera @Input.cpp!";
	}
	dP *= sCam->getCameraSpeed();
	sCam->moveCamera(dP);

}

void Input::setCamera(Camera *cam)
{
	camera = (cam);
	sCam = cam;
}

void Input::updateInput()
{
	mouseInput();
	keyInput();
}

void Input::mouseInput()
{
}

void Input::keyInput()
{

}

void GLFWCALL mouseButtonListener(int button, int action)
{

}

void GLFWCALL keyButtonListener( int key, int action )
{
	if(action == GLFW_PRESS)
	{
		switch(key)
		{
		case 'W':
			moveCamera(UP);
			break;
		case 'A':
			moveCamera(LEFT);
			break;

		case 'S':
			moveCamera(DOWN);
			break;

		case 'D':
			moveCamera(RIGHT);
			break;
		};
	}
}
