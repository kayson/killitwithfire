/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Söderström (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#ifndef __sphere_h__
#define __sphere_h__

#include "Geometry/Implicit.h"

/*!  \brief Sphere base class */
class Sphere : public virtual Implicit {
public:
  Sphere(float r, bool euclideanDistance = false);
  virtual ~Sphere();
  virtual float GetValue(float x, float y, float z) const;

protected:
  float radius2;
  bool mEuclideanDistance;
};

#endif
