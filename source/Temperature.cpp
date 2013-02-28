#include "Temperature.h"
#include "FirePresets.h"
#include "MACAdvect.h"
#include "fire.h"
#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#endif

Temperature::Temperature(GridField<double> *phi)
{

	int XDIM = FirePresets::GRID_DIM_X,
		YDIM = FirePresets::GRID_DIM_Y,
		ZDIM = FirePresets::GRID_DIM_Z;

	grid = new GridField<double>(XDIM, YDIM, ZDIM, FirePresets::GRID_SIZE);
	
	for(int i = 0; i < XDIM; i++){
		for(int j = 0; j < YDIM; j++){
			for(int k = 0; k < ZDIM; k++){
				InitCell(i,j,k, Fire::getCellType(phi->valueAtIndex(i, j, k)));
			}
		}
	}
}

void Temperature::InitCell(int i, int j, int k, CellType type)
{
	if(type == CellType::BLUECORE){
		grid->setValueAtIndex(FirePresets::T_IGNITION, i, j, k);
	}
	else if(type == CellType::IGNITED){
		grid->setValueAtIndex(FirePresets::T_AIR, i, j, k);
	}
}


void Temperature::SetToMax(int i, int j, int k){
	grid->setValueAtIndex(FirePresets::T_MAX, i, j, k);
}

void Temperature::SetToIgnite(int i, int j, int k)
{
	grid->setValueAtIndex(FirePresets::T_IGNITION, i, j, k);
}

GridField<double> Temperature::GetTemperatureGrid(){
	return *grid;
}

void Temperature::draw(){
	glBegin(GL_QUADS);
	for (GridFieldIterator<double> iter = grid->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        grid->indexToWorld(i, j, k, x, y, z);
        
		if(grid->valueAtIndex(i,j,k) > FirePresets::T_AIR){
			glColor3d(grid->valueAtIndex(i, j, k)/FirePresets::T_MAX, 0, 0);
        }
		else{
            glColor3d(0,0,0);
            
		}

        glVertex3f((float)x, (float)y, (float)z);
        glVertex3f(((float)x+1.f), (float)y, (float)z);
        glVertex3f(((float)x+1.f), ((float)y+1.f), (float)z);
        glVertex3f((float)x, ((float)y+1.f), (float)z);
        
    }
	glEnd();
}