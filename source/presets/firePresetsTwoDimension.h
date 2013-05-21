#ifndef FIREPRESETSTWODIM_H
#define FIREPRESETSTWODIM_H
#include "firePresets.h"
#include"ImplicitFunctions.h"
#include "AdvectLevelSetEuler.h"
#include "UpwindDiff.h"
#include "CentralDiff.h"
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

const int FirePresets::GRID_DIM_X = 20;
const int FirePresets::GRID_DIM_Y = 40;
const int FirePresets::GRID_DIM_Z = 20;

const double FirePresets::T_IGNITION = 2200;
const double FirePresets::T_MAX = 3000;
const double FirePresets::T_AIR = 20.0 + 273.15;

const double FirePresets::GRID_SIZE = 8;

const double FirePresets::dt = 1.0/100.0;

const double FirePresets::dx = GRID_DIM_X/GRID_SIZE; //Borde väl vara GRID_SIZE/GRID_DIM_X, vågar dock inte ändra då indextoWorld osv kan bero på det. Implicit funktionen behöver ändras då med. //Axel

const double FirePresets::CFL_NUMBER = 2;

const double FirePresets::S = 0.25;

const double FirePresets::VORTICITY_EPSILON_FUEL = 16.0/3.0;
const double FirePresets::VORTICITY_EPSILON_BURNT = 60.0/3.0;

const double FirePresets::TEMPERATURE_BUOYANCY_ALPHA = 1.15;
const double FirePresets::TEMPERATURE_LOSS_CONSTANT = 3000;

const double FirePresets::rhof = 1.0;
const double FirePresets::rhob = 0.01;

#endif //FIREPRESETSTWODIM_H
