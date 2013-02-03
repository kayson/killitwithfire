#include "Helper.h"

Helper::Helper(){}

Helper::~Helper(){}

// Central diff. i, j, k är halva index i staggered MAC grid.
double Helper::calcDxCentralDiff(Grid &g, int i , int j, int k){
	return (double)(g(i-1, j, k) + g(i+1, j, k))/2;//GRID_DIM_X;
}
double Helper::calcDxCentralDiff(Grid &g, int i , int j, int k){
	return (double)(g(i, j-1, k) + g(i, j+1, k))/2;//GRID_DIM_Y;
}
double Helper::calcDxCentralDiff(Grid &g, int i , int j, int k){
	return (double)(g(i, j, k-1) + g(i, j, k+1))/2;//GRID_DIM_Z;
}

// Här kan man enkelt anropa andra diskretiserings-metoder om man vill.
double Helper::calcDx(Grid &g, int i, int j, int k){
	return calcDxCentralDiff(g, i, j, k);
}
double Helper::calcDy(Grid &g, int i, int j, int k){
	return calcDyCentralDiff(g, i, j, k);
}
double Helper::calcDz(Grid &g, int i, int j, int k){
	return calcDzCentralDiff(g, i, j, k);
}

vec Helper::gradient(Grid &g, int i, int j, int k)
{
	std::cout << "gradient(..) called\n";
	float dx=0, dy=0, dz=0;

	/*cout << "i = " << i << ", j = " << j << endl;
	cout << "g(i,j,k) = " << (double)g(i,j,k) << endl;
	cout << "g(i-1,j,k) = " << (double)g(i-1,j,k) << endl;
	cout << "g(i+1,j,k) = " << (double)g(i+1,j,k) << endl;
	cout << "g(i,j-1,k) = " << (double)g(i,j-1,k) << endl;
	cout << "g(i,j+1,k) = " << (double)g(i,j+1,k) << endl;*/

	// Central diff. Är egentligen halva index i 
	// staggered MAC grid
	dx = calcDx(g, i, j, k);
	dy = calcDy(g, i, j, k);
	dz = 0;//Because 2d, calcDz(g, i, j, k);

	vec v(3); // Vektor med dim 3x1
	v(0) = dx;
	v(1) = dy;
	v(2) = dz;

	std::cout << "Gradient is: \n" << v << endl;

	return v;
}

double Helper::divergence(Grid &g, int i, int j, int k)
{
	std::cout << "divergence(..) called\n";
	float dx=0, dy=0, dz=0;
	// Central diff. Är egentligen halva index i 
	// staggered MAC grid
	dx = calcDx(g, i, j, k);
	dy = calcDy(g, i, j, k);
	dz = 0;//Because 2d, calcDz(g, i, j, k);

	return dx+dy+dz;
}