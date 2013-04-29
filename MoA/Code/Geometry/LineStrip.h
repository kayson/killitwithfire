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
#ifndef __line_strip_h__
#define __line_strip_h__

#include "Geometry/Geometry.h"
#include "Math/Vector3.h"
#include <vector>
#include <iostream>

class LineStrip : public Geometry
{
protected:
  std::vector<Vector3<float> > mJoints;

  // display information
  Vector3<float> mJointColor;
  Vector3<float> mLineColor;
  float mJointSize;
  float mLineWidth;

public:
  LineStrip(const std::vector<Vector3<float> > &joints);

  virtual void Update() { }

  virtual void Initialize() { }

  virtual void Render();

  virtual const char * GetTypeName() { return typeid(LineStrip).name(); }
};
#endif
