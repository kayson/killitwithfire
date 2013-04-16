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
#include "Sphere.h"

Sphere::Sphere(float r, bool euclideanDistance) : mEuclideanDistance(euclideanDistance)
{
  this->radius2 = r*r;
  this->mBox = Bbox(Vector3<float>(-r, -r, -r), Vector3<float>(r, r, r));
}

Sphere::~Sphere() { }

float Sphere::GetValue(float x, float y, float z) const
{
  TransformW2O(x,y,z);

  if (mEuclideanDistance)
    return sqrt(x*x + y*y + z*z) - sqrt(radius2);
  else
    return (x*x + y*y + z*z - radius2);
}

