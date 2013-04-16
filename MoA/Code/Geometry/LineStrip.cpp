
#include "LineStrip.h"

LineStrip::LineStrip(const std::vector<Vector3<float> > &joints) : mJoints(joints)
{
  mJointColor = Vector3<float>(1, 0, 0);
  mLineColor = Vector3<float>(0, 0, 1);
  mLineWidth = 3;
  mJointSize = 5;
}

void LineStrip::Render()
{
  // save line point and color states
  glPushAttrib(GL_POINT_BIT | GL_LINE_BIT | GL_CURRENT_BIT);

  // draw joints
  glPointSize(mJointSize);
  glColor3fv(mJointColor.GetArrayPtr());
  glBegin(GL_POINTS);
  for(std::vector<Vector3<float> >::const_iterator it(mJoints.begin());
      it!=mJoints.end();
      it++){
    glVertex3fv((*it).GetArrayPtr());
  }
  glEnd();

  // draw segments
  glLineWidth(mLineWidth);
  glColor3fv(mLineColor.GetArrayPtr());
  glBegin(GL_LINE_STRIP);
  for(std::vector<Vector3<float> >::const_iterator it(mJoints.begin());
      it!=mJoints.end();
      it++){
    glVertex3fv((*it).GetArrayPtr());
  }
  glEnd();

  // restore attribs
  glPopAttrib();

  GLObject::Render();
}
