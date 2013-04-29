#pragma once

#ifndef _GL_GRID_PLANE
#define _GL_GRID_PLANE

#include "GLObject.h"

class GLGridPlane : public GLObject
{
public :
	GLGridPlane(const std::string & name) : GLObject(name) {
		SetDimensions(1.0, 1.0);
		SetDensity(0.1);
	}
	virtual ~GLGridPlane() { }

  virtual const char * GetTypeName() { return typeid(GLGridPlane).name(); }

	virtual void Render();

	void SetDimensions(float x, float y) { mDimensions[0] = x; mDimensions[1] = y; }
	void SetDensity(float density) { mDensity = density; }

protected :

	float mDensity;
	float mDimensions[2];

};

#endif
