#include "CentralDiff.h"
#include "fire.h"
#include "ExtrapolateConstant.h"
/*
	Johan Noren
	Beräknar andra ordningens approx. av första-derivatan resp.
	andra-derivatan i rummet med central differentiation. Index
	är eg. halva index för staggered MAC-grid. Konstant extrapolering
	görs för kant-celler.

	//Egentligen en Colocated
*/
CentralDiff::CentralDiff()
{
	ext = new ExtrapolateConstant();
}

CentralDiff::~CentralDiff()
{
	delete ext;
}

double CentralDiff::calcDxm(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i, j, k) - g(i-1, j, k))/(FirePresets::dx);
}

double CentralDiff::calcDym(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i, j, k) - g(i, j - 1, k))/(FirePresets::dx);
}

double CentralDiff::calcDzm(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i, j, k) - g(i, j, k - 1))/(FirePresets::dx);
}

double CentralDiff::calcDxp(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i + 1, j, k) - g(i, j, k))/(FirePresets::dx);
}

double CentralDiff::calcDyp(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i, j + 1, k) - g(i, j, k))/(FirePresets::dx);
}

double CentralDiff::calcDzp(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i, j, k + 1) - g(i, j, k))/(FirePresets::dx);
}

double CentralDiff::calcDx(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i+1, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);
}
double CentralDiff::calcDy(GridField<double> &g, const int i, const int j, const int k)
{
	if(j <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(j >= g.ydim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i, j+1, k) - g(i, j-1, k))/(FirePresets::dx*2.0);
}
double CentralDiff::calcDz(GridField<double> &g, const int i, const int j, const int k)
{
	if(k <= 0)
		ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(k >= g.zdim()-1)
		ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (double)(g(i, j, k+1) - g(i, j, k-1))/(FirePresets::dx*2.0);
}

double CentralDiff::calcD2x(GridField<double> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ( g(i+1,j,k) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
	if(i >= g.xdim()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
	else
		return ( g(i+1,j,k) - 2*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
}
double CentralDiff::calcD2y(GridField<double> &g, const int i, const int j, const int k)
{
	if(j <= 0)
		return ( g(i,j+1,k) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
	if(j >= g.ydim()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
	else
		return ( g(i,j+1,k) - 2*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
}
double CentralDiff::calcD2z(GridField<double> &g, const int i, const int j, const int k)
{
	if(k <= 0)
		return ( g(i,j,k+1) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
	if(k >= g.zdim()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
	else
		return ( g(i,j,k+1) - 2*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
}
double CentralDiff::calcDyz(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4*FirePresets::dx*FirePresets::dx);
	//return 0;
}
double CentralDiff::calcDxz(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4*FirePresets::dx*FirePresets::dx);
}
double CentralDiff::calcDxy(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4*FirePresets::dx*FirePresets::dx);
}

// Samma funktioner men med Vector3

Vector3 CentralDiff::calcDx(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(i >= g.xdim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (Vector3)(g(i+1, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);
}
Vector3 CentralDiff::calcDy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(j <= 0)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(j >= g.ydim()-1)
		return ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (Vector3)(g(i, j+1, k) - g(i, j-1, k))/(FirePresets::dx*2.0);
}
Vector3 CentralDiff::calcDz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(k <= 0)
		ext->extrapolate(g,i,j,k,*this); // Extrapolering
	if(k >= g.zdim()-1)
		ext->extrapolate(g,i,j,k,*this); // Extrapolering
	else
		return (Vector3)(g(i, j, k+1) - g(i, j, k-1))/(FirePresets::dx*2.0);
}

Vector3 CentralDiff::calcD2x(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ( g(i+1,j,k) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
	if(i >= g.xdim()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
	else
		return ( g(i+1,j,k) - 2*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
}
Vector3 CentralDiff::calcD2y(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(j <= 0)
		return ( g(i,j+1,k) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
	if(j >= g.ydim()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
	else
		return ( g(i,j+1,k) - 2*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
}
Vector3 CentralDiff::calcD2z(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if(k <= 0)
		return ( g(i,j,k+1) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
	if(k >= g.zdim()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
	else
		return ( g(i,j,k+1) - 2*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
}
Vector3 CentralDiff::calcDyz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4*FirePresets::dx*FirePresets::dx);
	//return 0;
}
Vector3 CentralDiff::calcDxz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4*FirePresets::dx*FirePresets::dx);
}
Vector3 CentralDiff::calcDxy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4*FirePresets::dx*FirePresets::dx);
}