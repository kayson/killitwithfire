#include "Temperature.h"
#include "firePresets.h"
#include "MACAdvect.h"
#include "GridField.hpp"
#include "helper.h"
#include "MACGrid.h"
#include "LevelSet.h"
#include "fire.h"
#include "Vector3.h"
#include "BlackBodyRadiation.h"
#include "ClosestValueExtrapolation.h"
#include "Gradient.h"

#if defined __APPLE__ || defined __unix__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#endif

#include <iostream>

Temperature::Temperature(GridField<double> *phi)
{

	int XDIM = FirePresets::GRID_DIM_X*FirePresets::TEMPERATUR_MULT,
		YDIM = FirePresets::GRID_DIM_Y*FirePresets::TEMPERATUR_MULT,
		ZDIM = FirePresets::GRID_DIM_Z*FirePresets::TEMPERATUR_MULT;

	grid = new GridField<double>(XDIM, YDIM, ZDIM, FirePresets::GRID_SIZE, new ConstantValueExtrapolation<double>(FirePresets::T_AIR)); //TODO KORREKT EXTRAPOLERING?
    copy = new GridField<double>(XDIM, YDIM, ZDIM, FirePresets::GRID_SIZE, new ConstantValueExtrapolation<double>(FirePresets::T_AIR)); //TODO KORREKT EXTRAPOLERING?

    beyonce = new GridField<Vector3>(XDIM, YDIM, ZDIM, FirePresets::GRID_SIZE, new ClosestValueExtrapolation<Vector3>()); //TODO KORREKT EXTRAPOLERING?

	/*double scale = (double)XDIM / (double)YDIM;
	grid->multTransformation(glm::scale(scale, 1.0, 1.0));
	gridCopy->multTransformation(glm::scale(scale, 1.0, 1.0));
	beyonce->multTransformation(glm::scale(scale, 1.0, 1.0));
	*/
	for(int i = 0; i < XDIM; i++){
		for(int j = 0; j < YDIM; j++){
			for(int k = 0; k < ZDIM; k++){
				double x,y,z;
                grid->indexToWorld(i, j, k, x, y, z);
				InitCell(i,j,k, LevelSet::getCellType(phi->valueAtWorld(x, y, z)));
			}
		}
	}
}

void Temperature::ResetCell(int i, int j, int k, CellType type)
{
    double x, y, z;
    grid->indexToWorld(i, j, k, x, y, z);
    if(type == FUEL){
        grid->setValueAtIndex(FirePresets::T_IGNITION, i, j, k);
    }
}

void Temperature::InitCell(int i, int j, int k, CellType type)
{
	if(type == FUEL){
          grid->setValueAtIndex(FirePresets::T_IGNITION, i, j, k);
	}
	else if(type == BURNT){
		grid->setValueAtIndex(FirePresets::T_AIR, i, j, k);
	}
}

double Temperature::calculateTemperatureLoss(double T) const{
	return pow((T-FirePresets::T_AIR)/
                   (FirePresets::T_MAX - FirePresets::T_AIR), 4.0) *
            FirePresets::TEMPERATURE_LOSS_CONSTANT;
}

void Temperature::AdvectTemperatureField(double dt, const MACGrid &m, const LevelSet &ls){

	for(int i = 0; i < grid->xdim(); i++)
        for(int j = 0; j < grid->ydim(); j++)
            for(int k = 0; k < grid->zdim(); k++)
			{
				double x,y,z;
                grid->indexToWorld(i, j, k, x, y, z);
				ResetCell(i, j, k, ls.getCellType(x, y, z));
			}
    

    for(int i = 0; i < grid->xdim(); i++)
        for(int j = 0; j < grid->ydim(); j++)
            for(int k = 0; k < grid->zdim(); k++){
                double x,y,z;
                grid->indexToWorld(i, j, k, x, y, z);
                Vector3 vel = m.velocityAtWorld(Vector3(x,y,z));
                double val = grid->valueAtWorld(x-dt*vel.x, y-dt*vel.y, z-dt*vel.z);//TODO bättre integeringsmetod så man får med jump condition
				val -= calculateTemperatureLoss(val)*FirePresets::dt;

				if(val < FirePresets::T_AIR)//TODO KONTROLLERA VARFÖR DETTA HÄNDER ISTÄLLET
				{
					//std::cout << "Temperature is below air temperature!" << std::endl;
					val = FirePresets::T_AIR;
				}

                copy->setValueAtIndex(val, i, j, k);
    }
    
    /*
    for(int i = 0; i < grid->xdim(); i++)
        for(int j = 0; j < grid->ydim(); j++)
            for(int k = 0; k < grid->zdim(); k++){

				Vector3 grad = Gradient::getGradient(*grid, i, j, k, *FirePresets::centralDisc);
				Vector3 vel = m.velocityAtCenter(i, j, k);
				double v = -Vector3::dot(vel, grad);

                double c = calculateTemperatureLoss(i, j, k);

				double newValue = (grid->valueAtIndex(i,j,k) + v * dt) - c * dt;
				if(newValue < FirePresets::T_AIR*0.8)
					newValue = FirePresets::T_AIR*0.8;
				copy.setValueAtIndex(newValue, i, j, k);
            }
    */

    std::swap(grid, copy);
}

