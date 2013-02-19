#include "AdvectEuler.h"
#include <limits>
#include "../presets/firePresets.h"
#include "../Gradient.h"


void AdvectEuler::advect(MACGrid &u, GridField<double> **grid,GridField<double> **gridCopy, double dt){
    for(int i = 0; i < (*grid)->xdim(); i++)
	{
		for(int j = 0; j < (*grid)->ydim(); j++)
		{
			for(int k = 0; k < (*grid)->zdim(); k++)
			{
				double f = evaluate(u, **grid, i, j, k);
				(*gridCopy)->setValueAtIndex((**grid)(i,j,k) + f * dt, i, j, k);
			}
		}
	}

	GridField<double> *temp = *grid;
	*grid = *gridCopy;
	*gridCopy = temp;
}

double AdvectEuler::evaluate(MACGrid &u, GridField<double> &g, unsigned int i, unsigned int j, unsigned int k){
	Vector3 pos = Vector3(i,j,k);
	double xv, yv, zv;
	
	Vector3 gradPhi = Gradient::getGradient(g, i, j, k, *FirePresets::upwindDisc);

	Vector3 normal = Gradient::getGradient(g, i, j, k, *FirePresets::centralDisc);
	if(normal.norm() > 0.0)
		normal.normalize();
	else
		normal = Vector3(0.0, 1.0, 0.0);

	//Vector3 vel = (u.velocityAtCenter(pos) - normal*FirePresets::S*0.2);
	Vector3 vel = u.velocityAtCenter(pos);
	return Vector3::dot(-vel, gradPhi);
}