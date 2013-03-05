#include "Temperature.h"
#include "firePresets.h"
#include "MACAdvect.h"
#include "fire.h"
#if defined __APPLE__ || defined __unix__
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
	beyonce = new GridField<Vector3>(XDIM, YDIM, ZDIM, FirePresets::GRID_SIZE);

	/*double scale = (double)XDIM / (double)YDIM;
	grid->multTransformation(glm::scale(scale, 1.0, 1.0));
	gridCopy->multTransformation(glm::scale(scale, 1.0, 1.0));
	beyonce->multTransformation(glm::scale(scale, 1.0, 1.0));
	*/
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
    if(type == FUEL){
        grid->setValueAtIndex(FirePresets::T_MAX, i, j, k);
    }
}

void Temperature::InitCell(int i, int j, int k, CellType type)
{
	if(type == FUEL){
        grid->setValueAtIndex(FirePresets::T_MAX, i, j, k);
	}
	else if(type == BURNT){
		grid->setValueAtIndex(FirePresets::T_AIR, i, j, k);
	}
    gridCopy->setValueAtIndex(grid->valueAtIndex(i, j, k), i, j, k);
}

double Temperature::calculateTemperatureLoss(int i, int j, int k){
    double c_T = .001;

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

void Temperature::CalculateBuoyancyForceField()
{
	double alpha = 0.007;

	int xdim = FirePresets::GRID_DIM_X,
		ydim = FirePresets::GRID_DIM_Y,
		zdim = FirePresets::GRID_DIM_Z;

	for(int i = 0; i < xdim; i++)
		for(int j = 0; j < ydim; j++)
			for(int k = 0; k < zdim; k++){
				Vector3 force = Vector3(0.0, 1.0, 0.0);
				double amplitude = (grid->valueAtIndex(i,j,k) - FirePresets::T_AIR) * alpha;

				force *= amplitude;
				beyonce->setValueAtIndex(force, i, j, k);
	}

}

void Temperature::drawBuoyancyForce(){
    for (GridMappingIterator iter = grid->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        grid->indexToWorld(i, j, k, x, y, z);
    
		Vector3 v = beyonce->valueAtIndex(i,j,k)/100;
        glColor3d(1.0,1.0,0.0);
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, 0);
        glVertex3d(x + v.x, y +v.y , 0);
        glEnd();
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
        

        glColor3d((grid->valueAtIndex(i, j, k) - FirePresets::T_AIR)/100.0, 0, 0);


        glVertex3f((float)x, (float)y, (float)z);
        glVertex3f(((float)x+1.f), (float)y, (float)z);
        glVertex3f(((float)x+1.f), ((float)y+1.f), (float)z);
        glVertex3f((float)x, ((float)y+1.f), (float)z);
        
    }
	glEnd();

	//drawBuoyancyForce();
}
