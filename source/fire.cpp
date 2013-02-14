
#include "fire.h"
#include "advect/Advect.h"

#include "GridField.hpp"

Fire::Fire(FirePresets *p):phi(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z){
    //Presets
    preset = p;
	//GridField<double> g(1,1,1);
	//Grid
	u = VelocityField(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z);

	
	phi.fillLevelSet(preset->implicitFunction);
	
	preset->discretization->setVectorGrid(u.getCenterVel());
	preset->normalDiscretization->setVectorGrid(u.getCenterVel());

    preset->advect->setDiscretization(preset->discretization, preset->normalDiscretization);

	GridField<CellType> *g = new GridField<CellType>(5,1,1);

}

double Fire::computeDT(double currentTime){
	double smallStep;

	//Bridson s. 35
	double dx = preset->dx;
	double alpha = preset->CFL_NUMBER;
	double c = u.getMax();
	if(c != 0)
		smallStep = alpha * dx / c;
	else
		smallStep = dx;

	//Fixa overshoot

	if(smallStep > preset->dt - currentTime)
	{
		smallStep = preset->dt - currentTime;
	}
	return smallStep;
}

void Fire::advectVelocityField(double duration){
    /*preset->advect->advect(u, u.u, duration);
    preset->advect->advect(u, u.v, duration);
    preset->advect->advect(u, u.w, duration);*/
}

void Fire::advectLevelSet(double duration)
{
	preset->advect->advect(u, phi.phi, duration);
}

void Fire::computeCellTypes()
{
	for(GridFieldIterator<int> it = celltype.iterator(); !it.done(); it.next())
	{
		int i, j, k;
		it.index(i, j, k);
		celltype.setValueAtIndex(getCellType(i,j,k), i, j, k);
	}
}	

CellType Fire::getCellType(const int i, const int j, const int k)
{
	if(false) //Check if is solid
		return SOLID;
	else if(phi.phi(i,j,k) <= 0.0)
		return BLUECORE;
	else 
		return IGNITED;
}

void Fire::runSumulation(){
    
    //Beräkna tidssteget
	

	for(double currentTime = 0; currentTime < preset->dt;)
	{
		double dt = computeDT(currentTime);

		//Advektera hastighestsfältet
		advectVelocityField(dt);

		currentTime += dt;
	}

    
    //Externa krafter
    
    //preset->externalForce->addForce(grid);
    
    //Project
    
    //Advektera levelset
    for(double currentTime = 0; currentTime < preset->dt;)
	{
		double dt = computeDT(currentTime);

		//Advektera hastighestsfältet
		advectLevelSet(dt);

		currentTime += dt;
	}

	computeCellTypes(); //Beräkna om vad för typ voxlarna är

	//Fixa signed distance field
	phi.reinitialize();
}

void Fire::draw()
{
	phi.draw();
}

Fire::~Fire(){
    delete preset;
}