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
#ifndef __uc_spline_subdiv_h__
#define __uc_spline_subdiv_h__

#include "Subdivision.h"
#include "Math/Vector3.h"
#include "Geometry/Geometry.h"
#include "Geometry/LineStrip.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

class UniformCubicSplineSubdivisionCurve : public Subdivision, public Geometry {
protected:
  //! The coefficients dictating the shape
  std::vector<Vector3<float> > mCoefficients;
  //! The control polygon is simply a LineStrip
  LineStrip mControlPolygon;

  // display information
  Vector3<float> mLineColor;
  float mLineWidth;

public:
  UniformCubicSplineSubdivisionCurve(const std::vector<Vector3<float> > &joints,
                                     Vector3<float> lineColor = Vector3<float>(0.f,1.f,0.2f),
                                     float lineWidth = 2.f);

  virtual void Update() { }
  virtual void Initialize() { }

  virtual void Subdivide();

  virtual void Render();

  virtual const char * GetTypeName() { return typeid(UniformCubicSplineSubdivisionCurve).name(); }

};
#endif
