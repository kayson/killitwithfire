
#ifdef __WXMAC__
#include "GLUT/glut.h"
#else
#include <GL/glut.h>
#endif

#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Util/trackball.h"
/////////////////////////////////
//Note: All angles in degrees  //
/////////////////////////////////

class GLCamera
{
private:
  Vector3<float> mCameraFwd;
  Vector3<float> mCameraPos;
  Vector3<float> mCameraUp;
  float curquat[4]; // a representation of the rotation quaternion
  bool useCameraRotation;
public:
  GLCamera(Vector3<float> v=Vector3<float>(0,0,0)){
    mCameraPos = v;
    mCameraUp = Vector3<float>(0,1,0);
    mCameraFwd = Vector3<float>(0,0,-1);
    mCameraFwd.Normalize();
    trackball(curquat, 0, 0, 0, 0);
    useCameraRotation = false;
  }
  void SetCameraRotation(bool r) {
    useCameraRotation = r;
  }

  void RotateX(float r){
    if(useCameraRotation) {
    r = r*(1/180.f);
    mCameraUp = cos(r)*mCameraUp + sin(r)*mCameraFwd;
    mCameraFwd = cos(r)*mCameraFwd - sin(r)*mCameraUp;
    }
  }
  void RotateY(float r){
    if(useCameraRotation) {
    r = r*(1/180.f);
    mCameraFwd = cos(r)*mCameraFwd + sin(r)*GetRightVector();
    }
  }
  void RotateZ(float r){
    if(useCameraRotation) {
    r = r*(1/180.f);
    mCameraUp = cos(r)*mCameraUp + sin(r)*GetRightVector();
    }
  }

  void QuatRotate(float x1, float y1, float x2, float y2)
  {
    float lastquat[4];
    trackball(lastquat, x1,y1, x2,y2);  // calculates a reasonable quaternion using the start and end points
    add_quats(lastquat, curquat, curquat); // Quats can be accumulated for a smooth rotation effect
  }

  Vector3<float> GetPosition() const { return mCameraPos; }
  Vector3<float> GetLookAtVector() const{ return mCameraFwd; }
  Vector3<float> GetRightVector() const{
    return Vector3<float>(Cross(GetLookAtVector(), mCameraUp));
  }
  Vector3<float> GetUpVector() const{ return mCameraUp; }
  Vector3<float> GetLookAtPoint() const{
    return Vector3<float>(mCameraPos + mCameraFwd);
  }
  void LookAtOrigo(){
    mCameraFwd = -mCameraPos;
  }
  void Reset(){
    *this = GLCamera();
  }
  void Move(const Vector3<float> &m){
    mCameraPos += m;
  }
  void MoveForward(float r){
    Move(GetLookAtVector()*r);
  }
  void MoveUpward(float r){
    Move(GetUpVector()*r);
  }
  void MoveRight(float r){
    Move(GetRightVector()*r);
  }
  void Render(){
    gluLookAt(mCameraPos[0], mCameraPos[1], mCameraPos[2],
              mCameraPos[0]+mCameraFwd[0], mCameraPos[1]+mCameraFwd[1], mCameraPos[2]+mCameraFwd[2],
              mCameraUp[0], mCameraUp[1], mCameraUp[2]);
    GLfloat m[4][4];
    build_rotmatrix(m, curquat); // using trackball to compute the equivalent rotation matrix
    glMultMatrixf(&m[0][0]);
  }
  Vector3<float> Spherical2Cartesian(const Vector3<float> &spherical) const{
    const float & r = spherical[0];
    const float & phi = spherical[1];
    const float & theta = spherical[2];

    const float x = r*sin(theta)*sin(phi);
    const float y = r*cos(theta);
    const float z = r*sin(theta)*cos(phi);

    return Vector3<float>(x, y, z);
  }
  Vector3<float> Cartesian2Spherical(const Vector3<float> &cartesian) const{
    const float &x = cartesian[0];
    const float &y = cartesian[1];
    const float &z = cartesian[2];

    const float r = cartesian.Length();
    const float theta = acos(y/r);
    const float phi = atan2(x, z);

    return Vector3<float>(r, phi, theta);
  }
};


