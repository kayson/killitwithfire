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
#include "SignedDistanceSphere.h"

SignedDistanceSphere::SignedDistanceSphere(float r)
{
  this->radius = r;
  this->mBox = Bbox(Vector3<float>(-r, -r, -r), Vector3<float>(r, r, r));
}

SignedDistanceSphere::~SignedDistanceSphere() { }

float SignedDistanceSphere::getValue(float x, float y, float z) const
{
  TransformW2O(x,y,z);
  return std::sqrt(x*x + y*y + z*z) - radius;
}

