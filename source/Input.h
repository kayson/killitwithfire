#ifndef INPUT_H
#define INPUT_H

#ifdef __APPLE__
#include "glfw.h"
#include "armadillo.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#include<armadillo>

#endif
using namespace arma;

class Input
{
public:
	Input();
	~Input();

	void updateInput();
	void mouseInput();
	void keyInput();
	static void GLFWCALL mouseButtonListener(int button, int action);
	static void GLFWCALL keyButtonListener( int key, int action );
public:
	void (*moveFunction)(vec dP);
};

#endif