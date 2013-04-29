
#include "UniformCubicSplineSubdivisionCurve.h"

UniformCubicSplineSubdivisionCurve::UniformCubicSplineSubdivisionCurve(const std::vector<Vector3<float> > &joints,
                                                                       Vector3<float> lineColor,
                                                                       float lineWidth)
  : mCoefficients(joints), mControlPolygon(joints)
{
  this->mLineColor = lineColor;
  this->mLineWidth = lineWidth;
}


void UniformCubicSplineSubdivisionCurve::Subdivide()
{
	// Allocate space for new coefficients
	std::vector<Vector3<float> > newc;

	assert(mCoefficients.size() > 4 && "Need at least 5 points to subdivide");

	// Implement the subdivision scheme for a natural cubic spline here
	for(unsigned int i = 0; i < mCoefficients.size(); i++)
	{
		if(i > 0 && i < mCoefficients.size()-1)
		{
			Vector3<float> c1 = mCoefficients.at(i-1);
			Vector3<float> c2 = mCoefficients.at(i);
			Vector3<float> c3 = mCoefficients.at(i+1);

			newc.push_back( 1.0/8.0*(c1 + 6.0*c2 + c3) );
		}
		else 
			newc.push_back(mCoefficients.at(i));

		newc.push_back(mCoefficients.at(i));

		//C(i+½)
		if(i < mCoefficients.size()-1)
		{
			Vector3<float> c2 = mCoefficients.at(i);
			Vector3<float> c3 = mCoefficients.at(i+1);

			newc.push_back( 1.0/2.0*(c2 + c3) );
		}
	}

	// If 'mCoefficients' had size N, how large should 'newc' be? Perform a check here!
	assert(true && "Incorrect number of new coefficients!");

	mCoefficients = newc;
}


void UniformCubicSplineSubdivisionCurve::Render()
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
	// just draw the spline as a series of connected linear segments
	for(unsigned int i = 0; i < mCoefficients.size(); i++){
	glVertex3fv( mCoefficients.at(i).GetArrayPtr() );
	}
	glEnd();

	// restore attribs
	glPopAttrib();

	glPopMatrix();

	GLObject::Render();
}

