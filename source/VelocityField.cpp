#include "VelocityField.h"


VelocityField::VelocityField()
{
    
}

Vector3 VelocityField::getVelocityAtWorldCoordinate(Vector3 &pos){
    
    
    Vector3 v = Vector3();
    
    
    
    return v;
}


double VelocityField::getMax()
{
	double max = 1.2; //DBL_MIN;
	/*if ( u.getMax() > max )
	{
		max = u.getMax();
	}
	if ( v.getMax() > max )
	{
		max = v.getMax();
	}
	if ( w.getMax() > max )
	{
		max = w.getMax();
	}
	*/
	return max;
}
