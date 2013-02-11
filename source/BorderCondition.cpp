#include "BorderCondition.h"

// Returnerar true om ingen boundary-åtgärd behövs
bool BorderCondition::checkBorder(Grid &g, const int i, const int j, const int k)
{
	//std::cout << "checkBorder(...)\n";
	return !(i == 0  || j == 0 || i == g.getDimX()-1 || j == g.getDimY()-1 || k == 0 || k == g.getDimZ()-1);
}

// Denna skall endast anropas om checkBorder(..) returnerat FALSE
void BorderCondition::enforceBorderCondition(VelocityField &vel, Grid &g, const int i, const int j, const int k)
{
	//std::cout << "enforceBoundaryCondition(...), (i,j,k) = ("<<i<<", "<<j<<", "<<k<<")\n";
	Vector3 v = vel.getVelocityAtCoordinate(Vector3(i,j,k));
	if(i == 0 && v.x < 0){
		//std::cout << "1\n";
		v.x = 0;
		vel.setCenterVel(v,i,j,k);
		//vel.getGridU()->setData(i,j,k,0);
	}
	if(j == 0 && v.y < 0){
		//std::cout << "2\n";
		v.y = 0;
		vel.setCenterVel(v,i,j,k);
		//vel.getGridV()->setData(i,j,k,0);
	}
	if(k == 0 && v.z < 0){
		//std::cout << "3\n";
		v.z = 0;
		vel.setCenterVel(v,i,j,k);
		//vel.getGridW()->setData(i,j,k,0);
	}
	if(i == g.getDimX()-1 && v.x > 0){
		//std::cout << "4\n";
		v.x = 0;
		vel.setCenterVel(v,i,j,k);
		//vel.getGridU()->setData(i,j,k,0);
	}
	if(j == g.getDimY()-1 && v.y > 0){
		//std::cout << "5\n";
		v.y = 0;
		vel.setCenterVel(v,i,j,k);
		//vel.getGridV()->setData(i,j,k,0);
	}
	if(k == g.getDimZ()-1 && v.z > 0){
		//std::cout << "6\n";
		v.z = 0;
		vel.setCenterVel(v,i,j,k);
		//vel.getGridW()->setData(i,j,k,0);
	}
	//std::cout << "enforceBoundaryCondition(...), DONE!\n";
	return;
}