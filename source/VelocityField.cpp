#include "VelocityField.h"


VelocityField::VelocityField()
{
    
}

data& VelocityField::getMax()
{
	data max = 0;
	if ( u.getMax() > max )
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

	return max;
}

Vector3 VelocityField::getVelocityAtWorldCoordinate(Vector3 &pos){
    
    
    Vector3 v = Vector3();
    
    
    
    return v;
}
