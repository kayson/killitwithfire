#ifndef FIREPRESETSTWODIM_H
#define FIREPRESETSTWODIM_H
#include "firePresets.h"
#include"../levelset/ImplicitFunctions.h"
#include "../advect/AdvectEuler.h"
#include "../UpwindDiff.h"
#include "../CentralDiff.h"

#ifdef __APPLE__
#include "AdvectRK2.h"
#elif defined _WIN32 || defined _WIN64
#endif

class FirePresetsTwoDimension : public FirePresets
{
public:
	FirePresetsTwoDimension(){
		implicitFunction = implicitFunction::sphere;
        
        //Rutiner
        advect = new AdvectEuler();
		discretization = new UpwindDiff();
		normalDiscretization = new CentralDiff();
		
	};
	~FirePresetsTwoDimension(){
        delete advect;
        delete discretization;
    }

private:

};

const int FirePresets::GRID_DIM_X = 4*35;
const int FirePresets::GRID_DIM_Y = 4*35;
const int FirePresets::GRID_DIM_Z = 5;
const double FirePresets::dt = 1./200.;
const double FirePresets::dx = 1./8.;
const double FirePresets::CFL_NUMBER = .2;
const double FirePresets::S = .5;

#endif //FIREPRESETSTWODIM_H