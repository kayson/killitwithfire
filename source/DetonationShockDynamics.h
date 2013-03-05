#ifndef DETONATION_SHOCK_DYNAMICS
#define DETONATION_SHOCK_DYNAMICS

template<class T> GridField;

class DetonationShockDynamics{

 public:
  DetonationShockDynamics();
  ~DetonationShockDynamics(){};
  void Update_D_With_DSD();
  void Update_D_Without_DSD();
  void Update_D();
  double getFlameFrontVelocity(int i ,int j, int k) const;
  double getFlameSpeed(int i, int j, int k) const;
 private:
  GridField<double> *flameFrontVelocity;
  GridField<double> CalculateFlameSpeed();

};

#endif //DETONATION_SHOCK_DYNAMICS
