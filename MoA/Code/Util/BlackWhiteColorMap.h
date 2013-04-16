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
#ifndef __BLACKWHITE_COLOR_MAP_H__
#define __BLACKWHITE_COLOR_MAP_H__

#include "Util/ColorMapFactory.h"

class BlackWhiteColorMap : public ColorMap{
public :
  BlackWhiteColorMap();
protected :
  static ColorMapFactory::FactoryRegistration mFactoryRegistration;
};

#endif
