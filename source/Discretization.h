/*
	Bas-klass för olika diskretiseringsmetoder. Virtuella metoder
	skall överlagras av sub-klasser
*/

#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H
#include <iostream>
#include "Grid.h"
#include "datatype.h"

class Discretization{
public:
	//Discretization();
	//~Discretization();

	// Dessa skall och måste överlagras av subklasser
	virtual data calcDx(Grid<data> &g, int i, int j, int k)=0;
	virtual data calcDy(Grid<data> &g, int i, int j, int k)=0;
	virtual data calcDz(Grid<data> &g, int i, int j, int k)=0;

	virtual data calcD2x(Grid<data> &g, int i, int j, int k)=0;
	virtual data calcD2y(Grid<data> &g, int i, int j, int k)=0;
	virtual data calcD2z(Grid<data> &g, int i, int j, int k)=0;

	virtual data calcDyz(Grid<data> &g, int i, int j, int k)=0;
	virtual data calcDxz(Grid<data> &g, int i, int j, int k)=0;
	virtual data calcDxy(Grid<data> &g, int i, int j, int k)=0;

protected:
	data dx, dy, dz, d2x, d2y, d2z;

};
#endif