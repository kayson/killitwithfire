#ifndef _IMAGE
#define _IMAGE

#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#include "GLUT/glut.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glext.h>
#endif
#include <string>
#include <cmath>

template <typename DataType>
class Image
{
public :

  Image();
  ~Image();
  Image(GLuint width, GLuint height, GLuint components);
  Image(const std::string & filename);
  Image(const Image & img);

  void SetDimensions(GLuint width, GLuint height, GLuint components);

  void CaptureScreen();
  void ReadFromFile(const std::string & filename);
  void SaveToFile(const std::string & filename);
  GLuint LoadTexture();

  void SetName(const std::string & name) { mName = name; }
  const std::string & GetName() { return mName; }

  DataType Max(int component);
  DataType Min(int component);

  inline GLuint Width() { return mWidth; }
  inline GLuint Height() { return mHeight; }
  inline GLuint Components() { return mComponents; }
  inline GLuint TextureID() { return mTexID; }

  inline DataType & operator() (int x, int y, int c)
  {
    // Clamp to edge
    if (x < 0)                              x = 0;
    else if (x > mWidth-1)  x = mWidth-1;
    if (y < 0)                              y = 0;
    else if (y > mHeight-1) y = mHeight-1;

    return (mData + mComponents*(x + mWidth*y))[c];
  }

  inline DataType & operator() (unsigned int x, unsigned int y, int c)
  {
    return (mData + mComponents*(x + mWidth*y))[c];
  }

  inline DataType operator() (double x, double y, int c)
  {
    // Perform bi-linear interpolation
    // Assume (x,y) being normalized

    // Map to image coordinates
    double i = x*mWidth;
    double j = y*mHeight;

    /*
     *
     *      p4 -------- p3
     *  ^    |          |
     *  |    |  p0      |
     *  |    |          |
     *  b   p1 -------- p2
     *
     *       a --->
     */
    double a = i - floor(i);
    double b = j - floor(j);

    DataType p1 = (*this)((int)floor(i), (int)floor(j), c);
    DataType p2 = (*this)((int)ceil(i), (int)floor(j), c);
    DataType p3 = (*this)((int)ceil(i), (int)ceil(j), c);
    DataType p4 = (*this)((int)floor(i), (int)ceil(j), c);

    return (1.0-a)*(1.0-b)*p1 + a*(1.0-b)*p2 + b*(1.0-a)*p4 + a*b*p3;
  }

  Image & operator = (const Image & img);

private :

  // Color values range between 0.0 and 1.0
  DataType * mData;
  GLuint mWidth, mHeight, mComponents;
  GLuint mTexID;
  std::string mName;

};

#include "Image_Impl.h"

#endif

