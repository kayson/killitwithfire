#include "Camera.h"

Camera::Camera()
{
	cameraSpeed_ = 0.6;
	(position_) = Vector3(0,0,0);
}

void Camera::translateForCamera()
{
	glLoadIdentity();
	glTranslated(-position_.x, -position_.y, -position_.z);
}

void Camera::moveCamera(Vector3 &dP)
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
    

    
    if (true) {
        
        glMatrixMode(GL_PROJECTION);
        gluOrtho2D(-2.0f, 20.0f, -2.0f, 20.0f);
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();

    }else{
        // Select and setup the projection matrix.
        glMatrixMode(GL_PROJECTION); // "We want to edit the projection matrix"
        //glLoadIdentity(); // Reset the matrix to identity
        // 65 degrees FOV, same aspect ratio as window, depth range 1 to 100
        //gluPerspective( 85.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f );
        
        // Select and setup the modelview matrix.
        glMatrixMode( GL_MODELVIEW ); // "We want to edit the modelview matrix"
        glLoadIdentity(); // Reset the matrix to identity
        

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Look from 0,-5,0 towards 0,0,0 with Z as "up" in the image
        /*
        gluLookAt( 7.5f, 0.0, 7.5f,    // Eye position
                      0.0, 0.0, 0.0,   // View point
                    0.0f, 1.0f, 0.0f );  // Up vector
         */
    }
    
    
}