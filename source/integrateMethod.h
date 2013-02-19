#ifndef INTEGRATE_H
#define INTEGRATE_H

class IntegrateMethod
{
public:
	virtual ~Integrate();
abstract:
	virtual double calculateIntegral(MACGrid &u, GridField<double> &g, double dt) = 0;

}

#endif