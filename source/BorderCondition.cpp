// Johan Noren
// Klass-metoder for att kontrollera samt sakerstalla att fluiden
// ej har hastighetsvektorer som pekar ut ur grid:en.

#include "BorderCondition.h"

// Returnerar true om ingen boundary-åtgärd behövs
bool BorderCondition::checkBorder(GridField<double> &g, const int i, const int j, const int k) const{
	return !(i == 0  || j == 0 || i == g.xdim()-1 || j == g.ydim()-1 || k == 0 || k == g.zdim()-1);
}

// Denna skall endast anropas om checkBorder(..) returnerat FALSE
void BorderCondition::enforceBorderCondition(MACGrid &vel, GridField<double> &g, const int i, const int j, const int k) const{
    Vector3 p = Vector3(i,j,k);
	Vector3 v = vel.velocityAtCenter(p);
    /*
	if(i == 0 && v.x < 0){
		v.x = 0;
		vel.setCenterVel(v,i,j,k);
	}
	if(j == 0 && v.y < 0){
		v.y = 0;
		vel.setCenterVel(v,i,j,k);
	}
	if(k == 0 && v.z < 0){
		v.z = 0;
		vel.setCenterVel(v,i,j,k);
	}
	if(i == g.xdim()-1 && v.x > 0){
		v.x = 0;
		vel.setCenterVel(v,i,j,k);
	}
	if(j == g.ydim()-1 && v.y > 0){
		v.y = 0;
		vel.setCenterVel(v,i,j,k);
	}
	if(k == g.zdim()-1 && v.z > 0){
		v.z = 0;
		vel.setCenterVel(v,i,j,k);
	}*/
	return;
}