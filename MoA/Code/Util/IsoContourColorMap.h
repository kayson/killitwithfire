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
#ifndef __ISO_CONTOUR_COLOR_MAP_H__
#define __ISO_CONTOUR_COLOR_MAP_H__

#include "Util/ColorMapFactory.h"

class IsoContourColorMap : public ColorMap
{
public:
  IsoContourColorMap();

  virtual Vector3<float> Map(float val, float low, float high) const;

protected :
  static ColorMapFactory::FactoryRegistration mFactoryRegistration;
};

#endif
