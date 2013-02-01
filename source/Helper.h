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
	float divergence(Grid &g, int i, int j, int k);
};
#endif