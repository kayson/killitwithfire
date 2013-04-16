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
#ifndef __implicit_gradient_field_h__
#define __implicit_gradient_field_h__

#include "Math/Function3D.h"
#include "Math/Vector3.h"
#include "Geometry/Implicit.h"

class ImplicitGradientField : public Function3D<Vector3<float> >
{
protected :
  const Implicit * mImplicit;

public:
  ImplicitGradientField(const Implicit * implicit) : mImplicit(implicit) { };
  virtual ~ImplicitGradientField() {}

  //! Evaluate the function at x,y,z
  virtual Vector3<float> GetValue(float x, float y, float z) const
  {
    return mImplicit->GetGradient(x,y,z);
  }

  //! Return a bound on the maximum value of the function
  virtual Vector3<float> GetMaxValue() const
  {
    return Vector3<float>(1,1,1);
  }
};

#endif
