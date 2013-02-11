#include "AdvectEuler.h"
#include <limits>

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

	Vector3 vel = v.getVelocityAtCoordinate(pos);
	vel *= -1;
	Vector3 gradPhi;
	gradPhi.x = discretization->calcDx(g, pos.x, pos.y, pos.z);
	gradPhi.y = discretization->calcDy(g, pos.x, pos.y, pos.z);
	if(g.getDimZ() != 1)
		gradPhi.z = discretization->calcDz(g, pos.x, pos.y, pos.z);
	else
		gradPhi.z = 0;

	return vel.dot(gradPhi);
}