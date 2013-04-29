/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Sderstrm (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/

#include "ScalarCutPlane.h"
#ifndef __WXMAC__
#include <GL/glext.h>
#endif
#include <cassert>
#include <limits>

ScalarCutPlane::ScalarCutPlane(const std::string & name, float dx,
                                   const Function3D<float> * function)
  : mDx(dx), mFunction(function), mTextureID(0)
{
  SetName(name);
  Update();
}


void ScalarCutPlane::Render()
{
  if (glIsTexture(mTextureID) == GL_FALSE)  return;

  // Apply transform
  glPushMatrix();
  glMultMatrixf(mTransform.ToGLMatrix().GetArrayPtr());

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_TEXTURE_RECTANGLE_ARB);
  glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mTextureID);

  glColor4f(1, 1, 1, mOpacity);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0);
  glVertex3f(0.0f, -1.0, -1.0);

  glTexCoord2i(mWidth-1, 0);
  glVertex3f(0.0f, 1.0, -1.0);

  glTexCoord2i(mWidth-1, mHeight-1);
  glVertex3f(0.0f, 1.0, 1.0);

  glTexCoord2i(0, mHeight-1);
  glVertex3f(0.0f, -1.0, 1.0);
  glEnd();

  glDisable(GL_TEXTURE_RECTANGLE_ARB);
  glDisable(GL_BLEND);
  glPopAttrib();
  glPopMatrix();

  GLObject::Render();
}

void ScalarCutPlane::SetTransform(const Matrix4x4<float> & transform)
{
  Geometry::SetTransform(transform);
  Update();
}


void ScalarCutPlane::Update()
{
  mWidth = (GLuint)Round(2.0/mDx) + 1;
  mHeight = (GLuint)Round(2.0/mDx) + 1;
  GLfloat * image = new GLfloat[mWidth*mHeight*3];

  std::cerr << "Building scalar cut plane of size " << mWidth << "x" << mHeight << std::endl;

  // Find out min/max value range
  float x = 0;
  float y,z;
  std::vector<float> values;
  values.reserve(mWidth*mHeight);
  float minVal = std::numeric_limits<float>::max();
  float maxVal = -std::numeric_limits<float>::max();
  for (y = -1.0; y < 1.0+0.5*mDx; y += mDx) {
    for (z = -1.0; z < 1.0+0.5*mDx; z += mDx) {
      Vector4<float> vec(x,y,z,1);
      vec = mTransform*vec;

      float val = mFunction->GetValue(vec[0], vec[1], vec[2]);
      values.push_back(val);

      if (minVal > val)  minVal = val;
      if (maxVal < val)  maxVal = val;
    }
  }

  // Map the color values
  std::cerr << "Color mapping with range [" << minVal << "," << maxVal << "]" << std::endl;
  x = 0;
  unsigned int j = 0, k = 0, ind = 0;
  for (y = -1.0; y < 1.0+0.5*mDx; y += mDx, j++) {
    for (z = -1.0; z < 1.0+0.5*mDx; z += mDx, k++) {
      Vector3<float> c = mColorMap->Map(values.at(ind), minVal, maxVal);
      ind++;

      assert(j < mWidth && k < mHeight);
      image[3*(j + mWidth*k)    ] = c[0];
      image[3*(j + mWidth*k) + 1] = c[1];
      image[3*(j + mWidth*k) + 2] = c[2];
    }
    k = 0;
  }

  if (glIsTexture(mTextureID) == GL_FALSE)
    glGenTextures(1, &mTextureID);

  glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mTextureID);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 3, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, image);

  CheckGLError();

  delete[] image;
}
