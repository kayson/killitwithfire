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
#ifndef __COLOR_MAP_H__
#define __COLOR_MAP_H__

#include "Math/Vector3.h"
#include "Util/Util.h"
#include <vector>

class ColorMap{
public:
  ColorMap() {}
  virtual ~ColorMap() {}

  virtual Vector3<float> Map(float val, float low, float high) const;
  virtual Vector3<float> Map(const Vector3<float> & val, float low, float high) const;

protected:
  // A vector containing the colors to be interpolated
  std::vector<Vector3<float> > mColors;
};

#endif
