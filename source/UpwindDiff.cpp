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

double UpwindDiff::calcDx(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0 || i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).x > 0)
		return (g(i, j, k) - g(i-1, j, k))/FirePresets::dx;
	else
		return (g(i+1, j, k) - g(i, j, k))/FirePresets::dx;
}
double UpwindDiff::calcDy(GridField<double> &g, const int i, const int j, const int k)
{
	if(j <= 0 || j >= g.ydim()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).y > 0)
		return (g(i, j, k) - g(i, j-1, k))/FirePresets::dx;
	else
		return (g(i, j+1, k) - g(i, j, k))/FirePresets::dx;
}
double UpwindDiff::calcDz(GridField<double> &g, const int i, const int j, const int k)
{
	if(k <= 0 || k >= g.zdim()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).z > 0)
		return (g(i, j, k) - g(i, j, k-1))/FirePresets::dx;
	else
		return (g(i, j, k+1) - g(i, j, k))/FirePresets::dx;
}

double UpwindDiff::calcD2x(GridField<double> &g, const int i, const int j, const int k)
{return 0;}
double UpwindDiff::calcD2y(GridField<double> &g, const int i, const int j, const int k)
{return 0;}
double UpwindDiff::calcD2z(GridField<double> &g, const int i, const int j, const int k)
{return 0;}

double UpwindDiff::calcDyz(GridField<double> &g, const int i, const int j, const int k)
{return 0;}
double UpwindDiff::calcDxz(GridField<double> &g, const int i, const int j, const int k)
{return 0;}
double UpwindDiff::calcDxy(GridField<double> &g, const int i, const int j, const int k)
{return 0;}

// Funktions överlagring med Vector3 istället för double

Vector3 UpwindDiff::calcDx(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(i <= 0 || i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).x > 0)
		return (g(i, j, k) - g(i-1, j, k))/FirePresets::dx;
	else
		return (g(i+1, j, k) - g(i, j, k))/FirePresets::dx;
}
Vector3 UpwindDiff::calcDy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(j <= 0 || j >= g.ydim()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).y > 0)
		return (g(i, j, k) - g(i, j-1, k))/FirePresets::dx;
	else
		return (g(i, j+1, k) - g(i, j, k))/FirePresets::dx;
}
Vector3 UpwindDiff::calcDz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(k <= 0 || k >= g.zdim()-1)
		return ext->extrapolate(g,i,j,k,*this);	// Extrapolera
	if((*_w)(i, j, k).z > 0)
		return (g(i, j, k) - g(i, j, k-1))/FirePresets::dx;
	else
		return (g(i, j, k+1) - g(i, j, k))/FirePresets::dx;
}

Vector3 UpwindDiff::calcD2x(GridField<Vector3> &g, const int i, const int j, const int k)	{ return Vector3(); }
Vector3 UpwindDiff::calcD2y(GridField<Vector3> &g, const int i, const int j, const int k)	{ return Vector3(); }
Vector3 UpwindDiff::calcD2z(GridField<Vector3> &g, const int i, const int j, const int k)	{ return Vector3(); }
																							
Vector3 UpwindDiff::calcDyz(GridField<Vector3> &g, const int i, const int j, const int k)	{ return Vector3(); }
Vector3 UpwindDiff::calcDxz(GridField<Vector3> &g, const int i, const int j, const int k)	{ return Vector3(); }
Vector3 UpwindDiff::calcDxy(GridField<Vector3> &g, const int i, const int j, const int k)	{ return Vector3(); }
