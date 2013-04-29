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
#ifndef __vector_cutplane_h__
#define __vector_cutplane_h__

#include "Geometry/Geometry.h"
#include "Math/Function3D.h"
#include "Util/ColorMap.h"
#include "Math/Vector3.h"
#include <vector>

class VectorCutPlane : public Geometry
{
protected :

  float mDx;
  const Function3D<Vector3<float> > * mFunction;

  std::vector<Vector3<float> > mVectors;

public :

  VectorCutPlane(const std::string & name, float dx,
                   const Function3D<Vector3<float> > * function);

  virtual const char * GetTypeName() { return typeid(VectorCutPlane).name(); }

  virtual void Render();

  virtual void Initialize() { }

  virtual void Update();

  virtual void SetTransform(const Matrix4x4<float> & transform);
};

#endif
