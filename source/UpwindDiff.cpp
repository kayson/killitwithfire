#include "UpwindDiff.h"

/*
	Beräknar andra ordningens approximation av första-upwind differentiation.
	Index är eg. halva index för staggered MAC-grid.
*/
UpwindDiff::UpwindDiff(){}

// Att implementera
data UpwindDiff::calcDx(Grid &g, int i, int j, int k)
{
	//if(w(i,j,k).x > 0)
	if(true)
		return (g(i, j, k) - g(i-1, j, k))/GRID_SIZE;
	else
		return (g(i+1, j, k) - g(i, j, k))/GRID_SIZE;
}
data UpwindDiff::calcDy(Grid &g, int i, int j, int k)
{
	//if(w(i,j,k).y > 0)
	if(true)
		return (g(i, j, k) - g(i, j-1, k))/GRID_SIZE;
	else
		return (g(i, j+1, k) - g(i, j, k))/GRID_SIZE;
}
data UpwindDiff::calcDz(Grid &g, int i, int j, int k)
{
	//if(w(i,j,k).x > 0)
	if(true)
		return (g(i, j, k) - g(i, j, k-1))/GRID_SIZE;
	else
		return (g(i, j, k+1) - g(i, j, k))/GRID_SIZE;
}

data UpwindDiff::calcD2x(Grid &g, int i, int j, int k)
{return 0;}
data UpwindDiff::calcD2y(Grid &g, int i, int j, int k)
{return 0;}
data UpwindDiff::calcD2z(Grid &g, int i, int j, int k)
{return 0;}

data UpwindDiff::calcDyz(Grid &g, int i, int j, int k)
{return 0;}
data UpwindDiff::calcDxz(Grid &g, int i, int j, int k)
{return 0;}
data UpwindDiff::calcDxy(Grid &g, int i, int j, int k)
{return 0;}