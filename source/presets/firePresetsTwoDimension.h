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

const int FirePresets::GRID_DIM_X = 80;
const int FirePresets::GRID_DIM_Y = 160;
const int FirePresets::GRID_DIM_Z = 80;

const double FirePresets::T_IGNITION = 2200;
const double FirePresets::T_MAX = 3000;
const double FirePresets::T_AIR = 20.0 + 273.15;

const double FirePresets::GRID_SIZE = 4;

const double FirePresets::dt = 1.0/100.0;

//const double FirePresets::dx = GRID_DIM_X/GRID_SIZE; //Borde väl vara GRID_SIZE/GRID_DIM_X, vågar dock inte ändra då indextoWorld osv kan bero på det. Implicit funktionen behöver ändras då med. //Axel

const double FirePresets::CFL_NUMBER = 2;

const double FirePresets::S = 0.2;

const double FirePresets::VORTICITY_EPSILON_FUEL = 16.0/1.0;
const double FirePresets::VORTICITY_EPSILON_BURNT = 32.0/1.0;

const double FirePresets::TEMPERATURE_BUOYANCY_ALPHA = 0.3;
const double FirePresets::TEMPERATURE_LOSS_CONSTANT = 3000;

const double FirePresets::CHROMA = 100; //Chromatic adaption, högt värde minskar intensiteten
const int FirePresets::SAMPLE_STEP = 15; //Antal man hoppar över (1 är minsta, dvs man använder alla sampel)

const double FirePresets::rhof = 1.0;
const double FirePresets::rhob = 0.01;

#endif //FIREPRESETSTWODIM_H
