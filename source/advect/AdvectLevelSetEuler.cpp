#include "AdvectLevelSetEuler.h"
#include "GridField.hpp"
#include "LevelSet.h"
#include "firePresets.h"
#include "Gradient.h"
#include <cmath>

void AdvectLevelSetEuler::advect(GridField<Vector3> &w, LevelSet &phi, double dt){
	//Räkna ut dt för levelsetet
	double vmax = 0.0;
	for(int i = 0; i < w.xdim(); i++)
	{
		for(int j = 0; j < w.ydim(); j++)
		{
			for(int k = 0; k < w.zdim(); k++)
			{
				vmax = std::max(vmax, std::max(w.valueAtIndex(i, j, k).x, std::max(w.valueAtIndex(i, j, k).y, w.valueAtIndex(i, j, k).z)));
			}
		}
	}
	double ldt = FirePresets::dx/vmax;
	if(ldt > dt) ldt = dt;

	double steps = dt/ldt;

	do{//Justera så att man gör levelset advectionen under tiden dt, går nog att göra snyggare än så här
		for(int i = 0; i < (phi.grid)->xdim(); i++)
		{
			for(int j = 0; j < (phi.grid)->ydim(); j++)
			{
				for(int k = 0; k < (phi.grid)->zdim(); k++)
				{
					double f = evaluate(w, phi, i, j, k);
					phi.gridCopy->setValueAtIndex((*phi.grid)(i,j,k) + f * ldt, i, j, k);
				}
			}
		}

		steps -= 1.0;
		if(steps < 1.0) ldt *= steps;

		GridField<double> *temp = phi.grid;
		phi.grid = phi.gridCopy;
		phi.gridCopy = temp;
	}while(steps > 0.0);
}

double AdvectLevelSetEuler::evaluate(GridField<Vector3> &w, LevelSet &phi, unsigned int i, unsigned int j, unsigned int k){
	Vector3 gradPhi = Gradient::getGradient(*phi.grid, i, j, k, *FirePresets::upwindDisc);

	Vector3 vel = w(i,j,k);
	return -Vector3::dot(vel, gradPhi);
}
