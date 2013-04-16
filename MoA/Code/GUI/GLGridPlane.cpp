
#include "GLGridPlane.h"

void GLGridPlane::Render() {
	glPushMatrix();

	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
	GLfloat spanX = mDimensions[0]/2.0;
	GLfloat spanY = mDimensions[1]/2.0;
	for (GLfloat x = -spanX; x < spanX+mDensity*0.5; x += mDensity) {
		glVertex2f(x, -spanY);
		glVertex2f(x,  spanY);
	}
	for (GLfloat y = -spanY; y < spanY+mDensity*0.5; y += mDensity) {
		glVertex2f(-spanX, y);
		glVertex2f( spanX, y);
	}
	glEnd();

	glPopMatrix();

	GLObject::Render();
}

