
#include "GLObject.h"
#include <iostream>

void GLObject::Render() {
  CheckGLError();
}

void GLObject::CheckGLError() {
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		const GLubyte * errorString = gluErrorString(error);
		std::cerr << "OpenGL Error: " << errorString << " (in " << GetTypeName() << ")" << std::endl;
    error = glGetError();
	}
}
