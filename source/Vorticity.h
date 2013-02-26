#ifndef VORTICITY_H
#define VORTICITY_H

#include "MACGrid.h"
#include "GridField.h"
#include "Gradient.h"
#include "Divergence.h"

namespace Vorticity{
	// 1.	Skapa ett Gridfield ohm genom nabmla x u (dvs. curl)
	// 2.	Skapa N genom N = (gradienten av normen av ohm)/(gradient av täljaren)
	//		N kommer då peka från låg-vorticity-områden till områden med
	//		hög vorticity.

	Vector3 addVorticity(const MACGrid &u, const double epsilon, const int dx,
		const int dimx, const int dimy, const int dimz){
		
		GridField<double> *ohm = new GridField<double>(dimx, dimy, dimz);
		Discretization *d = new CentralDiff();
		Gradient g;
		Vector3 fconf;

		for(int i=0; i < dimx; ++i){
			for(int j=0; j < dimy; ++j){
				for(int k=0; k< dimz; ++k){
					Vector3 
						centerVelUp = u.velocityAtCenter(i,j+1,k),
						centerVelDown = u.velocityAtCenter(i,j-1,k),
						centerVelRight = u.velocityAtCenter(i+1,j,k),
						centerVelLeft = u.velocityAtCenter(i-1,j,k),
						centerVelFront = u.velocityAtCenter(i,j,k+1),
						centerVelBack = u.velocityAtCenter(i,j,k-1),
						centerVel = u.velocityAtCenter(i,j,k),
						ohmVec,
						n,
						N;

					// Ful-fix för boundaries
					if(i > 0 && j > 0 && k > 0 && i < dimx-1 && j < dimy-1 && k < dimz-1){
						// Från Fedkiw 2001, Smoke etc.
						ohmVec.x = (centerVelUp.z - centerVelDown.z - centerVelFront.y + centerVelBack.y)/(2*dx);
						ohmVec.y = (centerVelFront.x - centerVelDown.x - centerVelRight.z + centerVelLeft.z)/(2*dx);
						ohmVec.z = (centerVelRight.y - centerVelLeft.y - centerVelUp.x + centerVelDown.x)/(2*dx);

						//ohmVec.normalize();

						ohm->setValueAtIndex(ohmVec.norm(),i,j,k);

						n = g.getGradient(*ohm, i, j, k, *d);
						N = n/n.norm();

						// Enl. def. |u x v| = |u|*|v|*sin(theta), theta = vinkel mellan u och v
						fconf = epsilon*dx*( N.norm() * ohmVec.norm() * sin(N.angle(&ohmVec)) );
					}
				}
			}
		}
		delete ohm;
		delete d;
		return fconf;
	}
}

#endif