
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

void Fire::runSumulation(double duration){
    
    //Beräkna tidssteget

    //Advektera hastighestsfältet
    advect(duration);
    
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