#ifndef CAMERA_H
#define CAMERA_H

#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> 
#endif
#include <armadillo>

using namespace arma;

class Camera
{
public:
	Camera();
	~Camera() {};

	static void GLFWCALL reshape( int width, int height );
	void translateForCamera();
	double getCameraSpeed(){ return cameraSpeed_; };
	void moveCamera(vec dP);
	
private:
	vec position_;
	double cameraSpeed_;
};

#endif
