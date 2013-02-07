
#include "fire.h"

Fire::Fire(FirePresets *p){
    //Presets
    preset = p;
    
    //Grid
    grid = Grid();
}

void Fire::runSumulationStep(){
    //Advect
    preset->advect->advect(grid, 0.5);
    //External forces
    //preset->externalForce->addForce(grid);
    //Project
}

Fire::~Fire(){
    delete preset;
}