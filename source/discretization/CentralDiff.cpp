#include "CentralDiff.h"
#include "firePresets.h"
#include "Vector3.h"
#include "GridField.hpp"
#include "fire.h"
/*
	Johan Noren
	Ber�knar andra ordningens approx. av f�rsta-derivatan resp.
	andra-derivatan i rummet med central differentiation. Index
	�r eg. halva index f�r staggered MAC-grid. Konstant extrapolering
	g�rs f�r kant-celler.

	//Egentligen en Colocated
*/
#ifdef __APPLE__
#include <math.h>

#define _isnan(x) isnan(x)
#endif

double CentralDiff::calcDxm(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j, k) - g(i-1, j, k))/(g.dx());
}

double CentralDiff::calcDym(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j, k) - g(i, j - 1, k))/(g.dx());
}

double CentralDiff::calcDzm(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j, k) - g(i, j, k - 1))/(g.dx());
}

double CentralDiff::calcDxp(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i + 1, j, k) - g(i, j, k))/(g.dx());
}

double CentralDiff::calcDyp(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j + 1, k) - g(i, j, k))/(g.dx());
}

double CentralDiff::calcDzp(GridField<double> &g, const int i, const int j, const int k)
{
	throw;
	//EJ DEFINIERAD I CENTRAL
	return (double)(g(i, j, k + 1) - g(i, j, k))/(g.dx());
}

double CentralDiff::calcDx(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i+1, j, k) - g(i-1, j, k))/(g.dx()*2.0);
}
double CentralDiff::calcDy(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j+1, k) - g(i, j-1, k))/(g.dx()*2.0);
}
double CentralDiff::calcDz(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j, k+1) - g(i, j, k-1))/(g.dx()*2.0);
}

double CentralDiff::calcD2x(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k) - 2.0*g(i,j,k) + g(i-1,j,k))/( g.dx()*g.dx() /*deltaX^2 */ );
}
double CentralDiff::calcD2y(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k) - 2.0*g(i,j,k) + g(i,j-1,k))/( g.dx()*g.dx() /* deltaY^2 */ );
}
double CentralDiff::calcD2z(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i,j,k+1) - 2.0*g(i,j,k) + g(i,j,k-1))/( g.dx()*g.dx() /* deltaZ^2 */ );
}
double CentralDiff::calcDyz(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4.0*g.dx()*g.dx());
}
double CentralDiff::calcDxz(GridField<double> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4.0*g.dx()*g.dx());
}
double CentralDiff::calcDxy(GridField<double> &g, const int i, const int j, const int k)
{
	/*CellType c_t = LevelSet::getCellType(g(i,j,k));
	return ( g(i+1,j+1,k,c_t) - g(i+1,j-1,k,c_t) + g(i-1,j-1,k,c_t) - g(i-1,j+1,k,c_t) ) / (4.0*g.dx()*g.dx());*/
	/*double asd4 = g(i-1,j+1,k);
	double asd3 = g(i-1,j-1,k);
	double asd2 = g(i+1,j-1,k);
	double asd1 = g(i+1,j+1,k);
     */
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4.0*g.dx()*g.dx());
}

// GhostGridField-metoder
double calcDx(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return (double)(g(i+1, j, k, c_t) - g(i-1, j, k, c_t))/(g.dx()*2.0);
}
double calcDy(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return (double)(g(i, j+1, k, c_t) - g(i, j-1, k, c_t))/(g.dx()*2.0);
}
double calcDz(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return (double)(g(i, j, k+1, c_t) - g(i, j, k-1, c_t))/(g.dx()*2.0);
}
double calcD2x(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return ( g(i+1,j,k,c_t) - 2.0*g(i,j,k,c_t) + g(i-1,j,k,c_t))/( g.dx()*g.dx() );
}
double calcD2y(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return ( g(i,j+1,k,c_t) - 2.0*g(i,j,k,c_t) + g(i,j-1,k,c_t))/( g.dx()*g.dx() );
}
double calcD2z(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return ( g(i,j,k+1,c_t) - 2.0*g(i,j,k,c_t) + g(i,j,k-1,c_t))/( g.dx()*g.dx() );
}
double calcDyz(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return ( g(i,j+1,k+1,c_t) - g(i,j+1,k-1,c_t) + g(i,j-1,k-1,c_t) - g(i,j-1,k+1,c_t) ) / (4.0*g.dx()*g.dx());
}
double calcDxz(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return ( g(i+1,j,k+1,c_t) - g(i+1,j,k-1,c_t) + g(i-1,j,k-1,c_t) - g(i-1,j,k+1,c_t) ) / (4.0*g.dx()*g.dx());
}
double calcDxy(GhostGridField<double> &g, const int i, const int j, const int k){
	CellType c_t = LevelSet::getCellType(g(i,j,k));
	return ( g(i+1,j+1,k,c_t) - g(i+1,j-1,k,c_t) + g(i-1,j-1,k,c_t) - g(i-1,j+1,k,c_t) ) / (4.0*g.dx()*g.dx());
}

// Samma funktioner men med Vector3

Vector3 CentralDiff::calcDx(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return (Vector3)(g(i+1, j, k) - g(i-1, j, k))/(g.dx()*2.0);
}
Vector3 CentralDiff::calcDy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return (Vector3)(g(i, j+1, k) - g(i, j-1, k))/(g.dx()*2.0);
}
Vector3 CentralDiff::calcDz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return (Vector3)(g(i, j, k+1) - g(i, j, k-1))/(g.dx()*2.0);
}

Vector3 CentralDiff::calcD2x(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k) - 2.0*g(i,j,k) + g(i-1,j,k))/( g.dx()*g.dx() /*deltaX^2 */ );
}
Vector3 CentralDiff::calcD2y(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k) - 2.0*g(i,j,k) + g(i,j-1,k))/( g.dx()*g.dx() /* deltaY^2 */ );
}
Vector3 CentralDiff::calcD2z(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i,j,k+1) - 2.0*g(i,j,k) + g(i,j,k-1))/( g.dx()*g.dx() /* deltaZ^2 */ );
}
Vector3 CentralDiff::calcDyz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4.0*g.dx()*g.dx());
}
Vector3 CentralDiff::calcDxz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4.0*g.dx()*g.dx());
}
Vector3 CentralDiff::calcDxy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4.0*g.dx()*g.dx());
}
