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

const int FirePresets::GRID_DIM_X = 50;
const int FirePresets::GRID_DIM_Y = 50;
const int FirePresets::GRID_DIM_Z = 1;

const double FirePresets::T_IGNITION = 537.0 + 273.15; //  Methane ignition temp (537 °C).
const double FirePresets::T_MAX = 1600 + 273.15;	// Bunsen burner (Methane) flame temp (900-1600 °C).
const double FirePresets::T_AIR = 20.0 + 273.15;

const double FirePresets::GRID_SIZE = 18;
const double FirePresets::dt = 1.0/10.0;
const double FirePresets::dx = 1.0/8.0;
const double FirePresets::CFL_NUMBER = .2;

const double FirePresets::S = 0.5;

const double FirePresets::VORTICITY_EPSILON = 2.5;

const double FirePresets::TEMPERATURE_BUOYANCY_ALPHA = 0.004;
const double FirePresets::TEMPERATURE_LOSS_CONSTANT = 300;

const double FirePresets::rhof = 1.0;
const double FirePresets::rhob = 0.2;

#endif //FIREPRESETSTWODIM_H