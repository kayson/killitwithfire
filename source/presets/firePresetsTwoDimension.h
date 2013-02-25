#ifndef FIREPRESETSTWODIM_H
#define FIREPRESETSTWODIM_H
#include "firePresets.h"
#include"../levelset/ImplicitFunctions.h"
#include "../advect/AdvectLevelSetEuler.h"
#include "../UpwindDiff.h"
#include "../CentralDiff.h"

#ifdef __APPLE__
#include "AdvectRK2.h"
#elif defined _WIN32 || defined _WIN64

#endif



class FirePresetsTwoDimension : public FirePresets
{
public:
	FirePresetsTwoDimension()  
	{
		implicitFunction = implicitFunction::sphere;
        
        //Rutiner
        advection = new AdvectLevelSetEuler();
		advectVelocities = new MACAdvectEuler<double>();
	}

	~FirePresetsTwoDimension(){
        delete advection;
		delete advectVelocities;
    }

private:

};

UpwindDiff *FirePresets::upwindDisc = new UpwindDiff();
CentralDiff *FirePresets::centralDisc = new CentralDiff();

const int FirePresets::GRID_DIM_X = 40;
const int FirePresets::GRID_DIM_Y = 47;
const int FirePresets::GRID_DIM_Z = 10;
const double FirePresets::GRID_SIZE = 18;
const double FirePresets::dt = 1./80.;
const double FirePresets::dx = 1./8.;
const double FirePresets::CFL_NUMBER = .2;
const double FirePresets::S = .5;
const double FirePresets::rhof = 1;
const double FirePresets::rhoh = 0.1;

#endif //FIREPRESETSTWODIM_H