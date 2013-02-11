#include "AdvectEuler.h"


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
					double ddt = evaluate(v, g, i, j, k);
					g.setData(i, j, k, g(i,j,k) + ddt * dt);
				}
			}
		}
	}
}

double AdvectEuler::evaluate(VelocityField &v, Grid &g, unsigned int i, unsigned int j, unsigned int k){
	Vector3 pos = Vector3(i,j,k);
	double xv, yv, zv;

	//Fattas världstransform här!

	Vector3 vel = v.getVelocityAtWorldCoordinate(pos);
	xv = discretization->calcDx(g, pos.x, pos.y, pos.z);
	yv = discretization->calcDy(g, pos.x, pos.y, pos.z);
	if(g.getDimZ() != 1)
		zv = discretization->calcDz(g, pos.x, pos.y, pos.z);
	else
		zv = 0;

	return -(vel.x * xv + vel.y * yv + vel.z * zv);
}