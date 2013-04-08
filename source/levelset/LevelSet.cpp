#include "LevelSet.h"

#include "Gradient.h"
#include "Reinitialize.h"
#include <iostream>
#include <cmath>

#include "GridField.hpp"

#if defined __unix__ || defined __APPLE__
#include "firePresets.h"
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include "../presets/firePresets.h"
#include <GL/glfw.h>
#endif

LevelSet::LevelSet()
{
	grid  = new GridField<double>(1000,1000,1000);
	gridCopy = new GridField<double>(1000, 1000, 1000,10);
	normals = new GridField<Vector3>(1000, 1000, 1000, 10);
}

LevelSet::LevelSet(int xDim, int yDim, int zDim, double size)
{
	grid  = new GridField<double>(xDim,yDim,zDim,size);
	gridCopy = new GridField<double>(xDim,yDim,zDim,size);
	normals = new GridField<Vector3>(xDim,yDim,zDim,size);
        
    grid->multTransformation(glm::scale(1.0, 1.0, 1.0));
    gridCopy->multTransformation(glm::scale(1.0, 1.0, 1.0));
    normals->multTransformation(glm::scale(1.0, 1.0, 1.0));      
}

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

CellType LevelSet::getCellType(const int i, const int j, const int k) const {
	if(i < 2 || i >= (this->grid->xdim() - 2) || j< 2 || j >= (this->grid->ydim() - 2) ) //Check if is solid
		return SOLID;
	else if(this->grid->valueAtIndex(i,j,k) > 0.0)
		return FUEL;
	else 
		return BURNT;
}

CellType LevelSet::getCellType(const double w_x, const double w_y, const double w_z) const {
	return getCellType(this->grid->valueAtWorld(w_x, w_y, w_z));
}

CellType LevelSet::getCellType(const double phi){
	if(false) // Check if solid
		return SOLID;
	else if(phi > 0.0)
		return FUEL;
	else
		return BURNT;
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

void LevelSet::updateNormals(){
	for(int i = 0; i < grid->xdim(); i++){
		for(int j = 0; j < grid->ydim(); j++){
			for(int k = 0; k < grid->zdim(); k++){
				Vector3 g = Gradient::getGradient(*grid, i, j, k, *(FirePresets::centralDisc));
				double l = g.norm();
				if(l > 0.0)
					g *= 1.0/l;
				else
					g = Vector3(0.0, 1.0, 0.0);

                normals->setValueAtIndex(g, i, j, k);
            }
        }
    }
}

Vector3 LevelSet::getNormal(const int i, const int j, const int k)
{
	return normals->valueAtIndex(i,j,k);
}

Vector3 LevelSet::getNormal(const double w_x, const double w_y, const double w_z)
{
	return normals->valueAtWorld(w_x, w_y, w_z);
}
void LevelSet::drawNormals() const{

	glColor3d(1.0,1.0,1.0);
	for( GridFieldIterator<Vector3> iter = normals->iterator(); !iter.done(); iter.next() ){
		int i,j,k;
		iter.index(i,j,k);
		double x,y,z;
		normals->indexToWorld(i,j,k,x,y,z);
		Vector3 val = iter.value() / 5.0;

		glBegin(GL_LINE_STRIP);
		glVertex3d(x,y,0);
		glVertex3d(x+val.x, y+val.y,0);
		glEnd();
	
	}
}

void LevelSet::draw() const
{
	glBegin(GL_QUADS);

    for (GridFieldIterator<double> iter = grid->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        grid->indexToWorld(i, j, k, x, y, z);
        
        
        if(grid->valueAtIndex(i,j,k) > 0)
        {
			glColor3d(0,0,grid->valueAtIndex(i, j, k));
            
        }
		else
		{
            glColor3d(-grid->valueAtIndex(i, j, k)*0.1,-grid->valueAtIndex(i, j, k)*0.1,0);
            
        }
       

        glVertex3f((float)x, (float)y, (float)z);
        glVertex3f(((float)x+1.f), (float)y, (float)z);
        glVertex3f(((float)x+1.f), ((float)y+1.f), (float)z);
        glVertex3f((float)x, ((float)y+1.f), (float)z);
        

    }

    glEnd();
}
