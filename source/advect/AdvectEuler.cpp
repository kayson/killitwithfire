#include "AdvectEuler.h"
#include <limits>
#include "../presets/firePresets.h"
#include "../Gradient.h"

void AdvectEuler::advect(VelocityField &v, Grid &g, double dt){
    for(int i = 0; i < g.getDimX(); i++)
	{
		for(int j = 0; j < g.getDimY(); j++)
		{
			for(int k = 0; k < g.getDimZ(); k++)
			{
				//Fulhack eftersom att vi inte har med border kontroll.
				if(!(i == 0  || j == 0 || i == g.getDimX()-1 || j == g.getDimY()-1))
				{
					if(g.getDimZ() != 1 && (k == 0 || k == g.getDimZ()-1))
						continue;
					double f = evaluate(v, g, i, j, k);
					g.setData(i, j, k, g(i,j,k) + f * dt);
				}
			}
		}
	}

}

double AdvectEuler::evaluate(VelocityField &v, Grid &g, unsigned int i, unsigned int j, unsigned int k){
	Vector3 pos = Vector3(i,j,k);
	double xv, yv, zv;

	
	Vector3 gradPhi = Gradient::getGradient(g, i, j, k, *discretization);


	Vector3 normalGrad = Gradient::getGradient(g, i, j, k, *normalDiscretization);



	Vector3 localUnitNormal = normalGrad / gradPhi.norm();

	Vector3 vel = v.getVelocityAtCoordinate(pos);
	vel = (vel + localUnitNormal * FirePresets::S) * 1.0;

	return vel.dot(gradPhi);
}