#include "Camera.h"

Camera::Camera()
{

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
	gluOrtho2D(-10.0f, 10.0f, 0.0f, 20.0f); 
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}