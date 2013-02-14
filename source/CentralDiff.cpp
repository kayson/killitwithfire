#include "CentralDiff.h"
#include "fire.h"
#include "ExtrapolateConstant.h"
/*
	Beräknar andra ordningens approx. av första-derivatan eller
	andra-derivatan i rummet med central differentiation. Index
	är eg. halva index för staggered MAC-grid. Konstant extrapolering
	görs för kant-celler.
*/
CentralDiff::CentralDiff()
{
	ext = new ExtrapolateConstant();
}

CentralDiff::~CentralDiff()
{
	delete ext;
}

double CentralDiff::calcDx(Grid &g, const int i, const int j, const int k)
{
	if(i <= 0)	// Konstant extrapolering
		return ext->extrapolate(g,i,j,k,*this);
	if(i >= g.getDimX()-1)	// Konstant extrapolering
		return ext->extrapolate(g,i,j,k,*this);
	else
		return (double)(g(i+1, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);
}
double CentralDiff::calcDy(Grid &g, const int i, const int j, const int k)
{
	if(j <= 0)	// Konstant extrapolering
		return ext->extrapolate(g,i,j,k,*this);
	if(j >= g.getDimY()-1)	// Konstant extrapolering
		return ext->extrapolate(g,i,j,k,*this);
	else
		return (double)(g(i, j+1, k) - g(i, j-1, k))/(FirePresets::dx*2.0);
}
double CentralDiff::calcDz(Grid &g, const int i, const int j, const int k)
{
	if(k <= 0)	// Konstant extrapolering
		ext->extrapolate(g,i,j,k,*this);
	if(k >= g.getDimZ()-1)	// Konstant extrapolering
		ext->extrapolate(g,i,j,k,*this);
	else
		return (double)(g(i, j, k+1) - g(i, j, k-1))/(FirePresets::dx*2.0);
}

double CentralDiff::calcD2x(Grid &g, const int i, const int j, const int k)
{
	if(i <= 0)
		return ( g(i+1,j,k) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
	if(i >= g.getDimX()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
	else
		return ( g(i+1,j,k) - 2*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
}
double CentralDiff::calcD2y(Grid &g, const int i, const int j, const int k)
{
	if(j <= 0)
		return ( g(i,j+1,k) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
	if(j >= g.getDimY()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
	else
		return ( g(i,j+1,k) - 2*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
}
double CentralDiff::calcD2z(Grid &g, const int i, const int j, const int k)
{
	if(k <= 0)
		return ( g(i,j,k+1) - 2*g(i,j,k) + g(i,j,k))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
	if(k >= g.getDimZ()-1)
		return ( g(i,j,k) - 2*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
	else
		return ( g(i,j,k+1) - 2*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
}
double CentralDiff::calcDyz(Grid &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4*FirePresets::dx*FirePresets::dx);
	//return 0;
}
double CentralDiff::calcDxz(Grid &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4*FirePresets::dx*FirePresets::dx);
}
double CentralDiff::calcDxy(Grid &g, const int i, const int j, const int k)
{
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4*FirePresets::dx*FirePresets::dx);
}