#include "TrilinearInterpolator.h"

TrilinearInterpolator::TrilinearInterpolator()
{
}

TrilinearInterpolator::~TrilinearInterpolator()
{
}

Vector3<float> TrilinearInterpolator::Interpolate(float x, float y, float z, const Volume<Vector3<float> >& grid)
{
  int i = (unsigned int)x;
  int j = (unsigned int)y;
  int k = (unsigned int)z;

  float bx = x - i;
  float by = y - j;
  float bz = z - k;

  return (grid.GetValue(i,   j,   k  ) * (1-bx) * (1-by) * (1-bz) +
          grid.GetValue(i+1, j,   k  ) *  bx    * (1-by) * (1-bz) +
          grid.GetValue(i+1, j+1, k  ) *  bx    *  by    * (1-bz) +
          grid.GetValue(i,   j+1, k  ) * (1-bx) *  by    * (1-bz) +
          grid.GetValue(i,   j,   k+1) * (1-bx) * (1-by) *  bz    +
          grid.GetValue(i+1, j,   k+1) *  bx    * (1-by) *  bz    +
          grid.GetValue(i+1, j+1, k+1) *  bx    *  by    *  bz    +
          grid.GetValue(i,   j+1, k+1) * (1-bx) *  by    *  bz);
}

float TrilinearInterpolator::Interpolate(float x, float y, float z, const Volume<float >& grid)
{
  int i = (unsigned int)x;
  int j = (unsigned int)y;
  int k = (unsigned int)z;

  float bx = x - i;
  float by = y - j;
  float bz = z - k;

  return (grid.GetValue(i,   j,   k  ) * (1-bx) * (1-by) * (1-bz) +
          grid.GetValue(i+1, j,   k  ) *  bx    * (1-by) * (1-bz) +
          grid.GetValue(i+1, j+1, k  ) *  bx    *  by    * (1-bz) +
          grid.GetValue(i,   j+1, k  ) * (1-bx) *  by    * (1-bz) +
          grid.GetValue(i,   j,   k+1) * (1-bx) * (1-by) *  bz    +
          grid.GetValue(i+1, j,   k+1) *  bx    * (1-by) *  bz    +
          grid.GetValue(i+1, j+1, k+1) *  bx    *  by    *  bz    +
          grid.GetValue(i,   j+1, k+1) * (1-bx) *  by    *  bz);
}
