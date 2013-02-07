#ifndef CAMERA_H
#define CAMERA_H

#ifdef __APPLE__
#include "glfw.h"
#include "armadillo.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> 
#include <armadillo>
#endif

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
