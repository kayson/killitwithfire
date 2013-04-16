/*************************************************************************************************
*
* Modeling and animation (TNM079) 2007
* Code base for lab assignments. Copyright:
*   Gunnar Johansson (gunnar.johansson@itn.liu.se)
*   Ken Museth (ken.museth@itn.liu.se)
*   Michael Bang Nielsen (bang@daimi.au.dk)
*   Ola Nilsson (ola.nilsson@itn.liu.se)
*   Andreas Sˆderstrˆm (andreas.soderstrom@itn.liu.se)
*
*************************************************************************************************/
#ifndef __levelset_operator_h__
#define __levelset_operator_h__

#include "Levelset/LevelSet.h"

class LevelSetOperator
{
protected :
  LevelSet * mLS;

  //! Exposes access to the level set grid internally (used in derived classes)
  inline LevelSetGrid & GetGrid() { return mLS->mGrid; }
  inline const LevelSetGrid & GetGrid() const { return mLS->mGrid; }

  //! Computes the squares of the gradients using Godunov's method
  void Godunov(unsigned int i, unsigned int j, unsigned int k, float a,
               float & ddx2, float & ddy2, float & ddz2);

  void IntegrateEuler(float dt);
  void IntegrateRungeKutta(float dt);

public :

  LevelSetOperator(LevelSet * LS) : mLS(LS) { }
  virtual ~LevelSetOperator() {}
  virtual float ComputeTimestep() { return 0; }
  virtual void Propagate(float time) = 0;
  virtual float Evaluate(unsigned int i, unsigned int j, unsigned int k) { return 0; }
};


#endif

