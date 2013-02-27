// Johan Norén, 2013-02-26
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

	// Krafterna som beräknas är per cell, så krafterna måste lagras i ett
	// GridField<Vector3> som sedan appliceras på hastighets-fältet
	// antingen här eller i någon annan central kraft-metod.

	Vector3 addVorticity(const MACGrid &u, GridField<Vector3> &forces, const double epsilon, const double dx,
		const int dimx, const int dimy, const int dimz){
		
		GridField<double> ohmNorm = GridField<double>(dimx, dimy, dimz);
		GridField<Vector3> ohmVecGrid = GridField<Vector3>(dimx, dimy, dimz);
		
		Discretization *d = new CentralDiff();
		Gradient g;
		Vector3 fconf, ohmVecTemp, n, N;

		// Populate GridField ohm with vorticity values
		for(int i=0; i < dimx; ++i){
			for(int j=0; j < dimy; ++j){
				for(int k=0; k< dimz; ++k){
					// Ful-fix för boundaries
					if( (i > 1) && (j > 1) /*&& (k > 0)*/ && (i < dimx-2) && (j < dimy-2) /*&& (k < dimz)*/ ){
						Vector3 
						centerVelUp = u.velocityAtCenter(i,j+1,k),
						centerVelDown = u.velocityAtCenter(i,j-1,k),
						centerVelRight = u.velocityAtCenter(i+1,j,k),
						centerVelLeft = u.velocityAtCenter(i-1,j,k),
						centerVelFront = Vector3(0.,0.,0.),//u.velocityAtCenter(i,j,k+1),
						centerVelBack = Vector3(0.,0.,0.),//u.velocityAtCenter(i,j,k-1),
						centerVel = u.velocityAtCenter(i,j,k);
						
						// Från Fedkiw 2001, Smoke etc. ekv (9) s.3 samt diskretisering enl. s.6
						// ohm = nabla x û
						ohmVecTemp = Vector3(	(centerVelUp.z - centerVelDown.z - centerVelFront.y + centerVelBack.y)/(2*dx),
												(centerVelFront.x - centerVelBack.x - centerVelRight.z + centerVelLeft.z)/(2*dx),
												(centerVelRight.y - centerVelLeft.y - centerVelUp.x + centerVelDown.x)/(2*dx) );			

						ohmNorm.setValueAtIndex(ohmVecTemp.norm(),i,j,k);
						ohmVecGrid.setValueAtIndex(ohmVecTemp,i,j,k);
					}
				}
			}
		}

		// Calculate n, N and fcont
		for(int i=0; i < dimx; ++i){
			for(int j=0; j < dimy; ++j){
				for(int k=0; k< dimz; ++k){
					// Ful-fix för boundaries
					if( (i > 1) && (j > 1) /*&& (k > 0)*/ && (i < dimx-2) && (j < dimy-2) /*&& (k < dimz)*/ ){
						
						n = g.getGradient(ohmNorm, i, j, k, *d); // För denna krävs att hela ohm är def.

						(n.norm() != 0.) ? N = n/n.norm() : N = Vector3(0.);

						Vector3 ohmVecTemp;
						double x, y, z;

						ohmVecTemp = ohmVecGrid.valueAtIndex(i,j,k);

						Vector3 crossprod(	N.z * ohmVecTemp.y - N.y * ohmVecTemp.z,
											N.z * ohmVecTemp.x - N.x * ohmVecTemp.z,
											N.x * ohmVecTemp.y - N.y * ohmVecTemp.x);

						fconf = crossprod*dx*epsilon;

						forces.setValueAtIndex(fconf,i,j,k);
					}
				}
			}
		}
		delete d;
		return fconf;
	}
}

#endif