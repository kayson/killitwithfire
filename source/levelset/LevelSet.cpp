#include "LevelSet.h"
#include "DetonationShockDynamics.h"
#include "Gradient.h"
#include "Reinitialize.h"
#include <iostream>
#include <cmath>

#include "GridField.hpp"
#include "ClosestValueExtrapolation.h"
#include "ConstantValueExtrapolation.h"
#include "SimpleLevelSetExtrapolation.h"

#if defined __unix__ || defined __APPLE__
#include "firePresets.h"
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include "../presets/firePresets.h"
#include <GL/glfw.h>
#endif

#ifdef __APPLE__
#define _isnan(x) isnan(x)
#endif

LevelSet::LevelSet()
{

	grid  = new GridField<double>(10,10,10, FirePresets::GRID_SIZE, new SimpleLevelSetExtrapolation()); //TODO KORREKT EXTRAPOLERING?
	initLevelSet();

	gridCopy = new GridField<double>(10, 10, 10,10, new SimpleLevelSetExtrapolation()); //TODO KORREKT EXTRAPOLERING?
	normals = new GridField<Vector3>(10, 10, 10, 10, new ClosestValueExtrapolation<Vector3>()); //TODO KORREKT EXTRAPOLERING?

//	grid  = new GridField<double>(1000,1000,1000);
//	gridCopy = new GridField<double>(1000, 1000, 1000,10);
	normals = new GridField<Vector3>(1000, 1000, 1000,10,new ConstantValueExtrapolation<Vector3>());
	dsd = new DetonationShockDynamics();
}

LevelSet::LevelSet(int xDim, int yDim, int zDim, double size)
{
	grid  = new GridField<double>(xDim,yDim,zDim,size, new SimpleLevelSetExtrapolation()); //TODO KORREKT EXTRAPOLERING? bör fixa så det blir signed distance på dessa
	initLevelSet();

	gridCopy = new GridField<double>(xDim,yDim,zDim,size, new SimpleLevelSetExtrapolation()); //TODO KORREKT EXTRAPOLERING?
	normals = new GridField<Vector3>(xDim,yDim,zDim,size, new ClosestValueExtrapolation<Vector3>()); //TODO KORREKT EXTRAPOLERING?
        
    grid->multTransformation(glm::scale(1.0, 1.0, 1.0));
    gridCopy->multTransformation(glm::scale(1.0, 1.0, 1.0));
    normals->multTransformation(glm::scale(1.0, 1.0, 1.0));      
	dsd = new DetonationShockDynamics();
	
}

