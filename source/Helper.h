#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include "Grid.h"
#include "armadillo"

using namespace arma;

// Skicka in referens till grid samt vilken position
// man vill ha gradient eller divergens osv. av.
class Helper
{
public:
	Helper();
	~Helper();
	vec gradient(Grid &g, int i, int j, int k);
	double divergence(Grid &g, int i, int j, int k);
private:
	double calcDx(Grid &g, int i , int j, int k);
	double calcDy(Grid &g, int i , int j, int k);
	double calcDz(Grid &g, int i , int j, int k);

	double calcDxCentralDiff(Grid &g, int i , int j, int k);
	double calcDyCentralDiff(Grid &g, int i , int j, int k);
	double calcDzCentralDiff(Grid &g, int i , int j, int k);
};
#endif