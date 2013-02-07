
#include "fire.h"
#include "advect/Advect.h"

Fire::Fire(FirePresets *p){
    //Presets
    preset = p;

	phi.fillLevelSet(preset->implicitFunction);
    //Grid
    u = VelocityField();
}

double Fire::computeDT(double currentTime){
	double smallStep;

	//Bridson s. 35
	double dx = preset->dx;
	double alpha = preset->CFL_NUMBER;
	double c = u.findMaximumVelocity();
	if(c != 0)
		smallStep = alpha * dx / c;
	else
		smallStep = dx;

	//Fixa overshoot
	/*double diff = currentTime + smallStep - preset->dt;
	if(diff > 0)
	{
		smallStep -= diff;
	}*/
	return smallStep;
}

void Fire::advect(double duration){
    preset->advect->advect(u,u.u, 0.5);
    preset->advect->advect(u,u.v, 0.5);
    preset->advect->advect(u,u.w, 0.5);

}

void Fire::runSumulation(){
    
    //Beräkna tidssteget
	double currentTime = 0;

	while(currentTime < preset->dt)
	{
		double dt = computeDT(currentTime);

		//Advektera hastighestsfältet
		advect(dt);

		currentTime += dt;
	}

    
    //Externa krafter
    
    //preset->externalForce->addForce(grid);
    
    //Project
    
    //Advektera levelset
    
}

void Fire::draw()
{
	phi.draw();
}

Fire::~Fire(){
    delete preset;
}