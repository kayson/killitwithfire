#include "CentralDiff.h"
/*
	Beräknar andra ordningens approx. av första-derivatan eller
	andra-derivatan i rummet med central differentiation. Index
	är eg. halva index för staggered MAC-grid.
*/
CentralDiff::CentralDiff(){}

data CentralDiff::calcDx(Grid &g, const int i, const int j, const int k)
{
	return (double)(g(i+1, j, k) - g(i-1, j, k))/GRID_SIZE;//deltaX;
}
data CentralDiff::calcDy(Grid &g, const int i, const int j, const int k)
{
	return (double)(g(i, j+1, k) + g(i, j-1, k))/GRID_SIZE;//deltaY;
}
data CentralDiff::calcDz(Grid &g, const int i, const int j, const int k)
{
	return (double)(g(i, j, k+1) + g(i, j, k-1))/GRID_SIZE;//deltaZ;
}
data CentralDiff::calcD2x(Grid &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k) - 2*g(i,j,k) + g(i-1,j,k))/( GRID_SIZE*GRID_SIZE /*deltaX^2 */ );
}
data CentralDiff::calcD2y(Grid &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k) - 2*g(i,j,k) + g(i,j-1,k))/( GRID_SIZE*GRID_SIZE /* deltaY^2 */ );
}
data CentralDiff::calcD2z(Grid &g, const int i, const int j, const int k)
{
	return ( g(i,j,k+1) - 2*g(i,j,k) + g(i,j,k-1))/( GRID_SIZE*GRID_SIZE /* deltaZ^2 */ );
}
data CentralDiff::calcDyz(Grid &g, const int i, const int j, const int k)
{
	return ( g(i,j+1,k+1) - g(i,j+1,k-1) + g(i,j-1,k-1) - g(i,j-1,k+1) ) / (4*GRID_SIZE*GRID_SIZE);
	//return 0;
}
data CentralDiff::calcDxz(Grid &g, const int i, const int j, const int k)
{
	return ( g(i+1,j,k+1) - g(i+1,j,k-1) + g(i-1,j,k-1) - g(i-1,j,k+1) ) / (4*GRID_SIZE*GRID_SIZE);
}
data CentralDiff::calcDxy(Grid &g, const int i, const int j, const int k)
{
	return ( g(i+1,j+1,k) - g(i+1,j-1,k) + g(i-1,j-1,k) - g(i-1,j+1,k) ) / (4*GRID_SIZE*GRID_SIZE);
}