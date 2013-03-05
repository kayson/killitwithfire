#include "CentralDiff.h"
#include "presets\firePresets.h"
#include "Vector3.h"
#include "GridField.hpp"
/*
	Johan Noren
	Beräknar andra ordningens approx. av första-derivatan resp.
	andra-derivatan i rummet med central differentiation. Index
	är eg. halva index för staggered MAC-grid. Konstant extrapolering
	görs för kant-celler.

	//Egentligen en Colocated
*/

double CentralDiff::calcDxm(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j, k) - g(i-1, j, k))/(FirePresets::dx);
}

double CentralDiff::calcDym(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j, k) - g(i, j - 1, k))/(FirePresets::dx);
}

double CentralDiff::calcDzm(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j, k) - g(i, j, k - 1))/(FirePresets::dx);
}

double CentralDiff::calcDxp(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i + 1, j, k) - g(i, j, k))/(FirePresets::dx);
}

double CentralDiff::calcDyp(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j + 1, k) - g(i, j, k))/(FirePresets::dx);
}

double CentralDiff::calcDzp(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j, k + 1) - g(i, j, k))/(FirePresets::dx);
}

double CentralDiff::calcDx(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i+1, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);
}
double CentralDiff::calcDy(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j+1, k) - g(i, j-1, k))/(FirePresets::dx*2.0);
}
double CentralDiff::calcDz(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j, k+1) - g(i, j, k-1))/(FirePresets::dx*2.0);
}

double CentralDiff::calcD2x(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k) - 2.0*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
}
double CentralDiff::calcD2y(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k) - 2.0*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
}
double CentralDiff::calcD2z(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i,j,k+1) - 2.0*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
}
double CentralDiff::calcDyz(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4.0*FirePresets::dx*FirePresets::dx);
}
double CentralDiff::calcDxz(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4.0*FirePresets::dx*FirePresets::dx);
}
double CentralDiff::calcDxy(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4.0*FirePresets::dx*FirePresets::dx);
}

// Samma funktioner men med Vector3

Vector3 CentralDiff::calcDx(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return (Vector3)(g(i+1, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);
}
Vector3 CentralDiff::calcDy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return (Vector3)(g(i, j+1, k) - g(i, j-1, k))/(FirePresets::dx*2.0);
}
Vector3 CentralDiff::calcDz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return (Vector3)(g(i, j, k+1) - g(i, j, k-1))/(FirePresets::dx*2.0);
}

Vector3 CentralDiff::calcD2x(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k) - 2.0*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
}
Vector3 CentralDiff::calcD2y(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k) - 2.0*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
}
Vector3 CentralDiff::calcD2z(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i,j,k+1) - 2.0*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
}
Vector3 CentralDiff::calcDyz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4.0*FirePresets::dx*FirePresets::dx);
}
Vector3 CentralDiff::calcDxz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4.0*FirePresets::dx*FirePresets::dx);
}
Vector3 CentralDiff::calcDxy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4.0*FirePresets::dx*FirePresets::dx);
}