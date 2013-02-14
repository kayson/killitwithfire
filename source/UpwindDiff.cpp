#include "UpwindDiff.h"
#include "fire.h"
#include "ExtrapolateConstant.h"
/*
	Beräknar andra ordningens approximation av första-upwind differentiation.
	Index är eg. halva index för staggered MAC-grid.
*/
UpwindDiff::UpwindDiff()
{
	ext = new ExtrapolateConstant();
}

UpwindDiff::~UpwindDiff()
{
	delete ext;
}

double UpwindDiff::calcDx(Grid &g, const int i, const int j, const int k)
{
	if(i <= 0 || i >= g.getDimX()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).x > 0)
		return (g(i, j, k) - g(i-1, j, k))/FirePresets::dx;
	else
		return (g(i+1, j, k) - g(i, j, k))/FirePresets::dx;
}
double UpwindDiff::calcDy(Grid &g, const int i, const int j, const int k)
{
	if(j <= 0 || j >= g.getDimY()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).y > 0)
		return (g(i, j, k) - g(i, j-1, k))/FirePresets::dx;
	else
		return (g(i, j+1, k) - g(i, j, k))/FirePresets::dx;
}
double UpwindDiff::calcDz(Grid &g, const int i, const int j, const int k)
{
	if(k <= 0 || k >= g.getDimZ()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).z > 0)
		return (g(i, j, k) - g(i, j, k-1))/FirePresets::dx;
	else
		return (g(i, j, k+1) - g(i, j, k))/FirePresets::dx;
}

double UpwindDiff::calcD2x(Grid &g, const int i, const int j, const int k)
{return 0;}
double UpwindDiff::calcD2y(Grid &g, const int i, const int j, const int k)
{return 0;}
double UpwindDiff::calcD2z(Grid &g, const int i, const int j, const int k)
{return 0;}

double UpwindDiff::calcDyz(Grid &g, const int i, const int j, const int k)
{return 0;}
double UpwindDiff::calcDxz(Grid &g, const int i, const int j, const int k)
{return 0;}
double UpwindDiff::calcDxy(Grid &g, const int i, const int j, const int k)
{return 0;}