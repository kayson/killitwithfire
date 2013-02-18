#include "AdvectEuler.h"
#include <limits>
#include "../presets/firePresets.h"
#include "../Gradient.h"


void AdvectEuler::advect(MACGrid &u, GridField<double> *grid,GridField<double> *gridCopy, double dt){
    for(int i = 0; i < grid->xdim(); i++)
	{
		for(int j = 0; j < grid->ydim(); j++)
		{
			for(int k = 0; k < grid->zdim(); k++)
			{

				double f = evaluate(u, *grid, i, j, k);
				grid->setValueAtIndex((*grid)(i,j,k) + f * dt, i, j, k);
				
			}
		}
	}
	/*GridField<double> *temp = ng;
	ng = g;
	g = temp;*/
}

double AdvectEuler::evaluate(MACGrid &u, GridField<double> &g, unsigned int i, unsigned int j, unsigned int k){
	Vector3 pos = Vector3(i,j,k);
	double xv, yv, zv;
	
	Vector3 gradPhi = Gradient::getGradient(g, i, j, k, *upwindDiscretization);

	Vector3 normalGrad = Gradient::getGradient(g, i, j, k, *centralDiscretization);
	
	Vector3 vel = u.velocityAtIndex(pos)*-1.0;
	/*
	double l = normalGrad.norm();
	if(l != 0)
	{
		Vector3 localUnitNormal = normalGrad / l * -1.0;
		vel = (vel + localUnitNormal * FirePresets::S) * 1.0;
	}*/
	
	return vel.dot(gradPhi);
}