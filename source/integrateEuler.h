#ifndef INTEGRATE_EULER_H
#define INTEGRATE_EULER_H

class IntegrateEuler : public IntegrateMethod
{
public:
	IntegrateEuler(){};
	~IntegrateEuler(){};
	virtual double calculateIntegral(MACGrid &u, GridField<double> &g, double dt)
	{

	};

}

#endif