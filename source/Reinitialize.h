#ifndef REINITIALIZE_H
#define REINITIALIZE_H

template<class T> class GridField;

class reinitialize{
public:
	static double Evaluate(GridField<double> &g, int i, int j, int k);
	static void reinitializeGrid(GridField<double> **g, GridField<double> **gridCopy);
private:
	static double CalcMeanGradient(GridField<double> &g);
	static double CalcMaxGradient(GridField<double> &g);
	static void Godunov(GridField<double> &g, int i, int j, int k, double a, double &ddx2, double &ddy2, double &ddz2);
};

#endif