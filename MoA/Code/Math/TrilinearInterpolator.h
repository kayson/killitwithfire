#ifndef __TRILINEAR_INTERPOLATOR_H__
#define __TRILINEAR_INTERPOLATOR_H__

#include "Math/Interpolator.h"

class TrilinearInterpolator : public Interpolator
{
public:
  TrilinearInterpolator();
  ~TrilinearInterpolator();

  virtual Vector3<float> Interpolate(float x, float y, float z, const Volume<Vector3<float> >& grid);
  virtual float Interpolate(float x, float y, float z, const Volume<float >& grid);

protected:
};

#endif
