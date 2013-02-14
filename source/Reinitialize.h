#ifndef REINITIALIZE_H
#define REINITIALIZE_H

#include <climits>

#include "GridField.hpp"
#include "presets/firePresets.h"


namespace reinitialize{

	void resetToSurface(GridField<double> &g, GridField<bool> *known)
	{
		double treshold = 0.01;
		for(int i = 0; i < FirePresets::GRID_DIM_X; i++)
			for(int j = 0; j < FirePresets::GRID_DIM_Y; j++)
				for(int k = 0; k < FirePresets::GRID_DIM_Z; k++)
				{
					double v = g.valueAtIndex(i, j, k);
					if(v > treshold)
						known->setValueAtIndex(false, i, j, k);
					else
						known->setValueAtIndex(true, i, j, k);
		}

	}
	void fixForOrder(int i, int j, int k, int *in, int *jn, int *kn, int order)
	{
		switch(order)
		{
		case 1:
			*in = FirePresets::GRID_DIM_X - *in;
			break;
		case 2:
			*jn = FirePresets::GRID_DIM_Y - *jn;
			
			break;
		case 3:
			*kn -= FirePresets::GRID_DIM_Z - *kn;
			break;
		case 4:
			*in -= FirePresets::GRID_DIM_X - *in;
			*jn -= FirePresets::GRID_DIM_Y - *jn;
			break;
		case 5:
			*kn -= FirePresets::GRID_DIM_Z - *kn;
			*jn -= FirePresets::GRID_DIM_Y - *jn;
			break;
		case 6:
			*in -= FirePresets::GRID_DIM_X - *in;
			*kn -= FirePresets::GRID_DIM_Z - *kn;
			break;
		case 7:
			*in -= FirePresets::GRID_DIM_X - *in;
			*jn -= FirePresets::GRID_DIM_Y - *jn;
			*kn -= FirePresets::GRID_DIM_Z - *kn;
			break;
		};
	}
	//Fast swiping method
	void fastSwiping(GridField<double> &g, GridField<bool> *known, int order)
	{
		for(int i = 0; i < FirePresets::GRID_DIM_X; i++)
			for(int j = 0; j < FirePresets::GRID_DIM_Y; j++)
				for(int k = 0; k < FirePresets::GRID_DIM_Z; k++)
				{
					int in = i, jn = j, kn = k;
					fixForOrder(i, j, k, &in, &jn, &kn, order);
					
		}
	}

	void swipe(GridField<double> &g, GridField<bool> *known)
	{
		for(int j = 0; j < 2; j++)
		{
			for(int i = 0; i < 8; i++)
			{
				fastSwiping(g, known, i);
			}
		}
	}
	void reinitializeGrid(GridField<double> &g)
	{
/*		GridField<bool> *known = new GridField<bool>(FirePresets::GRID_DIM_X,FirePresets::GRID_DIM_Y,FirePresets::GRID_DIM_Z);
		resetToSurface(g, known);
		swipe(g, known);
*/
	}
}


#endif
