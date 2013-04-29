#ifndef __SPHERE_FRACTAL_H__
#define __SPHERE_FRACTAL_H__

#include "Geometry/Implicit.h"
#include <vector>
#include "Math/Matrix4x4.h"
#include "Geometry/Sphere.h"
#include "Geometry/CSG.h"
#include "Geometry/Bbox.h"
#include "Geometry/Geometry.h"

/*! \brief Fractal of spheres class*/
class SphereFractal : public Implicit {
  public:

    //! Constructor. Level denotes the level of the fractal. Level can be between 2 and 5
          SphereFractal(int level = 2);

    ~SphereFractal();

    virtual float GetValue(float x, float y, float z) const;

    //! Builds the fractal. Returns a pointer to an implicit geometry object.
    Implicit* buildFractal();

  private:
    std::vector<Implicit*> mSpheres;
    Implicit* mFractal;

};

#endif
