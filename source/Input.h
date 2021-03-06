#ifndef INPUT_H
#define INPUT_H

#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#endif

#include "Camera.h"

class Input
{
public:
	Input();
	~Input();

	void setCamera(Camera *cam);
	void updateInput(double dt);
	void mouseInput();
	void keyInput(double dt);
	
	void initListeners();
	int xpos,ypos;
    static GLdouble worldX, worldY, worldZ ; //variables to hold world x,y,z coordinates


private:
	Camera *camera;
};

#endif