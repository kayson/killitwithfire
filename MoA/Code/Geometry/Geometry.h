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
#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "Math/Matrix4x4.h"
#include "GUI/GLObject.h"

/*!  \brief Geometry base class */
class Geometry : public GLObject {
public:
  Geometry() {}
  virtual ~Geometry() {};

  //! Update geometry, e.g. differential quantities after modifications
  virtual void Update() = 0;

  //! Initialize geometry
  virtual void Initialize() = 0;

  //! Translate geometry
  void Translate(float x, float y, float z) {
    SetTransform(mTransform * Matrix4x4<float>::Translation(x,y,z));
  }

  //! Scale geometry
  void Scale(float s) {
    SetTransform(mTransform * Matrix4x4<float>::Scale(s));
  }

  //! Scale geometry
  void Scale(float x, float y, float z) {
    SetTransform(mTransform * Matrix4x4<float>::Scale(x,y,z));
  }

  //! Rotate geometry
  void Rotate(float rx, float ry, float rz) {
    SetTransform(mTransform * Matrix4x4<float>::RotationXYZ(rx,ry,rz));
  }

  //! Dilate geometry
  virtual void Dilate(float amount) {
    std::cout << "Dilate() not implemented for this type of geometry" << std::endl;
  }

  //! Erode geometry
  virtual void Erode(float amount) {
    std::cout << "Erode() not implemented for this type of geometry" << std::endl;
  }

  //! Smooth geometry
  virtual void Smooth(float amount) {
    std::cout << "Smooth() not implemented for this type of geometry" << std::endl;
  }

  //! Set transformation
  virtual void SetTransform(const Matrix4x4<float> & transform) {
    mTransform = transform;
  }

  //! Get transform
  Matrix4x4<float>& GetTransform(){
    return mTransform;
  }
  const Matrix4x4<float>& GetTransform() const {
    return mTransform;
  }

protected:
  Matrix4x4<float> mTransform;
};

#endif
