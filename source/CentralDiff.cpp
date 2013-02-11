#include "CentralDiff.h"
#include "fire.h"
/*
	Beräknar andra ordningens approx. av första-derivatan eller
	andra-derivatan i rummet med central differentiation. Index
	är eg. halva index för staggered MAC-grid.
*/
CentralDiff::CentralDiff(){}

data CentralDiff::calcDx(Grid &g, const int i, const int j, const int k)
{
	return (double)(g(i+1, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);  //deltaX;
}
data CentralDiff::calcDy(Grid &g, const int i, const int j, const int k)
{
	return (double)(g(i, j+1, k) + g(i, j-1, k))/(FirePresets::dx*2.0);  //deltaY;
}
data CentralDiff::calcDz(Grid &g, const int i, const int j, const int k)
{
	return (double)(g(i, j, k+1) + g(i, j, k-1))/(FirePresets::dx*2.0);  //deltaZ;
}
data CentralDiff::calcD2x(Grid &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k) - 2*g(i,j,k) + g(i-1,j,k))/( FirePresets::dx*FirePresets::dx /*deltaX^2 */ );
}
data CentralDiff::calcD2y(Grid &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k) - 2*g(i,j,k) + g(i,j-1,k))/( FirePresets::dx*FirePresets::dx /* deltaY^2 */ );
}
data CentralDiff::calcD2z(Grid &g, const int i, const int j, const int k)
{
	return ( g(i,j,k+1) - 2*g(i,j,k) + g(i,j,k-1))/( FirePresets::dx*FirePresets::dx /* deltaZ^2 */ );
}
data CentralDiff::calcDyz(Grid &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4*FirePresets::dx*FirePresets::dx);
	//return 0;
}
data CentralDiff::calcDxz(Grid &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4*FirePresets::dx*FirePresets::dx);
}
data CentralDiff::calcDxy(Grid &g, const int i, const int j, const int k)
{
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4*FirePresets::dx*FirePresets::dx);
}