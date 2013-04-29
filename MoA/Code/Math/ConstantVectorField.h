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
#ifndef __constant_vector_field_h__
#define __constant_vector_field_h__

#include "Math/Vector3.h"
#include "Math/Function3D.h"
/*! Constant vector field that always returns a constant vector */
class ConstantVectorField : public Function3D<Vector3<float> >{
protected:
  Vector3<float> mV;
public:
  ConstantVectorField(const Vector3<float> v) : mV(v) {}
  Vector3<float> GetValue(float x, float y, float z) const {
    return mV;
  }
  //! Return a bound on the maximum value of the function
  Vector3<float> GetMaxValue() const {
    return mV;
  }
};
#endif
