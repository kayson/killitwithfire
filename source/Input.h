#ifndef INPUT_H
#define INPUT_H

#include <GL/glfw.h>

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