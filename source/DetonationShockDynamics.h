#ifndef DETONATION_SHOCK_DYNAMICS
#define DETONATION_SHOCK_DYNAMICS

template<class T> class GridField;
class MACGrid;
class Vector3;
class DetonationShockDynamics{

public:
	DetonationShockDynamics();
	~DetonationShockDynamics(){};

	void Update_D(double dt, MACGrid *velocity, GridField<double> *phi);
	double getFlameFrontVelocity(int i ,int j, int k) const;
	Vector3 getFlameSpeed(int i, int j, int k, MACGrid *velocity, Vector3 &normal) const;
private:
	GridField<double> *flameFrontVelocity;
	GridField<double> *dDeriv;
	GridField<double> *curvature;
	GridField<double> *nextPhi;
	GridField<double> CalculateFlameSpeed();
	void Update_D_With_DSD(double dt, GridField<double> *phi, MACGrid *velocity);
	void Update_D_Without_DSD(GridField<double> *phi);

};

#endif //DETONATION_SHOCK_DYNAMICS
