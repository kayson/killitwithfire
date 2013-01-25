#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glfw.h>

class Camera
{
public:
	Camera();
	~Camera() {}

	static void GLFWCALL reshape( int width, int height );
};

#endif
