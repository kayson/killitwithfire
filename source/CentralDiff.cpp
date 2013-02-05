#include "CentralDiff.h"

/*
	Beräknar första ordningens eller andra ordningens
	derivata med central differentiation
*/

data CentralDiff::calcDx(Grid<data> &g, int i, int j, int k)
{
	return (double)(g(i-1, j, k) + g(i+1, j, k))/2;//GRID_DIM_X;
}
data CentralDiff::calcDy(Grid<data> &g, int i, int j, int k)
{
	return (double)(g(i, j-1, k) + g(i, j+1, k))/2;//GRID_DIM_Y;
}
data CentralDiff::calcDz(Grid<data> &g, int i, int j, int k)
{
	return (double)(g(i, j, k-1) + g(i, j, k+1))/2;//GRID_DIM_Z;
}
data CentralDiff::calcD2x(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}
data CentralDiff::calcD2y(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}
data CentralDiff::calcD2z(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}