#include "VelocityField.h"


VelocityField::VelocityField()
{

}

VelocityField::VelocityField(const VelocityField &v){
    this->xDim = v.xDim; this->yDim = v.yDim; this->zDim = v.zDim;
    centerVelocities = VectorGrid(xDim, yDim, zDim);
    initVelocityField();
}

void VelocityField::operator=(const VelocityField &v){
    this->xDim = v.xDim; this->yDim = v.yDim; this->zDim = v.zDim;
    centerVelocities = VectorGrid(xDim, yDim, zDim);
    initVelocityField();
}

void VelocityField::initVelocityField()
{
	for(int i = 0; i < xDim; i++)
	{
		for(int j = 0; j < yDim; j++)
		{
			for(int k = 0; k < zDim; k++)
			{
				centerVelocities(i, j, k) = Vector3(1., 0., 0.);
				centerVelocities(i, j, k).normalize();
			}
		}	
	}
	std::cout << centerVelocities(0, 0, 0);
}

double VelocityField::getMax()
{
	double max = 0;
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
	}*/
	double cv = centerVelocities.getMax();
	if( cv > max)
		max = cv;
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


Vector3 VelocityField::getVelocityAtCoordinate(Vector3 &pos){
    int a = (int) pos.x, b = (int) pos.y, c = (int) pos.z;
	//std::cout << "getVelocityAtCoordinate, (a,b,c) = ("<<a<<", "<<b<<", "<<c<<")\n";
	Vector3 v = centerVelocities(a, b, c);
    //Interpolera!
    
    return v;
}

void VelocityField::setCenterVel(Vector3 &v, const int i, const int j, const int k)
{
	centerVelocities.setData(i,j,k,v);
	return;
}
