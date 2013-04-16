/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Sˆderstrˆm (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#ifndef __implicitmesh_h__
#define __implicitmesh_h__

#include "Implicit.h"
#include "SimpleMesh.h"

#include <set>
#include <map>
#include <cassert>
#include <limits>
#include <algorithm>
#include <cmath>
#include <utility>

#include "Math/Vector3.h"
#include "Util/JetColorMap.h"
#include "Math/Volume.h"

class ImplicitMesh : public Implicit {

public:
  ImplicitMesh(SimpleMesh * mesh);
  virtual ~ImplicitMesh();

  virtual void Initialize();

  virtual float GetValue(float x, float y, float z) const {
    // Transform (x,y,z) to grid coordinates
    TransformW2O(x,y,z);
    x = (x - mBox.pMin[0]) / mMeshSampling;
    y = (y - mBox.pMin[1]) / mMeshSampling;
    z = (z - mBox.pMin[2]) / mMeshSampling;
    return mData->GetValue(x,y,z);
  }

  virtual void SetMeshSampling(float sampling) { Implicit::SetMeshSampling(sampling); Initialize(); }

protected:

  SimpleMesh * mSourceMesh;
  Volume<float> * mData;

  //! Computes the closest distance from a point in space to the mesh
  float DistanceToPoint(float x, float y, float z, const SimpleMesh & mesh) const;

  // Computes the closest distance between a triangle and a point in space.
  static std::pair<float, bool> DistanceSquared(const Vector3<float> & p, const Vector3<float> &v1,
                                                const Vector3<float> &v2, const Vector3<float> &v3);
};


#endif
