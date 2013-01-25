#include "Input.h"


Input::Input(void)
{
}


Input::~Input(void)
{
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

void GLFWCALL Input::mouseButtonListener(int button, int action)
{

}

void GLFWCALL Input::keyButtonListener( int key, int action )
{
	
}
