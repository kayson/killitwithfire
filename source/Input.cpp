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
