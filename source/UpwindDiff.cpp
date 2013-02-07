#include "UpwindDiff.h"

/*
	Beräknar andra ordningens approximation av första-upwind differentiation.
	Index är eg. halva index för staggered MAC-grid.
*/
UpwindDiff::UpwindDiff(){}

// Att implementera
data UpwindDiff::calcDx(const Grid &g, const int i, const int j, const int k)
{
	//if(w(i,j,k).x > 0)
	if(true)
		return (g(i, j, k) - g(i-1, j, k))/GRID_SIZE;
	else
		return (g(i+1, j, k) - g(i, j, k))/GRID_SIZE;
}
data UpwindDiff::calcDy(const Grid &g, const int i, const int j, const int k)
{
	//if(w(i,j,k).y > 0)
	if(true)
		return (g(i, j, k) - g(i, j-1, k))/GRID_SIZE;
	else
		return (g(i, j+1, k) - g(i, j, k))/GRID_SIZE;
}
data UpwindDiff::calcDz(const Grid &g, const int i, const int j, const int k)
{
	//if(w(i,j,k).x > 0)
	if(true)
		return (g(i, j, k) - g(i, j, k-1))/GRID_SIZE;
	else
		return (g(i, j, k+1) - g(i, j, k))/GRID_SIZE;
}

data UpwindDiff::calcD2x(const Grid &g, const int i, const int j, const int k)
{return 0;}
data UpwindDiff::calcD2y(const Grid &g, const int i, const int j, const int k)
{return 0;}
data UpwindDiff::calcD2z(const Grid &g, const int i, const int j, const int k)
{return 0;}

data UpwindDiff::calcDyz(const Grid &g, const int i, const int j, const int k)
{return 0;}
data UpwindDiff::calcDxz(const Grid &g, const int i, const int j, const int k)
{return 0;}
data UpwindDiff::calcDxy(const Grid &g, const int i, const int j, const int k)
{return 0;}