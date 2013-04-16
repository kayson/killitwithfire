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
#ifndef __COLOR_MAP_FACTORY_H__
#define __COLOR_MAP_FACTORY_H__

#include "Util/ColorMap.h"
#include <list>
#include <map>
#include <string>

class ColorMapFactory
{
public:
  ColorMapFactory() {}

  // Factory method for creating color maps given a name identifier
  static ColorMap * New(const std::string & name);

  // Returns the names of all color maps registered with the factory
  static std::list<std::string> GetColorMaps();

  // Color maps should instantiate a FactoryRegistration member to register
  // with the factory
  class FactoryRegistration {
    public :
    FactoryRegistration(const std::string & name, ColorMap * colormap) {
      ColorMapFactory::GetMap()[name] = colormap;
    }
  };

protected:

  // Map of all color maps registered with the factory
  static std::map<std::string, ColorMap *> & GetMap() {
    static std::map<std::string, ColorMap *> mColorMaps;
    return mColorMaps;
  }
};

#endif
