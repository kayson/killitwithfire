#include "LevelSet.h"

#include "../Discretization.h"
#include "../CentralDiff.h"
#include "../Gradient.h"
#include "../Reinitialize.h"
#include <iostream>
#include <cmath>


#ifdef __APPLE__
#include "firePresets.h"
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include "../presets/firePresets.h"
#include <GL/glfw.h>
#endif


void LevelSet::fillLevelSet(double (*implicitFunction)(int, int, int))
{
	for(int i = 0; i < grid->xdim(); i++){
		for(int j = 0; j < grid->ydim(); j++){
			for(int k = 0; k < grid->zdim(); k++){
                grid->setValueAtIndex( implicitFunction(i,j,k),i,j,k);
                //std::cout << phi.valueAtIndex(i, j, k);
            }
        }
    }
    
}

Vector3 LevelSet::getVelocity(MACGrid &g, int i, const int j, const int k){
    return Vector3(0,0,0);
}

void LevelSet::reinitialize()
{
	reinitialize::reinitializeGrid(*grid);
}
void LevelSet::printDistanceField()
{
	for(int i = 0; i < grid->xdim(); i++)
	{
		for(int j = 0; j < grid->ydim(); j++)
		{
			for(int k = 0; k < grid->zdim(); k++)
			{
				std::cout << grid->valueAtIndex(i,j,k) << " ";
			}
			
		}
		std::cout << std::endl;
	}
}

double LevelSet::getCurvature(const int i, const int j, const int k)
{
    
	Discretization *disc = new CentralDiff();
	
	double 
	dx = disc->calcDx(*grid, i, j, k),
	dy = disc->calcDy(*grid, i, j, k),
	dz = disc->calcDz(*grid, i, j, k),
	d2x = disc->calcD2x(*grid, i, j, k),
	d2y = disc->calcD2y(*grid, i, j, k),
	d2z = disc->calcD2z(*grid, i, j, k),
	dxy = disc->calcDxy(*grid, i, j, k),
	dxz = disc->calcDxz(*grid, i, j, k),
	dyz = disc->calcDyz(*grid, i, j, k);
	double a = 2.0000000000;
    
    delete disc;
    
	return (dx*dx*(d2y + d2z) - a*dy*dz*dyz + dy*dy*(d2x*d2z) - a*dx*dz*dxz + dz*dz*(d2x + d2y) - a*dx*dy*dxy)/(a*std::pow(dx*dx+dy*dy+dz*dz, 1.5));
}

Vector3 LevelSet::getNormal(const int i, const int j, const int k)
{
	Discretization *disc = new CentralDiff();

	//Osäker om detta är korrekt implementation
	Vector3 g = Gradient::getGradient(*grid, i, j, j, *disc);
	g.normalize();

	delete disc;
	
	return g;
}

void LevelSet::draw() const
{

    for (GridFieldIterator<double> iter = grid->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        grid->indexToWorld(i, j, k, x, y, z);
        
        
        if(grid->valueAtIndex(i,j,k) > 0)
        {
            glColor3f(0,0,grid->valueAtIndex(i, j, k)/3.);
            
        }else{
            glColor3f(-grid->valueAtIndex(i, j, k)/50.0,0,0);
            
        }
        glBegin(GL_QUADS);

        glVertex3f((float)x, (float)y, (float)z);
        glVertex3f(((float)x+1.f), (float)y, (float)z);
        glVertex3f(((float)x+1.f), ((float)y+1.f), (float)z);
        glVertex3f((float)x, ((float)y+1.f), (float)z);
        glEnd();

    }
    

    
	/*double dx = FirePresets::dx;

	glBegin(GL_QUADS);

	for(int x = 0; x < phi->xdim()-1; x++)
	{
		for(int y = 0; y < phi->ydim()-1; y++)
		{
			for(int z = 0; z < phi->zdim(); z++)
			{
                
                
                
				if(phi->valueAtIndex(x,y,z) <= 0)
				{
                    glColor3f(0,0,-phi->valueAtIndex(x, y, z)/3.0);
                    //glColor3f(0,0,1);
                                    
				}else{
					glColor3f(phi->valueAtIndex(x, y, z)/50.0,0,0);
                    //glColor3f(1,0,0);
                
                }
                
                glVertex3f(dx*(float)x, dx*(float)y, dx*(float)z);
                glVertex3f(dx*((float)x+1.f), dx*(float)y, dx*(float)z);
                glVertex3f(dx*((float)x+1.f), dx*((float)y+1.f), dx*(float)z);
                glVertex3f(dx*(float)x, dx*((float)y+1.f), dx*(float)z);
			}
		}
	}
	glEnd();
     */
}