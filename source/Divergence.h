#ifndef DIVERGENCE_H
#define DIVERGENCE_H

class Discretization;
template<class T> class GridField;


class Divergence
{
public:
	Divergence();

	static double getDivergence(GridField<double> &g, const int i, const int j, const int k, Discretization &c);

};

#endif