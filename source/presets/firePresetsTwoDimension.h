#ifndef FIREPRESETSTWODIM_H
#define FIREPRESETSTWODIM_H
#include "firePresets.h"
#include"ImplicitFunctions.h"
#include "AdvectLevelSetEuler.h"
#include "UpwindDiff.h"
#include "CentralDiff.h"

#ifdef __APPLE__
#include "MACAdvect.h"
#include "AdvectRK2.h"
#elif defined _WIN32 || defined _WIN64
#include "advect/MACAdvect.h"
#else
#include "MACAdvect.h"
#include "AdvectRK2.h"
#endif



class FirePresetsTwoDimension : public FirePresets
{
public:
	FirePresetsTwoDimension()  
	{
            implicitFunction = implicitFunction::sphere;
        
            //Rutiner
            advection = new AdvectLevelSetEuler();
            advectVelocities = new MACAdvectRK2<double>();
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

const int FirePresets::GRID_DIM_X = 30;
const int FirePresets::GRID_DIM_Y = 30;
const int FirePresets::GRID_DIM_Z = 30;

const double FirePresets::T_IGNITION = 2200;
const double FirePresets::T_MAX = 3000;
const double FirePresets::T_AIR = 20.0 + 273.15;

const double FirePresets::GRID_SIZE = 8;

const double FirePresets::dt = 1.0/30.0;

const double FirePresets::dx = GRID_DIM_X/GRID_SIZE;
const double FirePresets::CFL_NUMBER = 2;

const double FirePresets::S = 0.1;

const double FirePresets::VORTICITY_EPSILON_FUEL = 16.0/3.0;
const double FirePresets::VORTICITY_EPSILON_BURNT = 60.0/3.0;

const double FirePresets::TEMPERATURE_BUOYANCY_ALPHA = 0.15;
const double FirePresets::TEMPERATURE_LOSS_CONSTANT = 3000;

const double FirePresets::rhof = 1.0;
const double FirePresets::rhob = 0.1;

#endif //FIREPRESETSTWODIM_H
