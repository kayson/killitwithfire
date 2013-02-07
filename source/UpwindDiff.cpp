#include "UpwindDiff.h"
#include "fire.h"
/*
	Beräknar andra ordningens approximation av första-upwind differentiation.
	Index är eg. halva index för staggered MAC-grid.
*/
UpwindDiff::UpwindDiff(){}

data UpwindDiff::calcDx(Grid &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).x > 0)
		return (g(i, j, k) - g(i-1, j, k))/FirePresets::dx;
	else
		return (g(i+1, j, k) - g(i, j, k))/FirePresets::dx;
}
data UpwindDiff::calcDy(Grid &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).y > 0)
		return (g(i, j, k) - g(i, j-1, k))/FirePresets::dx;
	else
		return (g(i, j+1, k) - g(i, j, k))/FirePresets::dx;
}
data UpwindDiff::calcDz(Grid &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).z > 0)
		return (g(i, j, k) - g(i, j, k-1))/FirePresets::dx;
	else
		return (g(i, j, k+1) - g(i, j, k))/FirePresets::dx;
}

data UpwindDiff::calcD2x(Grid &g, const int i, const int j, const int k)
{return 0;}
data UpwindDiff::calcD2y(Grid &g, const int i, const int j, const int k)
{return 0;}
data UpwindDiff::calcD2z(Grid &g, const int i, const int j, const int k)
{return 0;}

data UpwindDiff::calcDyz(Grid &g, const int i, const int j, const int k)
{return 0;}
data UpwindDiff::calcDxz(Grid &g, const int i, const int j, const int k)
{return 0;}
data UpwindDiff::calcDxy(Grid &g, const int i, const int j, const int k)
{return 0;}