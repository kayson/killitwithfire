
#ifndef _SUBDIVISION__H_
#define _SUBDIVISION__H_

#include "Geometry/Geometry.h"
#include <typeinfo>

class Subdivision
{
public :

  Subdivision() { }
  virtual ~Subdivision() { }

  virtual void Subdivide() = 0;
};

#endif
