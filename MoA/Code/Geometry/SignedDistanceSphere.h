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
#ifndef __signed_distance_sphere_h__
#define __signed_distance_sphere_h__

#include "Geometry/Implicit.h"

/*!  \brief Sphere base class */
class SignedDistanceSphere : public Implicit {
public:
  SignedDistanceSphere(float r);
  virtual ~SignedDistanceSphere();
  virtual float getValue(float x, float y, float z) const;

protected:
  float radius;
};

#endif
