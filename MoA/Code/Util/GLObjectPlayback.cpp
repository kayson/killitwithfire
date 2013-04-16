
#include <Util/GLObjectPlayback.h>

void GLObjectPlayback::Reset()
{
	mStopwatch.start();
	mCurrentFrame = 0;
	mFrameCapture = false;
}

SimpleMesh& GLObjectPlayback::NextFrame()
{
 int frame;

 if(mFrameCapture) {
	 frame = mCurrentFrame;
	 mFrameCapture = ( (++mCurrentFrame) < mFrames.size() ); //do framecapture until currentframe reach last frame in vector
	}
 else {
	frame = (int)(mStopwatch.read() * mFPS) % mFrames.size();
	}

	assert( frame >= 0 && frame < mFrames.size() );
	return mFrames.at( frame );
}

void GLObjectPlayback::Render()
{
	SimpleMesh& obj = NextFrame();
	obj.Render();
}
