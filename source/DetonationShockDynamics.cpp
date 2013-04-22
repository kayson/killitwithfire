#include "DetonationShockDynamics.h"
#include "firePresets.h"
#include "GridField.h"
#include "LevelSet.h"
#include "MACGrid.h"

DetonationShockDynamics::DetonationShockDynamics(){
	flameFrontVelocity = new GridField<double>(FirePresets::GRID_DIM_X, FirePresets::GRID_DIM_Y, FirePresets::GRID_DIM_Z);
	dDeriv = new GridField<double>(FirePresets::GRID_DIM_X, FirePresets::GRID_DIM_Y, FirePresets::GRID_DIM_Z);
}
void DetonationShockDynamics::Update_D_With_DSD(LevelSet *ls){
	
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

double DetonationShockDynamics::getFlameSpeed(int i, int j, int k, MACGrid *velocity) const{
	return velocity->velocityAtCenter(i, j, k) - flameFrontVelocity->valueAtIndex(i, j, k);
	//velocity->
}

double DetonationShockDynamics::getFlameFrontVelocity(int i, int j, int k) const{
  return flameFrontVelocity->valueAtIndex(i, j, k);
}
