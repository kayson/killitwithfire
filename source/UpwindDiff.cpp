#include "UpwindDiff.h"

/*
	Ber�knar andra ordningens approximation av f�rsta-
	eller andra-derivatan i rummet med upwind differentiation.
	Index �r eg. halva index f�r staggered MAC-grid.
*/
UpwindDiff::UpwindDiff(){}
// Att implementera
data UpwindDiff::calcDx(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}
data UpwindDiff::calcDy(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}
data UpwindDiff::calcDz(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}
data UpwindDiff::calcD2x(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}
data UpwindDiff::calcD2y(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}
data UpwindDiff::calcD2z(Grid<data> &g, int i, int j, int k)
{
	// TODO
	return 0;
}
data UpwindDiff::calcDyz(Grid<data> &g, int i, int j, int k)
{return 0;}
data UpwindDiff::calcDxz(Grid<data> &g, int i, int j, int k)
{return 0;}
data UpwindDiff::calcDxy(Grid<data> &g, int i, int j, int k)
{return 0;}