// Johan Nor�n, 2013-02-26
#ifndef VORTICITY_H
#define VORTICITY_H

#include "MACGrid.h"
#include "GridField.h"
#include "Gradient.h"
#include "Divergence.h"

namespace Vorticity{
	// 1.	Skapa ett Gridfield ohm genom nabmla x u (dvs. curl)
	// 2.	Skapa N genom N = (gradienten av normen av ohm)/(gradient av t�ljaren)
	//		N kommer d� peka fr�n l�g-vorticity-omr�den till omr�den med
	//		h�g vorticity.

	Vector3 addVorticity(const MACGrid &u, const double epsilon, const double dx,
		const int dimx, const int dimy, const int dimz){
		
		GridField<double> *ohm = new GridField<double>(dimx, dimy, dimz);
		GridField<Vector3> *angles = new GridField<Vector3>(dimx, dimy, dimz);
		Discretization *d = new CentralDiff();
		Gradient g;
		Vector3 fconf, ohmVec, n, N;

		// Populate GridField ohm with vorticity values
		for(int i=0; i < dimx; ++i){
			for(int j=0; j < dimy; ++j){
				for(int k=0; k< dimz; ++k){
					// Ful-fix f�r boundaries
					if( (i > 1) && (j > 1) /*&& (k > 0)*/ && (i < dimx-2) && (j < dimy-2) /*&& (k < dimz)*/ ){
						Vector3 
						centerVelUp = u.velocityAtCenter(i,j+1,k),
						centerVelDown = u.velocityAtCenter(i,j-1,k),
						centerVelRight = u.velocityAtCenter(i+1,j,k),
						centerVelLeft = u.velocityAtCenter(i-1,j,k),
						centerVelFront = Vector3(0.,0.,0.),//u.velocityAtCenter(i,j,k+1),
						centerVelBack = Vector3(0.,0.,0.),//u.velocityAtCenter(i,j,k-1),
						centerVel = u.velocityAtCenter(i,j,k);
						
						// Fr�n Fedkiw 2001, Smoke etc.
						//ohmVec = Vector3( (centerVelUp.z - centerVelDown.z - centerVelFront.y + centerVelBack.y)/(2*dx),
							//				(centerVelFront.x - centerVelBack.x - centerVelRight.z + centerVelLeft.z)/(2*dx),
								//				(centerVelRight.y - centerVelLeft.y - centerVelUp.x + centerVelDown.x)/(2*dx) );*/

						/*ohmVec.x = (centerVelUp.z - centerVelDown.z - centerVelFront.y + centerVelBack.y)/(2*dx);
						ohmVec.y = (centerVelFront.x - centerVelBack.x - centerVelRight.z + centerVelLeft.z)/(2*dx);
						ohmVec.z = (centerVelRight.y - centerVelLeft.y - centerVelUp.x + centerVelDown.x)/(2*dx);*/

						double x, y, z;
						x = (centerVelUp.z - centerVelDown.z - centerVelFront.y + centerVelBack.y)/(2*dx);
						y = (centerVelFront.x - centerVelBack.x - centerVelRight.z + centerVelLeft.z)/(2*dx);
						z = (centerVelRight.y - centerVelLeft.y - centerVelUp.x + centerVelDown.x)/(2*dx);
						ohmVec = Vector3(x,y,z);

						ohm->setValueAtIndex(ohmVec.norm(),i,j,k);
						angles->setValueAtIndex(ohmVec,i,j,k);
					}
				}
			}
		}

		// Calculate
		for(int i=0; i < dimx; ++i){
			for(int j=0; j < dimy; ++j){
				for(int k=0; k< dimz; ++k){
					// Ful-fix f�r boundaries
					if( (i > 1) && (j > 1) /*&& (k > 0)*/ && (i < dimx-2) && (j < dimy-2) /*&& (k < dimz)*/ ){
						n = g.getGradient(*ohm, i, j, k, *d); // F�r denna kr�vs att hela ohm �r def.

						if(n.norm() != 0.)
							N = n/n.norm();
						else
							N = Vector3();

						// Enl. def. |u x v| = |u|*|v|*sin(theta), d�r theta = vinkeln mellan u och v
						Vector3 ang = angles->valueAtIndex(i,j,k);
						double sineAngle = sin(N.angle(&ang));
						
						if((sineAngle == sineAngle)) // Om sineAngle �r NaN
							fconf += epsilon*dx*( N.norm() * ohm->valueAtIndex(i,j,k) * sineAngle); // f�r angle kr�vs vektorn ohmVec
						
						/*if(!(fconf == Vector3(0.,0.,0.)))
							std::cout << "Buuuuu!\n";*/
					}
				}
			}
		}
		delete angles;
		delete ohm;
		delete d;
		return fconf;
	}
}

#endif