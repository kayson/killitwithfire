
#include "ColorMapFactory.h"


ColorMap * ColorMapFactory::New(const std::string & name)
{
  return GetMap()[name];
}

std::list<std::string> ColorMapFactory::GetColorMaps()
{
  std::list<std::string> maps;
  std::map<std::string, ColorMap *>::iterator iter = GetMap().begin();
  std::map<std::string, ColorMap *>::iterator iend = GetMap().end();
  while (iter != iend) {
    maps.push_back((*iter).first);
    iter++;
  }

  return maps;
}
