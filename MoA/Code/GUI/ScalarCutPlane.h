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
#ifndef __scalar_cutplane_h__
#define __scalar_cutplane_h__

#include "Geometry/Geometry.h"
#include "Math/Function3D.h"
#include "Util/ColorMap.h"

class ScalarCutPlane : public Geometry
{
protected :

  float mDx;
  const Function3D<float> * mFunction;

  GLuint mTextureID;
  GLuint mWidth, mHeight;

public :

  ScalarCutPlane(const std::string & name, float dx,
                   const Function3D<float> * function);

  virtual const char * GetTypeName() { return typeid(ScalarCutPlane).name(); }

  virtual void Render();

  virtual void Initialize() { }

  virtual void Update();

  virtual void SetTransform(const Matrix4x4<float> & transform);

  virtual void SetColorMap(ColorMap * colormap) { GLObject::SetColorMap(colormap); Update(); }

};

#endif