void Temperature::CalculateBuoyancyForceField(LevelSet &ls)
{
	for(int i = 0; i < grid->xdim(); i++)
      for(int j = 0; j < grid->ydim(); j++)
          for(int k = 0; k < grid->zdim(); k++){
			  double x,y,z;
              grid->indexToWorld(i, j, k, x, y, z);
			  if(!(ls.getCellType(x, y, z) == FUEL)) {
				  Vector3 force = Vector3(0.0, 1.0, 0.0);
				  double amplitude = (grid->valueAtIndex(i,j,k) -
									  FirePresets::T_AIR)
									 * FirePresets::TEMPERATURE_BUOYANCY_ALPHA;
        
				  force *= amplitude;
				  beyonce->setValueAtIndex(force, i, j, k);
			  }
			  else
				  beyonce->setValueAtIndex(Vector3(0), i, j, k);
          }
}

void Temperature::drawBuoyancyForce(){
    for (GridMappingIterator iter = grid->iterator();
         !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        grid->indexToWorld(i, j, k, x, y, z);
    
        Vector3 v = beyonce->valueAtIndex(i,j,k)/500;
        glColor3d(1.0,1.0,0.0);
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, 0);
        glVertex3d(x + v.x, y +v.y , 0);
        glEnd();
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glVertex3f(x, y, 0);
		glEnd();
    }
}

GridField<double> Temperature::GetTemperatureGrid(){
    return *grid;
}

double Temperature::maxTemp()
{
	int xdim = FirePresets::GRID_DIM_X*FirePresets::TEMPERATUR_MULT,
		ydim = FirePresets::GRID_DIM_Y*FirePresets::TEMPERATUR_MULT,
		zdim = FirePresets::GRID_DIM_Z*FirePresets::TEMPERATUR_MULT;
  
	double max = 0;

	for(int i = 0; i < xdim; i++)
		for(int j = 0; j < ydim; j++)
			for(int k = 0; k < zdim; k++){
				if(grid->valueAtIndex(i,j,k) > max)
					max = grid->valueAtIndex(i,j,k);
			}

	return max;
}

void Temperature::draw()
{
	double maxT = maxTemp();
		glBegin(GL_QUADS);

    float dx = (float)grid->dx();
    float dy = (float)grid->dy();
    for (GridFieldIterator<double> iter = grid->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        grid->indexToWorld(i, j, k, x, y, z);
		double v = (grid->valueAtIndex(i,j,k) - FirePresets::T_AIR)/(FirePresets::T_MAX - FirePresets::T_AIR);
        glColor3d(0.7*v,0.7*v,0.0);

        glVertex3f((float)x, (float)y, (float)z);
        glVertex3f(((float)x+1.f), (float)y, (float)z);
        glVertex3f(((float)x+1.f), ((float)y+1.f), (float)z);
        glVertex3f((float)x, ((float)y+1.f), (float)z);   
    }
    glEnd();

	//Vector3 LMSw = BlackBodyRadiation::XYZtoLMS(BlackBodyRadiation::blackbodyToXYZ(maxT));

 //   glBegin(GL_QUADS);
 //   for (GridFieldIterator<double> iter = grid->iterator();
 //        !iter.done(); iter.next()) {
 //       int i,j,k;
 //       iter.index(i, j, k);
 //       double x,y,z;
 //       grid->indexToWorld(i, j, k, x, y, z);
 //       

	//	// Eq. 23, Nguyen'02
	//	Vector3 xyz = BlackBodyRadiation::blackbodyToXYZ(grid->valueAtIndex(i,j,k));
	//	Vector3 lms = BlackBodyRadiation::XYZtoLMS(xyz);
	//	lms.x /= LMSw.x;
	//	lms.y /= LMSw.y;
	//	lms.z /= LMSw.z;
	//	xyz = BlackBodyRadiation::LMStoXYZ(lms);
	//	Vector3 rgb = BlackBodyRadiation::XYZtoRGB(xyz);

	//	if(grid->valueAtIndex(i,j,k) >= maxT*0.6)
	//		glColor3d(rgb.x, rgb.y, rgb.z);
	//	else
	//		glColor3d(rgb.x*0.3, rgb.y*0.3, rgb.z*0.3);

 //       
 //       glVertex3f((float)x, (float)y, (float)z);
 //       glVertex3f(((float)x+1.f), (float)y, (float)z);
 //       glVertex3f(((float)x+1.f), ((float)y+1.f), (float)z);
 //       glVertex3f((float)x, ((float)y+1.f), (float)z);
 //       
 //   }
 //   glEnd();

    //drawBuoyancyForce();
}
