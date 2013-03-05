#include "AdvectLevelSetEuler.h"
#include "GridField.hpp"
#include "levelset\LevelSet.h"
#include "presets\firePresets.h"
#include "Gradient.h"

void AdvectLevelSetEuler::advect(GridField<Vector3> &w, LevelSet &phi, double dt){
    for(int i = 0; i < (phi.grid)->xdim(); i++)
	{
		for(int j = 0; j < (phi.grid)->ydim(); j++)
		{
			for(int k = 0; k < (phi.grid)->zdim(); k++)
			{
				double f = evaluate(w, phi, i, j, k);
				phi.gridCopy->setValueAtIndex((*phi.grid)(i,j,k) + f * dt, i, j, k);
			}
		}
	}

	GridField<double> *temp = phi.grid;
	phi.grid = phi.gridCopy;
	phi.gridCopy = temp;
}

double AdvectLevelSetEuler::evaluate(GridField<Vector3> &w, LevelSet &phi, unsigned int i, unsigned int j, unsigned int k){
	Vector3 gradPhi = Gradient::getGradient(*phi.grid, i, j, k, *FirePresets::upwindDisc);

	Vector3 vel = w(i,j,k);
	return -Vector3::dot(vel, gradPhi);
}