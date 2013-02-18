#ifndef REINITIALIZE_H
#define REINITIALIZE_H

#include <climits>

#include "GridField.hpp"
#include "presets/firePresets.h"

double Evaluate(unsigned int i, unsigned int j, unsigned int k)
{
	return 1.0;
}


namespace reinitialize{

	

	void reinitializeGrid(GridField<double> &g)
	{
		double time = FirePresets::dt;
		double dt = 0.5 * time;
		
		for(double elapsed = 0; elapsed < time;)
		{
			//Dilate grid

			//

			if(dt > time)
				dt = time - elapsed;

			//Integrate

			//
			break;
			//Rebuild grid
		}
	}
}


#endif
