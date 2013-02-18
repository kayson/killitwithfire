#ifndef CREATELEVELSET_H
#define CREATELEVELSET_H

#include <climits>

#include "GridField.hpp"
#include "presets/firePresets.h"

#include <cmath>

#ifdef __APPLE__
#include "glm.hpp"
#elif defined _WIN32 || defined _WIN64
#include <glm/glm.hpp>
#endif

using namespace std;
using namespace glm;

const double DEFINED = true;
const double UNDEFINED = false;

template <typename T> T sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

namespace CreateLevelset{

	void resetToSurface(GridField<double> &g, GridField<dvec4> *known)
	{
		double treshold = 0.3;
		for(int i = 0; i < FirePresets::GRID_DIM_X; i++)
			for(int j = 0; j < FirePresets::GRID_DIM_Y; j++)
				for(int k = 0; k < FirePresets::GRID_DIM_Z; k++)
				{
					double vo = g.valueAtIndex(i, j, k);
					double v = abs(vo);
					if(v > treshold)
					{
						dvec4 v = dvec4((double)i, (double)j, (double)k, UNDEFINED);
						known->setValueAtIndex(v, i, j, k);

						g.setValueAtIndex(DBL_MAX * sgn<double>(vo), i, j, k);

					}
					else
					{
						dvec4 v = dvec4((double)i, (double)j, (double)k, DEFINED);
						known->setValueAtIndex(v, i, j, k);
					}
		}

	}
		
	void fixForOrder(int i, int j, int k, int *in, int *jn, int *kn, int order)
	{
		switch(order)
		{
		case 1:
			*in = FirePresets::GRID_DIM_X - i - 1;
			break;
		case 2:
			*jn = FirePresets::GRID_DIM_Y - j - 1;
			break;
		case 3:
			*in = FirePresets::GRID_DIM_X - i - 1;
			*jn = FirePresets::GRID_DIM_Y - j - 1;
			break;
		case 4:
			*kn = FirePresets::GRID_DIM_Z - k - 1;
			break;
		case 5:
			*kn = FirePresets::GRID_DIM_Z - k - 1;
			*jn = FirePresets::GRID_DIM_Y - j - 1;
			break;
		case 6:
			*in = FirePresets::GRID_DIM_X - i - 1;
			*kn = FirePresets::GRID_DIM_Z - k - 1;
			break;
		case 7:
			*in = FirePresets::GRID_DIM_X - i - 1;
			*jn = FirePresets::GRID_DIM_Y - j - 1;
			*kn = FirePresets::GRID_DIM_Z - k - 1;
			break;
		};
	}
	
	//Fast swiping method
	void fastSwiping(GridField<double> &g, GridField<dvec4> *known, int order)
	{
		for(int i = 0; i < FirePresets::GRID_DIM_X; i++)
		{
			for(int j = 0; j < FirePresets::GRID_DIM_Y; j++)
			{
				for(int k = 0; k < FirePresets::GRID_DIM_Z; k++)
				{
					int in = i, jn = j, kn = k;
					fixForOrder(i, j, k, &in, &jn, &kn, order);
					//Om gridpunkt inte är känd
					if(known->valueAtIndex(in,jn,kn).w == UNDEFINED)
					{
						//Spara nuvarande position
						double closest = abs(g.valueAtIndex(in, jn, kn));
						double closestSign = 1;
						dvec3 closestPos = dvec3(0.0,0.,0.);
						bool changed = false;
						for(int x = 0; x < 3; x++)
						{
							for(int y = 0; y < 3; y++)
							{
								for(int z = 0; z < 3; z++)
								{
									Vector3 p = Vector3(x,y,z) - Vector3(1, 1, 1);
									int ioffset = in + p.x;
									int joffset = jn + p.y;
									int koffset = kn + p.z;
									bool threeDim = (g.zdim() != 1);
									//Inte mittenpunkt och känd
									if(!(x == 1 && y == 1 && z == 1))
									{
										if((ioffset != -1 && joffset != -1 && (koffset != -1)))
										{
											if(!(ioffset == g.xdim() || joffset == g.ydim() || (koffset == g.zdim()) ))
											{
												if(koffset == 0 || threeDim)
												{
													if(known->valueAtIndex(ioffset, joffset, koffset).w == DEFINED)
													{
														//Räkna ut avstånd
														double lengthOfOther = g.valueAtIndex(ioffset, joffset, koffset);
														double lengthOfOtherAbs = abs(lengthOfOther);

														dvec4 tempPos = known->valueAtIndex(ioffset, joffset,koffset);
														dvec3 surfacePos = dvec3(tempPos.x, tempPos.y, tempPos.z);

														double length = glm::length(dvec3((double)in, (double)jn,(double)kn) - surfacePos);

														length *= FirePresets::dx;


														if(length < lengthOfOtherAbs)
														{
															dvec4 newKnown = known->valueAtIndex(ioffset, joffset, koffset);
															newKnown.w = UNDEFINED;
															known->setValueAtIndex(newKnown, ioffset, joffset, koffset);
														}
														else if(closest > length)
														{
															closest = length;
															closestSign = sgn<double>(lengthOfOther);
															closestPos = surfacePos;
															changed = true;
														}
														
													}
												}
											}
										}
									}
								}
							}
						}				
						if(changed)
						{
							dvec4 newKnown = known->valueAtIndex(closestPos.x, closestPos.y, closestPos.z);
							newKnown.w = DEFINED;
							known->setValueAtIndex(newKnown, in, jn, kn);
							g.setValueAtIndex(closest * closestSign, in, jn, kn);
						}
					}
				}
			}
		}
	}
	void showUnknown(GridField<double> &g, GridField<dvec4> *known)
	{
		int c = 0;
		int m = 0;
		for(int i = 0; i < FirePresets::GRID_DIM_X; i++)
			for(int j = 0; j < FirePresets::GRID_DIM_Y; j++)
				for(int k = 0; k < FirePresets::GRID_DIM_Z; k++)
				{
					if(known->valueAtIndex(i,j,k).w == UNDEFINED)
						c++;
					if(abs(g.valueAtIndex(i,j,k)) == DBL_MAX)
						m++;
					
		}
		cout << "Num unknown: " << c << " ant max: " << m << endl;

	}
	void swipe(GridField<double> &g, GridField<dvec4> *known)
	{
		for(int j = 0; j < 2; j++)
		{
			for(int i = 0; i < 8; i++)
			{
				/*if((g.zdim() == 1 && i < 4))
					fastSwiping(g, known, i);
				else if(g.zdim() > 1)*/
					fastSwiping(g, known, i);
				showUnknown(g, known);
			}
		}
	}
	
	void reinitializeGrid(GridField<double> &g)
	{
		GridField<dvec4> *known = new GridField<dvec4>(FirePresets::GRID_DIM_X,FirePresets::GRID_DIM_Y,FirePresets::GRID_DIM_Z);

		resetToSurface(g, known);
		swipe(g, known);
	}
}


#endif
