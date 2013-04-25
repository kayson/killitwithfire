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
	for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
		for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
			for(int k = 0; k < FirePresets::GRID_DIM_Z; k++){
				flameFrontVelocity->setValueAtIndex(0, i, j, k);
				dDeriv->setValueAtIndex(0, i, j, k);
				curvature->setValueAtIndex(0, i, j, k);
				nextPhi->setValueAtIndex(0, i, j, k);
			}
		}
	}
}
void DetonationShockDynamics::Update_D_With_DSD(double dt, GridField<double> *phi, MACGrid *velocity){
	double DCJ = 1,
		c1 = 1,
		c2 = 1,
		c3 = 1,
		c4 = 1,
		c5 = 1,
		mu = 1;
	for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
		for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
			for(int k = 0; k < FirePresets::GRID_DIM_Z; k++){
				curvature->setValueAtIndex(LevelSet::getCurvature(i, j, k, phi), i, j, k);
				nextPhi->setValueAtIndex(phi->valueAtIndex(i, j, k), i, j, k);
			}
		}
	}
	for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
		for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
			for(int k = 0; k < FirePresets::GRID_DIM_Z; k++){
				double vphi = FirePresets::tempAdvect->advect(dt, *velocity, *nextPhi, i, j, k);
				nextPhi->setValueAtIndex(nextPhi->valueAtIndex(i, j, k) - vphi * dt, i, j, k);

				double v = FirePresets::tempAdvect->advect(dt, *velocity, *flameFrontVelocity, i, j, k);
				flameFrontVelocity->setValueAtIndex(flameFrontVelocity->valueAtIndex(i, j, k) - v * dt, i, j, k);

				double dv = FirePresets::tempAdvect->advect(dt, *velocity, *dDeriv, i, j, k);
				dDeriv->setValueAtIndex(dDeriv->valueAtIndex(i, j, k) - dv * dt, i, j, k);

				double c = FirePresets::tempAdvect->advect(dt, *velocity, *curvature, i, j, k);
				curvature->setValueAtIndex(curvature->valueAtIndex(i, j, k) - c * dt, i, j, k);
			}
		}
	}
	for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
		for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
			for(int k = 0; k < FirePresets::GRID_DIM_Z; k++){
				double curvNext = LevelSet::getCurvature(i, j, k, nextPhi);
				double curvDeriv = (curvNext - curvature->valueAtIndex(i, j, k)) / dt;
				double delta = dDeriv->valueAtIndex(i, j, k) - DCJ;
				double alpha = exp(mu * delta);
				double Lcj = log(1 + c5 * curvNext / alpha);
				double ddD = -c1 * alpha * alpha * delta 
					- c2 * alpha * dDeriv->valueAtIndex(i, j, k)
					- c3 * alpha * alpha * Lcj - c4 * curvDeriv;
				dDeriv->addValueAtIndex(ddD * dt, i, j , k);
				flameFrontVelocity->addValueAtIndex(dDeriv->valueAtIndex(i, j, k) * dt, i, j, k);
			}
		}
	}
}
void DetonationShockDynamics::Update_D_Without_DSD(GridField<double> *phi){
	double a = 0.1;
	double b = 0.2;
	for(int i = 0; i < FirePresets::GRID_DIM_X; i++){
		for(int j = 0; j < FirePresets::GRID_DIM_Y; j++){
			for(int k = 0; k < FirePresets::GRID_DIM_Z; k++){
				//Calculate mean curvature
				double curvature = LevelSet::getCurvature(i, j, k, phi);

				flameFrontVelocity->setValueAtIndex(a - b * curvature, i, j, k);
			}
		}
	}
}
void DetonationShockDynamics::Update_D(double dt, MACGrid *velocity, GridField<double> *phi){
	//Update_D_With_DSD(dt, phi, velocity);
	Update_D_Without_DSD(phi);
}

Vector3 DetonationShockDynamics::getFlameSpeed(int i, int j, int k, MACGrid *velocity, Vector3 &normal) const{
	/*return (Vector3::dot(velocity->velocityAtCenter(i, j, k) - flameFrontVelocity->valueAtIndex(i, j, k), 
		normal));*/
	normal.normalize();
	double speed = (Vector3::dot(velocity->velocityAtCenter(i, j, k), normal) * normal).norm() - flameFrontVelocity->valueAtIndex(i, j, k);
	Vector3 a = normal * speed;
	Vector3 b = velocity->velocityAtCenter(i, j, k) + normal * FirePresets::S;
	return b;
}

double DetonationShockDynamics::getFlameFrontVelocity(int i, int j, int k) const{
	return flameFrontVelocity->valueAtIndex(i, j, k);
}
