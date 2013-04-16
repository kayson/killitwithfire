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
#ifndef __operatordilateerode_h__
#define __operatordilateerode_h__

#include "Levelset/LevelSetOperator.h"

/*! \brief A level set operator that does erosion or dilation.
 *
 * This class implements level set propagation in the normal direction
 * as defined by
 *  \f$
 *  \dfrac{\partial \phi}{\partial t}+ F(\mathbf{x})|\nabla \phi| = 0
 *  \f$
 * where the sign of F dictates erosion (c<0), or dilation (c>0).
 */
//! \lab4 Implement erosion and dilation
class OperatorDilateErode : public LevelSetOperator
{
protected :
  //! The constant speed function
  float mF;

public :

  OperatorDilateErode(LevelSet * LS, float f) : LevelSetOperator(LS), mF(f) { }

	virtual float ComputeTimestep()
	{
		// Compute and return a stable timestep
		return 0.99*mLS->GetDx()/std::abs(mF);
	}

  virtual void Propagate(float time)
  {
    // Determine timestep for stability
    float dt = ComputeTimestep();

    // Propagate level set with stable timestep dt
    // until requested time is reached
    for (float elapsed = 0; elapsed < time;) {

      if (dt > time-elapsed)
        dt = time-elapsed;
      elapsed += dt;

      // Integrate level set function in time using Euler integration
      IntegrateEuler(dt);
      //IntegrateRungeKutta(dt);
    }
  }


	virtual float Evaluate(unsigned int i, unsigned int j, unsigned int k)
	{
	// Compute the rate of change (dphi/dt)

		//-F*abs( grad f(x) )
		float ddx2, ddy2, ddz2;
		Godunov(i,j,k, mF, ddx2, ddy2, ddz2);

		float agradF = std::sqrt(ddx2 + ddy2 + ddz2);

		return -mF*agradF;
	}


};

#endif
