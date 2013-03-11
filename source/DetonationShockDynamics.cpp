#include "DetonationShockDynamics.h"
#include "firePresets.h"
#include "GridField.h"

DetonationShockDynamics::DetonationShockDynamics(){
  flameFrontVelocity = new GridField<double>(1.0, 1.0, 1.0);
}
void DetonationShockDynamics::Update_D_With_DSD(){
  
}
void DetonationShockDynamics::Update_D_Without_DSD(){
  double a = 1;
  double b = 1;
  for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
    for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
      for(int k = 0; k < FirePresets::GRID_DIM_Y; k++){
        //Calculate mean curvature
        double curvature = 10.0;
        
        flameFrontVelocity->setValueAtIndex(a - b * curvature, i, j, k);
      }
    }
  }
}
void DetonationShockDynamics::Update_D(){
  Update_D_Without_DSD();
}

double DetonationShockDynamics::getFlameSpeed(int i, int j, int k) const{
  return 0;
}

double DetonationShockDynamics::getFlameFrontVelocity(int i, int j, int k) const{
  return flameFrontVelocity->valueAtIndex(i, j, k);
}
