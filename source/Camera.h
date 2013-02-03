#ifndef CAMERA_H
#define CAMERA_H

#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> 
#endif

class Camera
{
public:
	Camera();
	~Camera() {}

	static void GLFWCALL reshape( int width, int height );
};

#endif
