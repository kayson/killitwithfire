#ifndef FIREPRESETS_H
#define FIREPRESETS_H
#include "advect/Advect.h"

class FirePresets
{
public:
	FirePresets(){};
	~FirePresets(){};
	data (*implicitFunction)(int x, int y, int z);


    //Konstanter
    static const double dx;
    static const double dt;

    
	static const int GRID_DIM_X;
	static const int GRID_DIM_Y;
	static const int GRID_DIM_Z;
	static const double GRID_SIZE;
    
    //Rutiner
	Advect *advect;
    
};


#endif //FIREPRESETS_H