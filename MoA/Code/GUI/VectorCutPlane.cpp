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

#include "VectorCutPlane.h"

VectorCutPlane::VectorCutPlane(const std::string & name, float dx,
                               const Function3D<Vector3<float> > * function)
  : mDx(dx), mFunction(function)
{
  SetName(name);
  Update();
}


void VectorCutPlane::Render()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  glLineWidth(1);
  glBegin(GL_LINES);
  unsigned int i = 0;
  float x = 0;
  for (float y = -1; y <= 1; y += mDx) {
    for (float z = -1; z <= 1; z += mDx) {
      Vector4<float> vec(x,y,z,1);
      vec = mTransform*vec;

      Vector3<float> color = mColorMap->Map(mVectors[i], -1, 1);
      glColor3fv(color.GetArrayPtr());

	  Vector3<float> end = Vector3<float>(vec[0], vec[1], vec[2]) + mVectors[i]*mDx;
      glVertex3fv(vec.GetArrayPtr());
      glVertex3fv(end.GetArrayPtr());
      i++;
    }
  }
  glEnd();
  glLineWidth(1);

  glPointSize(3);
  glColor3f(1.0, 0, 0);
  glBegin(GL_POINTS);
  for (float y = -1; y <= 1; y += mDx) {
    for (float z = -1; z <= 1; z += mDx) {
      Vector4<float> vec(x,y,z,1);
      vec = mTransform*vec;
      glVertex3fv(vec.GetArrayPtr());
    }
  }
  glEnd();
  glPointSize(1);

  glPopAttrib();

  GLObject::Render();
}


void VectorCutPlane::SetTransform(const Matrix4x4<float> & transform)
{
  Geometry::SetTransform(transform);
  Update();
}


void VectorCutPlane::Update()
{
  std::cerr << "Building vector cut plane of size " << 2/mDx << "x" << 2/mDx << std::endl;
  mVectors.clear();
  float x = 0;
  for (float y = -1; y <= 1; y += mDx) {
    for (float z = -1; z <= 1; z += mDx) {
      Vector4<float> vec(x,y,z,1);
      vec = mTransform*vec;

      mVectors.push_back( mFunction->GetValue(vec[0], vec[1], vec[2]) );
    }
  }
}
