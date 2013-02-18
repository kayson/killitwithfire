
#include "fire.h"
#include "advect/Advect.h"

#include "GridField.hpp"

Fire::Fire(FirePresets *p):phi(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z),celltype(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z){
    //Presets
    preset = p;

	u = VelocityField(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z);

	phi.fillLevelSet(preset->implicitFunction);
	
	preset->discretization->setVectorGrid(u.getCenterVel());
	preset->normalDiscretization->setVectorGrid(u.getCenterVel());

    preset->advect->setDiscretization(preset->discretization, preset->normalDiscretization);

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
	preset->advect->advect(u, phi.phi, phi.temp, duration);
}

void Fire::computeCellTypes()
{
	for(GridFieldIterator<int> it = celltype.iterator(); !it.done(); it.next())
	{
		int i, j, k;
		it.index(i, j, k);
        
        if(celltype(i,j,k)){
            
        }   
        
		celltype.setValueAtIndex(getCellType(i,j,k), i, j, k);
        
        
            
	}
}	

CellType Fire::getCellType(const int i, const int j, const int k)
{
	if(false) //Check if is solid
		return SOLID;
	else if(phi.phi->valueAtIndex(i,j,k) <= 0.0)
		return BLUECORE;
	else 
		return IGNITED;
}
int count = 0;
void Fire::runSimulation(){
    
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
	/*if(count % 50 == 0)
		phi.reinitialize();*/
	count++;
}

void Fire::drawCenterVelocities()
{
	for(int i = 0; i < phi.phi->xdim(); i += 5)
	{
		for(int j = 0; j < phi.phi->ydim(); j += 5)
		{
			for(int k = 0; k < phi.phi->zdim(); k += 5)
			{
				float x0 = FirePresets::dx*((double)(i) + 0.5);
				float y0 = FirePresets::dx*((double)(j) + 0.5);
				float z0 = FirePresets::dx*((double)(k) + 0.5);
                
				Vector3 v = u.centerVelocities(i, j, k)*FirePresets::dx;
				glColor3f(0,1,0);
				glBegin(GL_LINES);
					glVertex3f(x0, y0, z0);
					glVertex3f(x0 + v.x, y0 + v.y, z0 + v.z);
				glEnd();

				glColor3f(0,0,0);
				glBegin(GL_POINTS);
					glVertex3f(x0 + v.x, y0 + v.y, z0 + v.z);
				glEnd();
			}
		}
	}
	
}

void Fire::draw()
{
	phi.draw();
	drawCenterVelocities();
}

Fire::~Fire(){
    delete preset;
}