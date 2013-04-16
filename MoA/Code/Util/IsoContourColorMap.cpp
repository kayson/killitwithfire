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

#include "Util/IsoContourColorMap.h"

ColorMapFactory::FactoryRegistration IsoContourColorMap::mFactoryRegistration("Iso contour", new IsoContourColorMap());

IsoContourColorMap::IsoContourColorMap()
{
  mColors.push_back(Vector3<float>(0.0, 1.0, 0.0));
  mColors.push_back(Vector3<float>(1.0, 0.0, 0.0));
}

Vector3<float> IsoContourColorMap::Map(float val, float low, float high) const
{
  // Take absolute value
  if (val < 0) val = -val;

  // Compute fraction to do wrap-around
  float fraction = val*10 - (unsigned int)(val*10);

  // Do color mapping
  return ColorMap::Map(fraction, 0, 1);
}
