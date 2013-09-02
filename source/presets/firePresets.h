#ifndef FIREPRESETS_H
#define FIREPRESETS_H

template<class T> class MACAdvect;
class CentralDiff;
class UpwindDiff;
class AdvectLevelSet;

#include "CentralDiff.h"
#include "UpwindDiff.h"

template<class T> class MACAdvect;
class FirePresets
{
public:
	FirePresets(){}
	~FirePresets(){}
	double (*implicitFunction)(int x, int y, int z);

	// constants
    static const double dt;
	static const double CFL_NUMBER;
    
	static const int GRID_DIM_X;
	static const int GRID_DIM_Y;
	static const int GRID_DIM_Z;
	static const double GRID_SIZE;
	static const double T_IGNITION;
	static const double T_MAX;
	static const double T_AIR;

	static const double VORTICITY_EPSILON_FUEL;
	static const double VORTICITY_EPSILON_BURNT;

	static const double TEMPERATURE_BUOYANCY_ALPHA;
	static const double TEMPERATURE_LOSS_CONSTANT;

	static const double TEMPERATUR_MULT; // 1 means that the temperature is the same as the rest, 2 means twice the resolution and so on.

	static const double S;

	static const double rhof; // Fuel density (kg/m^3)
	static const double rhob; // Hot gaseous products density (kg/m^3)

	static const double CHROMA; //Chromatic adaption, högt värde minskar intensiteten
	static const int SAMPLE_STEP; 
	static const double SAMPLE_DL; 
	static const int SAMPLE_STEP_QUALITY;
	static const int TOTAL_SAMPLES;
	static const bool QUALITY_ROOM;
	

    //Rutiner
	AdvectLevelSet *advection;
	MACAdvect<double> *advectVelocities;
	void advectLevelSet(double duration);
	static UpwindDiff *upwindDisc;
	static CentralDiff *centralDisc;
    static MACAdvect<double> *tempAdvect;
};


#endif //FIREPRESETS_H
