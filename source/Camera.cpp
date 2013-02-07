#include "Camera.h"

Camera::Camera()
{
	cameraSpeed_ = 0.3;
	(position_) = zeros<vec>(3,1);
}

void Camera::translateForCamera()
{
	glLoadIdentity();
	glTranslatef(-position_[0], -position_[1], -position_[2]);
}

void Camera::moveCamera(vec dP)
{
	position_ += dP;
}

void GLFWCALL Camera::reshape( int width, int height )
{
	//Bör HEIGHT och WIDTH bör vara globala variabler för fönsterstorleken
	int HEIGHT = height;
	int WIDTH = width;

    if(HEIGHT<=0) HEIGHT=1; // Safeguard against iconified/closed window

	glViewport (0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
	//double offsetX = position_[0];
	gluOrtho2D(-2.0f, 20.0f, -2.0f, 20.0f); 
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}