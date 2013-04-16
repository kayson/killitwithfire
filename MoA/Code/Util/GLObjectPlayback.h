#pragma once

#ifndef _GLOBJECT_PLAYBACK_H
#define _GLOBJECT_PLAYBACK_H

/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Sàderstràm (andreas.soderstrom@itn.liu.se)
 *   Joao Pedro Jorge (jpjorge@gmail.com)
 *
 *************************************************************************************************/

#include <Geometry/SimpleMesh.h>
#include "Util/Stopwatch.h"

/*! \brief A simple playback function working as a flipbook for Simple Meshes.
 */
class GLObjectPlayback: public GLObject
{
public:
	GLObjectPlayback(const std::string & name, int fps = 24) : mFPS(fps), GLObject(name)
	{ Reset(); }

	virtual const char * GetTypeName() { return typeid(GLObjectPlayback).name(); };

	virtual void Render();

	void Reset();

	void AddFrame(const SimpleMesh& obj) { mFrames.push_back( obj ); }

	bool HasReachedEnd() {return !mFrameCapture;}
	void SetFrameCapture(bool f) {mFrameCapture = f;}

protected:

	SimpleMesh& NextFrame();

private:

	int											mFPS;
	Stopwatch               mStopwatch;
	std::vector<SimpleMesh>	mFrames;
	bool mFrameCapture;
	unsigned int mCurrentFrame;
};

#endif
