#pragma once

#ifndef _GL_AXIS
#define _GL_AXIS

#include "GLObject.h"

class GLAxis : public GLObject
{
public :
	GLAxis(const std::string & name) : GLObject(name) {
		mAxisPicked[0] = mAxisPicked[1] = mAxisPicked[2] = false;
	}
	virtual ~GLAxis() { }

  virtual const char * GetTypeName() { return typeid(GLAxis).name(); }

	virtual void Render();

	virtual void PickChildren(GLuint * objects, int numberOfObjects);

protected :

	bool mAxisPicked[3];

};

#endif
