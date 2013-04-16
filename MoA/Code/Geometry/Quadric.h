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
#ifndef __quadric_h__
#define __quadric_h__

#include "Geometry/Implicit.h"

/*!  \brief Quadric base class */
class Quadric : public Implicit {
public:
  //! Initialize the quadric from matrix q
  Quadric(const Matrix4x4<float> & q);
  virtual ~Quadric();
  //! evaluate the quadric at world coordinates x y z
  virtual float GetValue(float x, float y, float z) const;
  //! calculate the gradient at world coordinates x y z
  virtual Vector3<float> GetGradient(float x, float y, float z) const;
  //virtual float Quadric::GetCurvature(float x, float y, float z) const;
  virtual void SetTransform(const Matrix4x4<float> & transform);
protected:
  //! The quadrics coefficent matrix
  Matrix4x4<float> mQuadric;
  Matrix4x4<float> transformQ;
};

#endif
