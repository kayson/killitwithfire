#ifndef FIRE_H
#define FIRE_H

#include "firePresets.h"
#include "VelocityField.h"

class Fire{
protected:
    double computeDT();
    void advect(double duration);
public:
	Fire(FirePresets *preset);
	~Fire();
    
    void runSumulation(double duration);
protected:
    FirePresets *preset;
    
    //Grid
    VelocityField u;
};

#endif //FIRE_H
