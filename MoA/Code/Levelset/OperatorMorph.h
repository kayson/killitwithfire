/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Sderstrm (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#ifndef __operatormorph_h__
#define __operatormorph_h__

#include "Levelset/LevelSetOperator.h"

/*! \brief A level set operator that does morphing
 */
//! \lab5 Implement morphing
class OperatorMorph : public LevelSetOperator
{
protected :

  const Implicit * mTarget;

public :

  OperatorMorph(LevelSet * LS, const Implicit * target) : LevelSetOperator(LS), mTarget(target) { }

  virtual float ComputeTimestep()
  {
    // Compute and return a stable timestep
	  return 0.99;
  }

  virtual void Propagate(float time)
  {
    // Propagate level set with stable timestep dt
    // until requested time is reached
    for (float elapsed = 0; elapsed < time;) {

      // Determine timestep for stability
      float dt = ComputeTimestep();

      if (dt > time-elapsed)
        dt = time-elapsed;
      elapsed += dt;

      // Integrate level set function in time using Euler integration
      //IntegrateEuler(dt);
	  IntegrateRungeKutta(dt);
    }
  }


  virtual float Evaluate(unsigned int i, unsigned int j, unsigned int k)
  {
    // Compute the rate of change (dphi/dt)
		float x = i;
		float y = j;
		float z = k;
		mLS->TransformGridToWorld(x, y, z);

		return (mTarget->GetValue(x, y, z)-mLS->GetValue(x, y, z));
  }


};

#endif
