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
#ifndef __uniform_cubic_spline_h__
#define __uniform_cubic_spline_h__

#include "Math/Vector3.h"
#include "Geometry/Geometry.h"
#include "Geometry/LineStrip.h"
#include <vector>
#include <iostream>
#include <cmath>

class UniformCubicSpline : public Geometry {
protected:
  //! The coefficients dictating the shape
  std::vector<Vector3<float> > mCoefficients;
  //! The control polygon is simply a LineStrip
  LineStrip mControlPolygon;

  //! Decides the length of the linear approximating segments used when drawing
  float mDt;

	int mBSplineEvaluations;

  // display information
  Vector3<float> mLineColor;
  float mLineWidth;

public:
  UniformCubicSpline(const std::vector<Vector3<float> > &joints,
                     Vector3<float> lineColor = Vector3<float>(0.f,1.f,0.f),
                     float lineWidth = 2.f,
                     float segmentLength = .01f);

  /*! The BSpline value is calculated from one of the four cardinal BSpline
   * segments
   */
  float GetBSplineValue(int i, float t);

  /*! Evaluate the spline as the sum of the coefficients times the bsplines */
  Vector3<float> GetValue(float t);

  virtual void Update() { }
  virtual void Initialize() { }

  virtual void Render();

  virtual const char * GetTypeName() { return typeid(UniformCubicSpline).name(); }

};
#endif
