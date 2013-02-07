#ifndef FIREPRESETSTWODIM_H
#define FIREPRESETSTWODIM_H
#include "firePresets.h"
#include"../levelset/ImplicitFunctions.h"


#ifdef __APPLE__
#include "AdvectRK2.h"
#elif defined _WIN32 || defined _WIN64
#include "advect/AdvectRK2.h"
#endif

class FirePresetsTwoDimension : public FirePresets
{
public:
	FirePresetsTwoDimension(){
		implicitFunction = implicitFunction::sphere;
        
        
        //Rutiner
        advect = new AdvectRK2();
	};
	~FirePresetsTwoDimension();

private:

};

const int FirePresets::GRID_DIM_X = 15;
const int FirePresets::GRID_DIM_Y = 15;
const int FirePresets::GRID_DIM_Z = 1;


#endif //FIREPRESETSTWODIM_H