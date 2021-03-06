// Johan Nor�n, 2013-02-26

#include "Vorticity.h"
#include "MACGrid.h"

#include "GridField.hpp"

#include "Gradient.h"
#include "Divergence.h"
#include "CentralDiff.h"

#include "ConstantValueExtrapolation.h"

#include "firePresets.h"

// 1.	Skapa ett Gridfield ohm genom nabmla x u (dvs. curl)
// 2.	Skapa N genom N = (gradienten av normen av ohm)/(gradient av t�ljaren)
//		N kommer d� peka fr�n l�g-vorticity-omr�den till omr�den med
//		h�g vorticity.
// 3.	G�r (N x ohm)*dx*epsilon och spara i GridField-referensen forces.

void Vorticity::addVorticity(const MACGrid &u, GridField<Vector3> &forces, const double epsilon, const double dx, const int dimx, const int dimy, const int dimz){
		
	GridField<double> ohmNorm = GridField<double>(dimx, dimy, dimz, FirePresets::GRID_SIZE, new ConstantValueExtrapolation<double>());  //TODO KORREKT EXTRAPOLERING?
	GridField<Vector3> ohmVecGrid = GridField<Vector3>(dimx, dimy, dimz, FirePresets::GRID_SIZE, new ConstantValueExtrapolation<Vector3>());  //TODO KORREKT EXTRAPOLERING?
		
	Discretization *d = new CentralDiff();
	Gradient g;
	Vector3 fconf, ohmVecTemp, n, N;

	// Populera GridField ohm med vorticity v�rden
	for(int i=0; i < dimx; ++i){
		for(int j=0; j < dimy; ++j){
			for(int k=0; k< dimz; ++k){
				if( (i > 1) && (j > 1) && (i < dimx-2) && (j < dimy-2) ){
					Vector3 
					centerVelUp = u.velocityAtCenter(i,j+1,k),
					centerVelDown = u.velocityAtCenter(i,j-1,k),
					centerVelRight = u.velocityAtCenter(i+1,j,k),
					centerVelLeft = u.velocityAtCenter(i-1,j,k),
					centerVelFront = u.velocityAtCenter(i,j,k+1),
					centerVelBack = u.velocityAtCenter(i,j,k-1),
					centerVel = u.velocityAtCenter(i,j,k);
						
					// Fr�n Fedkiw 2001, Smoke etc. ekv (9) s.3 samt diskretisering enl. s.6
					// ohm = nabla x �
					ohmVecTemp = Vector3(	(centerVelUp.z - centerVelDown.z - centerVelFront.y + centerVelBack.y)/(2*dx),
											(centerVelFront.x - centerVelBack.x - centerVelRight.z + centerVelLeft.z)/(2*dx),
											(centerVelRight.y - centerVelLeft.y - centerVelUp.x + centerVelDown.x)/(2*dx) );			

					ohmNorm.setValueAtIndex(ohmVecTemp.norm(),i,j,k);
					ohmVecGrid.setValueAtIndex(ohmVecTemp,i,j,k);
				}
			}
		}
	}

	// Ber�kna n, N and fcont
	for(int i=0; i < dimx; ++i){
		for(int j=0; j < dimy; ++j){
			for(int k=0; k< dimz; ++k){
				if( (i > 1) && (j > 1) && (i < dimx-2) && (j < dimy-2) ){
						
					n = g.getGradient(ohmNorm, i, j, k, *d);

					(n.norm() != 0.) ? N = n/n.norm() : N = Vector3(0.);

					Vector3 ohmVecTemp;

					ohmVecTemp = ohmVecGrid.valueAtIndex(i,j,k);

					Vector3 crossprod(	N.y * ohmVecTemp.z - N.z * ohmVecTemp.y,
										-(N.x * ohmVecTemp.z - N.z * ohmVecTemp.x),
										N.x * ohmVecTemp.y - N.y * ohmVecTemp.x);

					fconf = crossprod*dx*epsilon;

					forces.setValueAtIndex(fconf,i,j,k);
				}
			}
		}
	}
	delete d;
}
