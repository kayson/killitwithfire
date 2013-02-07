#ifndef FIREPRESETSTWODIM_H
#define FIREPRESETSTWODIM_H
#include "firePresets.h"
#include"../levelset/ImplicitFunctions.h"
#include "advect/AdvectRK2.h"

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