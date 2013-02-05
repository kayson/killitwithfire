#include <iostream>
#include "Grid.h"
#include "datatype.h"
class Discretization{
public:
	Discretization();
	virtual ~Discretization() = 0;

	// Dessa skall överlagras av subklasser
	virtual data calcDx(Grid<data> &g, int i, int j, int k) = 0;
	virtual data calcDy(Grid<data> &g, int i, int j, int k) = 0;
	virtual data calcDz(Grid<data> &g, int i, int j, int k) = 0;

	virtual data calcD2x(Grid<data> &g, int i, int j, int k) = 0;
	virtual data calcD2y(Grid<data> &g, int i, int j, int k) = 0;
	virtual data calcD2z(Grid<data> &g, int i, int j, int k) = 0;

private:
	data dx, dy, dz, d2x, d2y, d2z;

};