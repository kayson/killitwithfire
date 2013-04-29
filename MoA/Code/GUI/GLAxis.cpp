
#include "GLAxis.h"

void GLAxis::Render() {
	glPushMatrix();

	glDisable(GL_LIGHTING);

	glPushName(0);
	glBegin(GL_LINES);
	if (IsHovering() && mAxisPicked[0])
		glColor4f(1.0f, 0.0f, 0.0f, 0.8f);
	else
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	RenderString(1.0, 0.0, 0.0, "X");
	glPopName();

	glPushName(1);
	glBegin(GL_LINES);
	if (IsHovering() && mAxisPicked[1])
		glColor4f(0.0f, 1.0f, 0.0f, 0.8f);
	else
		glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	RenderString(0.0, 1.0, 0.0, "Y");
	glPopName();

	glPushName(2);
	glBegin(GL_LINES);
	if (IsHovering() && mAxisPicked[2])
		glColor4f(0.0f, 0.0f, 1.0f, 0.8f);
	else
		glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
	RenderString(0.0, 0.0, 1.0, "Z");
	glPopName();

	glPopMatrix();

	GLObject::Render();
}

void GLAxis::PickChildren(GLuint * objects, int numberOfObjects)
{
	// We know the number of possible children is 1 (stack size = 1)
	mAxisPicked[0] = mAxisPicked[1] = mAxisPicked[2] = false;
	mAxisPicked[objects[0]] = true;
}

