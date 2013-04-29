#ifndef __CUBE_H__
#define __CUBE_H__

#include <vector>
#include "Geometry/Quadric.h"

class Cube : public Implicit {
  public:
    Cube();
    ~Cube();

    virtual float GetValue(float x, float y, float z) const;

  private:
    std::vector<Quadric*> mPlanes;
    bool mEuclideanDistance;


};

#endif
