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
#ifndef __VortexVectorField_h__
#define __VortexVectorField_h__

#include <cmath>
#include "Math/Function3D.h"
#include "Math/Vector3.h"

class VortexVectorField : public Function3D<Vector3<float> >
{
public:
  VortexVectorField()
  {
    rScale = 0.1;
    thetaScale = 0.3;
    phiScale = 0.3;
  }
  ~VortexVectorField(void) {}

  Vector3<float> GetMaxValue() const { return Vector3<float>(1.f,1.f,1.f); }

  Vector3<float> GetValue(float xf, float yf, float zf) const
  {
    Vector3<float> v;
    // 3D VORTEX FIELD
    float r = sqrt(xf*xf + yf*yf + zf*zf);

    if (r != 0)
    {
      float theta = atan2(yf,xf);
      float phi = acos(zf / r);

      v[0] = rScale * cos(theta)*sin(phi) +
      thetaScale * (-sin(theta)) +
      phiScale * cos(theta)*cos(phi);

      v[1] = rScale * sin(theta)*sin(phi) +
      thetaScale * (cos(theta)) +
      phiScale * sin(theta)*cos(phi);

      v[2] = rScale * cos(phi) +
      thetaScale * (0) +
      phiScale * (-sin(phi));
    }
    else
    {
      v[0] = 0;
      v[1] = 0;
      v[2] = 0;
    }
    return v;
  }
protected:
  float t;
  float rScale, thetaScale, phiScale;
};

#endif
