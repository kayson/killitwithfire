
#include "UniformCubicSpline.h"

UniformCubicSpline::UniformCubicSpline(const std::vector<Vector3<float> > &joints,
                                       Vector3<float> lineColor,
                                       float lineWidth,
                                       float segmentLength)
  : mCoefficients(joints), mControlPolygon(joints)
{
  this->mLineColor = lineColor;
  this->mLineWidth = lineWidth;
  this->mDt = segmentLength;
}

/*! The BSpline value is calculated from one of the four cardinal BSpline
 * segments
 */
float UniformCubicSpline::GetBSplineValue(int i, float t)
{
	mBSplineEvaluations++;

  // Find the offset from spline i
  t = t - (float)i;
  if(std::abs(t) >= 2){
    // if outside of support return zero
    return 0;
  } else if(0 <= t && t < 1)
  {
    // get the fractional part
    float ft = t;
    return 1/6.0*(3*ft*ft*ft - 6*ft*ft + 4);
  }
  else if(1 <= t && t < 2)
  {
    // get the fractional part
    float ft = t - 1;
    return 1/6.0*(-ft*ft*ft + 3*ft*ft - 3*ft + 1);
  }
  else if(-1 <= t && t < 0)
  {
    // get the fractional part
    float ft = t + 1;
    return 1/6.0*(-3*ft*ft*ft + 3*ft*ft + 3*ft + 1);
  }
  else if(-2 < t && t < -1)
  {
    // get the fractional part
    float ft = t + 2;
    return 1/6.0*(ft*ft*ft);
  }

  return 0;
}

/*! Evaluate the spline as the sum of the coefficients times the bsplines */
Vector3<float> UniformCubicSpline::GetValue(float t)
{
	Vector3<float> val;
	float sum = 0;

	int a = ((int)(t-1) >= 0) ?  (int)(t-1) : 0;
	int b = ((int)(t+2) <= mCoefficients.size()-1) ? (int)(t+2) : mCoefficients.size()-1;
	
	//Om den ligger precis på en punkt ci så påverkas den endast av ci-1 och ci+1
	float delta = t-(int)t;
	if(-0.001 < delta && delta < 0.001 && (int)(t+1) <= mCoefficients.size()-1)
	{
		b = (int)(t+1);
	}

	for (unsigned int i = a; i <= b; i++) 
	{
		float bval = GetBSplineValue(i, t);
		val += mCoefficients.at(i)*bval;
		sum += bval;
	}
	return val;
}

void UniformCubicSpline::Render()
{
  // Apply transform
  glPushMatrix(); // Push modelview matrix onto stack

  // Convert transform-matrix to format matching GL matrix format
  // Load transform into modelview matrix
  glMultMatrixf( mTransform.ToGLMatrix().GetArrayPtr() );

  mControlPolygon.Render();

  // save line point and color states
  glPushAttrib(GL_POINT_BIT | GL_LINE_BIT | GL_CURRENT_BIT);

  // draw segments
  glLineWidth(mLineWidth);
  glColor3fv(mLineColor.GetArrayPtr());
  glBegin(GL_LINE_STRIP);

  // We only have full BSpline support from spline at index 1, thus we begin evaluating at 1.0
	mBSplineEvaluations = 0;
  for(float i = 1; i < mCoefficients.size()-2; i+= mDt){
    glVertex3fv( this->GetValue(i).GetArrayPtr());
  }
  glEnd();

	std::cout << "Number of B-spline evaluations " << mBSplineEvaluations << std::endl;

  // restore attribs
  glPopAttrib();

  glPopMatrix();

  GLObject::Render();
}

