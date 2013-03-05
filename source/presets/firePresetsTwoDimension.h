#ifndef FIREPRESETSTWODIM_H
#define FIREPRESETSTWODIM_H
#include "firePresets.h"
#include"../levelset/ImplicitFunctions.h"
#include "../advect/AdvectLevelSetEuler.h"
#include "../UpwindDiff.h"
#include "../CentralDiff.h"
#include "../advect/MACAdvect.h"

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

MACAdvect<double> *FirePresets::tempAdvect = new MACAdvectRK2<double>();

const int FirePresets::GRID_DIM_X = 64;
const int FirePresets::GRID_DIM_Y = 64;
const int FirePresets::GRID_DIM_Z = 1;

const double FirePresets::T_IGNITION = 50.0;
const double FirePresets::T_MAX = 1200.0;
const double FirePresets::T_AIR = 25.0;

const double FirePresets::GRID_SIZE = 18;
const double FirePresets::dt = 1.0/10.0;
const double FirePresets::dx = 1.0/8.0;
const double FirePresets::CFL_NUMBER = .2;

const double FirePresets::S = 0.5;

const double FirePresets::VORTICITY_EPSILON = 2.5;

const double FirePresets::TEMPERATURE_BUOYANCY_ALPHA = 0.0004;
const double FirePresets::TEMPERATURE_LOSS_CONSTANT = .001;

const double FirePresets::rhof = 1;
const double FirePresets::rhob = 0.1;

#endif //FIREPRESETSTWODIM_H