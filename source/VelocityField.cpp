#include "VelocityField.h"


VelocityField::VelocityField()
{
    
}

data VelocityField::getMax()
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

Vector3 getPositionAtFace(int i ,int j, int k, DirectionEnums direction){
    
    double dx = 50; //Använd global variabel
    double dx2 = dx/2.; 
    double x = i*dx+dx2;
    double y = j*dx+dx2;
    double z = k*dx+dx2;
    
    if (direction == RIGHT) {
        x += dx;
    }else if (direction == DOWN){
        y += dx;
    }else if (direction == FORWARD){
        z += dx;
    }
    
    return Vector3(x,y,z);
}


Vector3 VelocityField::getVelocityAtWorldCoordinate(Vector3 &pos){
    
    
    Vector3 v = Vector3();
    return v;
}
