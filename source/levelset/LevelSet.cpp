#include "LevelSet.h"

#include "../Discretization.h"
#include "../CentralDiff.h"
#include "../Gradient.h"
#include "../Reinitialize.h"
#include <iostream>
#include <cmath>


#ifdef __APPLE__
#include "firePresets.h"
#elif defined _WIN32 || defined _WIN64
#include "../presets/firePresets.h"
#endif

void LevelSet::fillLevelSet(double (*implicitFunction)(int, int, int))
{
	for(int i = 0; i < phi.xdim(); i++)
		for(int j = 0; j < phi.ydim(); j++)
			for(int k = 0; k < phi.zdim(); k++)
			{
				phi.setValueAtIndex(implicitFunction(i, j, k),i,j,k);
	}
}
void LevelSet::printDistanceField()
{
	for(int i = 0; i < phi.xdim(); i++)
	{
		for(int j = 0; j < phi.ydim(); j++)
		{
			for(int k = 0; k < phi.zdim(); k++)
			{
				std::cout << phi.valueAtIndex(i,j,k) << " ";
			}
			
		}
		std::cout << std::endl;
	}
}

double LevelSet::getCurvature(const int i, const int j, const int k)
{
    
	Discretization *disc = new CentralDiff();
	
	double 
	dx = disc->calcDx(phi, i, j, k),
	dy = disc->calcDy(phi, i, j, k),
	dz = disc->calcDz(phi, i, j, k),
	d2x = disc->calcD2x(phi, i, j, k),
	d2y = disc->calcD2y(phi, i, j, k),
	d2z = disc->calcD2z(phi, i, j, k),
	dxy = disc->calcDxy(phi, i, j, k),
	dxz = disc->calcDxz(phi, i, j, k),
	dyz = disc->calcDyz(phi, i, j, k);
	double a = 2.0000000000;
    
    delete disc;
    
	return (dx*dx*(d2y + d2z) - a*dy*dz*dyz + dy*dy*(d2x*d2z) - a*dx*dz*dxz + dz*dz*(d2x + d2y) - a*dx*dy*dxy)/(a*std::pow(dx*dx+dy*dy+dz*dz, 1.5));
}

Vector3 LevelSet::getNormal(const int i, const int j, const int k)
{
	Discretization *disc = new CentralDiff();

	//Osäker om detta är korrekt implementation
	Vector3 g = Gradient::getGradient(phi, i, j, j, *disc);
	g.normalize();

	delete disc;
	
	return g;
}

void LevelSet::draw() const
{
	double dx = FirePresets::dx;

	for(int x = 0; x < phi.xdim()-1; x++)
	{
		for(int y = 0; y < phi.ydim()-1; y++)
		{
			for(int z = 0; z < phi.zdim(); z++)
			{
                glBegin(GL_QUADS);
				
				if(phi.valueAtIndex(x,y,z) <= 0)
				{
                    //glColor3f(0,0,-grid[x][y][z]/8.0);
                    glColor3f(0,0,1);
                
                    
				}else{
					//glColor3f(grid[x][y][z]/100.0,0,0);
                    glColor3f(1,0,0);
                
                }
                
                glVertex3f(dx*(float)x, dx*(float)y, dx*(float)z);
                glVertex3f(dx*((float)x+1.f), dx*(float)y, dx*(float)z);
                glVertex3f(dx*((float)x+1.f), dx*((float)y+1.f), dx*(float)z);
                glVertex3f(dx*(float)x, dx*((float)y+1.f), dx*(float)z);
                
                glEnd();

			}
		}
	}
}