#ifndef FIREPRESETS_H
#define FIREPRESETS_H


#ifdef __APPLE__
#include "Advect.h"
#elif defined _WIN32 || defined _WIN64
#include "../advect/Advect.h"
#endif


class FirePresets
{
public:
	FirePresets(){};
	~FirePresets(){};
	double (*implicitFunction)(int x, int y, int z);


    //Konstanter
    static const double dx;
    static const double dt;
	static const double CFL_NUMBER;
    
	static const int GRID_DIM_X;
	static const int GRID_DIM_Y;
	static const int GRID_DIM_Z;
	static const double GRID_SIZE;

	static const double S;

	static const double rhof; // Fuel density (kg/m^3)
	static const double rhoh; // Hot gaseous products density (kg/m^3)

    //Rutiner
	Advect *advection;
    //Discretization *upwindDiscretization;
	//Discretization *centralDiscretization;
	static Discretization *upwindDisc;
	static Discretization *centralDisc;
};


#endif //FIREPRESETS_H