#include "UpwindDiff.h"
#include "presets\firePresets.h"
#include "Vector3.h"
#include "GridField.hpp"
/*
	Ber�knar andra ordningens approximation av f�rsta-upwind differentiation.
	Index �r eg. halva index f�r staggered MAC-grid.
*/

double UpwindDiff::calcDxm(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j, k) - g(i-1, j, k))/(FirePresets::dx);
}

double UpwindDiff::calcDym(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j, k) - g(i, j-1, k))/(FirePresets::dx);
}

double UpwindDiff::calcDzm(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j, k) - g(i, j, k-1))/(FirePresets::dx);
}

double UpwindDiff::calcDxp(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i+1, j, k) - g(i, j, k))/(FirePresets::dx);
}

double UpwindDiff::calcDyp(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j + 1, k) - g(i, j, k))/(FirePresets::dx);
}

double UpwindDiff::calcDzp(GridField<double> &g, const int i, const int j, const int k)
{
	return (double)(g(i, j, k + 1) - g(i, j, k))/(FirePresets::dx);
}

double UpwindDiff::calcDx(GridField<double> &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).x > 0.0)
		return (g(i, j, k) - g(i-1, j, k))/FirePresets::dx;
	else
		return (g(i+1, j, k) - g(i, j, k))/FirePresets::dx;
}
double UpwindDiff::calcDy(GridField<double> &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).y > 0.0)
		return (g(i, j, k) - g(i, j-1, k))/FirePresets::dx;
	else
		return (g(i, j+1, k) - g(i, j, k))/FirePresets::dx;
}
double UpwindDiff::calcDz(GridField<double> &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).z > 0.0)
		return (g(i, j, k) - g(i, j, k-1))/FirePresets::dx;
	else
		return (g(i, j, k+1) - g(i, j, k))/FirePresets::dx;
}

double UpwindDiff::calcD2x(GridField<double> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return 0;
}
double UpwindDiff::calcD2y(GridField<double> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return 0;
}
double UpwindDiff::calcD2z(GridField<double> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return 0;
}

double UpwindDiff::calcDyz(GridField<double> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return 0;
}
double UpwindDiff::calcDxz(GridField<double> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return 0;
}
double UpwindDiff::calcDxy(GridField<double> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return 0;
}

// Funktions �verlagring med Vector3 ist�llet f�r double

Vector3 UpwindDiff::calcDx(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).x > 0.0)
		return (g(i, j, k) - g(i-1, j, k))/FirePresets::dx;
	else
		return (g(i+1, j, k) - g(i, j, k))/FirePresets::dx;
}
Vector3 UpwindDiff::calcDy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).y > 0.0)
		return (g(i, j, k) - g(i, j-1, k))/FirePresets::dx;
	else
		return (g(i, j+1, k) - g(i, j, k))/FirePresets::dx;
}
Vector3 UpwindDiff::calcDz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	if((*_w)(i, j, k).z > 0.0)
		return (g(i, j, k) - g(i, j, k-1))/FirePresets::dx;
	else
		return (g(i, j, k+1) - g(i, j, k))/FirePresets::dx;
}

Vector3 UpwindDiff::calcD2x(GridField<Vector3> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return Vector3();
}
Vector3 UpwindDiff::calcD2y(GridField<Vector3> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return Vector3();
}
Vector3 UpwindDiff::calcD2z(GridField<Vector3> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return Vector3();
}

Vector3 UpwindDiff::calcDyz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return Vector3();
}
Vector3 UpwindDiff::calcDxz(GridField<Vector3> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return Vector3();
}
Vector3 UpwindDiff::calcDxy(GridField<Vector3> &g, const int i, const int j, const int k)
{
	throw; //EJ DEFINIERAD
	return Vector3();
}