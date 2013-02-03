#ifndef INPUT_H
#define INPUT_H

#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#endif

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
};

#endif