#ifndef FIREPRESETSTWODIM_H
#define FIREPRESETSTWODIM_H
#include "firePresets.h"
#include"../levelset/ImplicitFunctions.h"

class FirePresetsTwoDimension : public FirePresets
{
public:
	FirePresetsTwoDimension()
	{
		implicitFunction = implicitFunction::sphere;
	};
	~FirePresetsTwoDimension();

private:

};

const int FirePresetsTwoDimension::GRID_DIM_X = 15;
const int FirePresetsTwoDimension::GRID_DIM_Y = 15;
const int FirePresetsTwoDimension::GRID_DIM_Z = 1;


#endif //FIREPRESETSTWODIM_H