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
	reinitialize::reinitializeGrid(&grid, &gridCopy);
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
	
	double 
		dx = FirePresets::centralDisc->calcDx(*grid, i, j, k),
		dy = FirePresets::centralDisc->calcDy(*grid, i, j, k),
		dz = FirePresets::centralDisc->calcDz(*grid, i, j, k),
		d2x = FirePresets::centralDisc->calcD2x(*grid, i, j, k),
		d2y = FirePresets::centralDisc->calcD2y(*grid, i, j, k),
		d2z = FirePresets::centralDisc->calcD2z(*grid, i, j, k),
		dxy = FirePresets::centralDisc->calcDxy(*grid, i, j, k),
		dxz = FirePresets::centralDisc->calcDxz(*grid, i, j, k),
		dyz = FirePresets::centralDisc->calcDyz(*grid, i, j, k);
	double a = 2.0000000000;
    
	return (dx*dx*(d2y + d2z) - a*dy*dz*dyz + 
		dy*dy*(d2x*d2z) - a*dx*dz*dxz + 
		dz*dz*(d2x + d2y) - a*dx*dy*dxy) /
		(a*std::pow(dx*dx+dy*dy+dz*dz, 1.5));
}

Vector3 LevelSet::getNormal(const int i, const int j, const int k)
{

	//Osäker om detta är korrekt implementation
	Vector3 g = Gradient::getGradient(*grid, i, j, k, *(FirePresets::centralDisc));
	
	// l blir 0 ibland...
	double l = g.norm();
	if(l > 0.0)
		g *= 1.0/l;
	else
		g = Vector3(0.0, 1.0, 0.0);

	return g;
}

Vector3 LevelSet::getNormal(const double w_x, const double w_y, const double w_z)
{
	int i, j, k;
	grid->worldToIndex(i, j, k, w_x, w_y, w_z);

	// TODO FIXA!!!
	//Ej korrekt implementation, då normalen måste interpoleras
	Vector3 g = Gradient::getGradient(*grid, i, j, k, *(FirePresets::centralDisc));

	// l blir 0 ibland...
	double l = g.norm();
	if(l > 0.0)
		g *= 1.0/l;
	else
		g = Vector3(0.0, 1.0, 0.0);

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
            glColor3d(0,0,grid->valueAtIndex(i, j, k));
            
        }else{
            glColor3d(-grid->valueAtIndex(i, j, k)/3.0,0,0);
            
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