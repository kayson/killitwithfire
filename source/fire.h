#ifndef FIRE_H
#define FIRE_H

#include "AdvectRK2.h"
#include "firePresets.h"
#include "Advect.h"

class Fire
{
public:
	Fire(FirePresets *preset);
	~Fire();
    
    void runSumulationStep();

protected:
    FirePresets *preset;
    
    //Grid
    Grid grid;
};
#endif //FIRE_H
