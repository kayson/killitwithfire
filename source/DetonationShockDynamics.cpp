#include "DetonationShockDynamics.h"
#include "firePresets.h"
#include "GridField.h"
#include "LevelSet.h"
#include "MACGrid.h"
#include "MACAdvect.h"
#include "Vector3.h"
#include "helper.h"

DetonationShockDynamics::DetonationShockDynamics(){
	flameFrontVelocity = new GridField<double>(FirePresets::GRID_DIM_X, FirePresets::GRID_DIM_Y, FirePresets::GRID_DIM_Z);
	dDeriv = new GridField<double>(FirePresets::GRID_DIM_X, FirePresets::GRID_DIM_Y, FirePresets::GRID_DIM_Z);
	curvature = new GridField<double>(FirePresets::GRID_DIM_X, FirePresets::GRID_DIM_Y, FirePresets::GRID_DIM_Z);
	nextPhi = new GridField<double>(FirePresets::GRID_DIM_X, FirePresets::GRID_DIM_Y, FirePresets::GRID_DIM_Z);
}
void DetonationShockDynamics::Update_D_With_DSD(double dt, LevelSet *ls, MACGrid *velocity){
	for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
		for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
			for(int k = 0; k < FirePresets::GRID_DIM_Y; k++){
				curvature->setValueAtIndex(ls->getCurvature(i, j, k), i, j, k);
			}
		}
	}
	for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
		for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
			for(int k = 0; k < FirePresets::GRID_DIM_Y; k++){
				double vphi = FirePresets::tempAdvect->advect(dt, *velocity, *flameFrontVelocity, i, j, k);
				double v = FirePresets::tempAdvect->advect(dt, *velocity, *flameFrontVelocity, i, j, k);
				double dv = FirePresets::tempAdvect->advect(dt, *velocity, *dDeriv, i, j, k);
				double c = FirePresets::tempAdvect->advect(dt, *velocity, *curvature, i, j, k);
				nextPhi->setValueAtIndex(nextPhi->valueAtIndex(i, j, k) - vphi * dt, i, j, k);
				flameFrontVelocity->setValueAtIndex(flameFrontVelocity->valueAtIndex(i, j, k) - v * dt, i, j, k);
				dDeriv->setValueAtIndex(dDeriv->valueAtIndex(i, j, k) - dv * dt, i, j, k);
				curvature->setValueAtIndex(curvature->valueAtIndex(i, j, k) - c * dt, i, j, k);
			}
		}
	}
	for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
		for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
			for(int k = 0; k < FirePresets::GRID_DIM_Y; k++){
				//curvDeriv = (
			}
		}
	}
}
void DetonationShockDynamics::Update_D_Without_DSD(LevelSet *ls){
  double a = 1;
  double b = 1;
  for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
    for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
      for(int k = 0; k < FirePresets::GRID_DIM_Y; k++){
        //Calculate mean curvature
		  double curvature = ls->getCurvature(i, j, k);
        
		flameFrontVelocity->setValueAtIndex(a - b * curvature, i, j, k);
      }
    }
  }
}
void DetonationShockDynamics::Update_D(LevelSet *ls){
	Update_D_Without_DSD(ls);
}

double DetonationShockDynamics::getFlameSpeed(int i, int j, int k, MACGrid *velocity, LevelSet *ls) const{
	return (Vector3::dot(velocity->velocityAtCenter(i, j, k), 
		ls->getNormal(i, j, k)) - flameFrontVelocity->valueAtIndex(i, j, k));
}

double DetonationShockDynamics::getFlameFrontVelocity(int i, int j, int k) const{
  return flameFrontVelocity->valueAtIndex(i, j, k);
}
