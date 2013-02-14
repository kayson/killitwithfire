#include "AdvectEuler.h"
#include <limits>
#include "../presets/firePresets.h"
#include "../Gradient.h"

void AdvectEuler::advect(VelocityField &v, GridField<double> &g, double dt){
    for(int i = 0; i < g.xdim(); i++)
	{
		for(int j = 0; j < g.ydim(); j++)
		{
			for(int k = 0; k < g.zdim(); k++)
			{
				if(!borderCondition.checkBorder(g,i,j,k))
					borderCondition.enforceBorderCondition(v,g,i,j,k);
				{
					double f = evaluate(v, g, i, j, k);
					g.setValueAtIndex(g(i,j,k) + f * dt, i, j, k);// setData(i, j, k, );
				}
			}
		}
	}
}

double AdvectEuler::evaluate(VelocityField &v, GridField<double> &g, unsigned int i, unsigned int j, unsigned int k){
	Vector3 pos = Vector3(i,j,k);
	double xv, yv, zv;
	
	Vector3 gradPhi = Gradient::getGradient(g, i, j, k, *discretization);

	Vector3 normalGrad = Gradient::getGradient(g, i, j, k, *normalDiscretization);
	double l = normalGrad.norm();
	Vector3 vel = v.getVelocityAtCoordinate(pos)*-1.0;
	if(l != 0)
	{
		Vector3 localUnitNormal = normalGrad / l * -1.0;
		vel = (vel + localUnitNormal * FirePresets::S) * 1.0;
	}
	return vel.dot(gradPhi);
}