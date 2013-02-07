
#include "fire.h"
#include "advect/Advect.h"

Fire::Fire(FirePresets *p){
    //Presets
    preset = p;

	phi.fillLevelSet(preset->implicitFunction);
    //Grid
    u = VelocityField();
}

double Fire::computeDT(){
    return 0.5;
}

void Fire::advect(double duration){
    preset->advect->advect(u, 0.5);
}

void Fire::runSumulation(){
    
    //Beräkna tidssteget
	double currentTime = 0;

	double dt = computeDT();
	while(currentTime < preset->dt)
	{
		//Advektera hastighestsfältet
		advect(preset->dt);
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