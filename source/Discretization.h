/*
	Bas-klass f�r olika diskretiseringsmetoder. Virtuella metoder
	skall �verlagras av sub-klasser
*/

#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H
#include <iostream>
#include "Grid.h"
#include "datatype.h"

class Discretization{
public:
	Discretization();
	//~Discretization();

	// Dessa skall �verlagras av subklasser
	virtual data calcDx(Grid<data> &g, int i, int j, int k);
	virtual data calcDy(Grid<data> &g, int i, int j, int k);
	virtual data calcDz(Grid<data> &g, int i, int j, int k);

	virtual data calcD2x(Grid<data> &g, int i, int j, int k);
	virtual data calcD2y(Grid<data> &g, int i, int j, int k);
	virtual data calcD2z(Grid<data> &g, int i, int j, int k);

protected:
	data dx, dy, dz, d2x, d2y, d2z;

};
#endif