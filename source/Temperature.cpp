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
	gridCopy = new GridField<double>(XDIM, YDIM, ZDIM, FirePresets::GRID_SIZE);

	
	for(int i = 0; i < XDIM; i++){
		for(int j = 0; j < YDIM; j++){
			for(int k = 0; k < ZDIM; k++){
				InitCell(i,j,k, Fire::getCellType(phi->valueAtIndex(i, j, k)));
			}
		}
	}
}

void Temperature::ResetCell(int i, int j, int k, CellType type)
{
    double x, y, z;
    grid->indexToWorld(i, j, k, x, y, z);
    if(type == BLUECORE){
        grid->setValueAtIndex(FirePresets::T_MAX, i, j, k);
    }
}

void Temperature::InitCell(int i, int j, int k, CellType type)
{
	if(type == BLUECORE){
        grid->setValueAtIndex(FirePresets::T_MAX, i, j, k);
	}
	else if(type == IGNITED){
		grid->setValueAtIndex(FirePresets::T_AIR, i, j, k);
	}
    gridCopy->setValueAtIndex(grid->valueAtIndex(i, j, k), i, j, k);
}

double Temperature::calculateTemperatureLoss(int i, int j, int k){
    double c_T = .7;

    double T = grid->valueAtIndex(i, j, k);


    return pow((T-FirePresets::T_AIR)/(FirePresets::T_MAX - FirePresets::T_AIR), 4.0) * c_T;
}

void Temperature::AdvectTemperatureField(double dt, MACGrid m, LevelSet ls){
    for(int i = 0; i < grid->xdim(); i++)
        for(int j = 0; j < grid->ydim(); j++)
            for(int k = 0; k < grid->zdim(); k++){
                ResetCell(i, j, k, Fire::getCellType(ls.grid->valueAtIndex(i, j, k)));
                double c = calculateTemperatureLoss(i, j, k);
                double v = FirePresets::tempAdvect->advect(dt, m, *grid, i, j, k);
                grid->setValueAtIndex(v - c, i, j, k);
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
        

        glColor3d((grid->valueAtIndex(i, j, k) - FirePresets::T_AIR)/10.0, 0, 0);


        glVertex3f((float)x, (float)y, (float)z);
        glVertex3f(((float)x+1.f), (float)y, (float)z);
        glVertex3f(((float)x+1.f), ((float)y+1.f), (float)z);
        glVertex3f((float)x, ((float)y+1.f), (float)z);
        
    }
	glEnd();
}