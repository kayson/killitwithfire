#ifndef DETONATION_SHOCK_DYNAMICS
#define DETONATION_SHOCK_DYNAMICS

template<class T> class GridField;
class LevelSet;
class MACGrid;

class DetonationShockDynamics{

 public:
  DetonationShockDynamics();
  ~DetonationShockDynamics(){};
  void Update_D_With_DSD(double dt, LevelSet *ls, MACGrid *velocity);
  void Update_D_Without_DSD(LevelSet *ls);
  void Update_D(LevelSet *ls);
  double getFlameFrontVelocity(int i ,int j, int k) const;
  double getFlameSpeed(int i, int j, int k, MACGrid *velocity, LevelSet *ls) const;
 private:
  GridField<double> *flameFrontVelocity;
  GridField<double> *dDeriv;
  GridField<double> *curvature;
  GridField<double> *nextPhi;


  GridField<double> CalculateFlameSpeed();

};

#endif //DETONATION_SHOCK_DYNAMICS