//Initiera levelsetet så det inte har någon yta från början
void LevelSet::initLevelSet()
{
	for(int i = 0; i < grid->xdim(); i++){
		for(int j = 0; j < grid->ydim(); j++){
			for(int k = 0; k < grid->zdim(); k++){
                grid->setValueAtIndex(-FLT_MAX, i,j,k);
            }
        }
    }
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
void LevelSet::fillLevelSetFromConverter(MeshToVolumeConverter &converter)
{
	for(int i = 0; i < grid->xdim(); i++){
		for(int j = 0; j < grid->ydim(); j++){
			for(int k = 0; k < grid->zdim(); k++){
				double x, y, z;
				grid->indexToWorld(i, j, k, x, y, z);
				grid->setValueAtIndex( converter.fill(x,y,z),i,j,k);
                //std::cout << phi.valueAtIndex(i, j, k);
            }
        }
    }
    
}

Vector3 LevelSet::getVelocity(MACGrid &g, int i, const int j, const int k){
    assert(false);
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

// Beräknar volumen på levelsetet (FUEL)
double LevelSet::getVolume() const
{
	double volume = 0.0;
	double H;
	double idx = 1.0/grid->dx();
	for (GridFieldIterator<double> it = grid->iterator(); !it.done(); it.next()) {
        if (it.value()) {
            int i,j,k;
            it.index(i, j, k);
			double val = it.value();
			if(val > grid->dx())
				H = 1.0;
			else if(val < -grid->dx())
				H = 0.0;
			else
				H = 0.5 + 0.75 * val * idx - 0.25 * pow(val*idx, 3.0);
			volume += H;
        }
    }

	return volume * pow(grid->dx(), 3.0);
}

CellType LevelSet::getCellType(const int i, const int j, const int k) const {
	return getCellType(grid->valueAtIndex(i, j, k));
}

CellType LevelSet::getCellType(const double w_x, const double w_y, const double w_z) const {
	return getCellType(this->grid->valueAtWorld(w_x, w_y, w_z));
}

CellType LevelSet::getCellType(const double phi){
	if(false) // Check if solid 
		//if(i < 2 || i >= (this->grid->xdim() - 2) || j< 2 || j >= (this->grid->ydim() - 2) ) //Check if is solid
		//ELLER solids.valueAtIndex(i, j, k) Fanns som sagt 3 olika def.
		return SOLID;
	else if(phi > 0.0)
		return FUEL;
	else
		return BURNT;
}
Vector3 LevelSet::getFlameSpeed(const int i, const int j, const int k, MACGrid *vel) const{
	if(getCellType(i, j, k) == FUEL){
        Vector3 v = getNormal(i, j, k);
		return dsd->getFlameSpeed(i, j, k, vel, v);
	}
	else
		return Vector3(0, 0, 0);
}
void LevelSet::updateDSD(double dt, MACGrid *u){
	dsd->Update_D(dt, u, grid);
}
double LevelSet::getCurvature(const int i, const int j, const int k, GridField<double> *gridfield){
	double 
		dx = FirePresets::centralDisc->calcDx(*gridfield, i, j, k),
		dy = FirePresets::centralDisc->calcDy(*gridfield, i, j, k),
		dz = FirePresets::centralDisc->calcDz(*gridfield, i, j, k),
		d2x = FirePresets::centralDisc->calcD2x(*gridfield, i, j, k),
		d2y = FirePresets::centralDisc->calcD2y(*gridfield, i, j, k),
		d2z = FirePresets::centralDisc->calcD2z(*gridfield, i, j, k),
		dxy = FirePresets::centralDisc->calcDxy(*gridfield, i, j, k),
		dxz = FirePresets::centralDisc->calcDxz(*gridfield, i, j, k),
		dyz = FirePresets::centralDisc->calcDyz(*gridfield, i, j, k);
	double a = 2.0000000000;
	//double val1 = dx*dx*(d2y + d2z) - a*dy*dz*dyz +
		dy*dy*(d2x*d2z) - a*dx*dz*dxz + 
		dz*dz*(d2x + d2y) - a*dx*dy*dxy;
	double val = dx*dx+dy*dy+dz*dz;
	if(val == 0) return 0;

	return (dx*dx*(d2y + d2z) - a*dy*dz*dyz + 
		dy*dy*(d2x*d2z) - a*dx*dz*dxz + 
		dz*dz*(d2x + d2y) - a*dx*dy*dxy) /
		(a*std::pow(dx*dx+dy*dy+dz*dz, 1.5));
}
double LevelSet::getCurvature(const int i, const int j, const int k)
{
	return LevelSet::getCurvature(i, j, k, grid);
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

Vector3 LevelSet::getNormal(const int i, const int j, const int k) const
{
	Vector3 N = normals->valueAtIndex(i,j,k);
	double l = N.norm();
	if(l > 0.0)
		N *= 1.0/l;
	else
		N = Vector3(0.0, 1.0, 0.0);

	return N;
}

Vector3 LevelSet::getNormal(const double w_x, const double w_y, const double w_z) const
{
	Vector3 N = normals->valueAtWorld(w_x, w_y, w_z);
	double l = N.norm();
	if(l > 0.0)
		N *= 1.0/l;
	else
		N = Vector3(0.0, 1.0, 0.0);

	return N;
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

    float dx = (float)grid->dx();
    float dy = (float)grid->dy();
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
