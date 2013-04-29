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
#ifndef __operatormeancurvatureflow_h__
#define __operatormeancurvatureflow_h__

#include "Levelset/LevelSetOperator.h"

/*! \brief A level set operator that does mean curvature flow.
 *
 * This class implements level set propagation in the normal direction
 * as defined by the mean curvature flow \f$\kappa\f$ in the following PDE
 *
 *  \f[
 *  \dfrac{\partial \phi}{\partial t} + \alpha \kappa|\nabla \phi| = 0
 *  \f]
 */
//! \lab4 Implement mean curvature flow
class OperatorMeanCurvatureFlow : public LevelSetOperator
{
protected:
  //! Scaling parameter, affects time step constraint
  float mAlpha;
public :

  OperatorMeanCurvatureFlow(LevelSet * LS, float alpha=.9f)
    : LevelSetOperator(LS), mAlpha(alpha) { }

	virtual float ComputeTimestep()
	{
		return 0.99*mLS->GetDx()*mLS->GetDx()/(6.0*std::abs(mAlpha));
	}

  virtual void Propagate(float time)
  {
    // Determine timestep for stability
	  std::cout << "Volume Before = " << mLS->ComputeVolume() << std::endl;

    float dt = ComputeTimestep();

	float c = 0.1;
    // Propagate level set with stable timestep dt
    // until requested time is reached
    for (float elapsed = 0; elapsed < time;) {

      if (dt > time-elapsed)
        dt = time-elapsed;
      elapsed += dt;


      //IntegrateEuler(dt);
      IntegrateRungeKutta(dt);
    }
	std::cout << "Volume After = " << mLS->ComputeVolume() << std::endl;
  }


	virtual float Evaluate(unsigned int i, unsigned int j, unsigned int k)
	{
		// Compute the rate of change (dphi/dt)
		float dx = mLS->DiffXpm(i,j,k),
		dy = mLS->DiffYpm(i,j,k),
		dz = mLS->DiffZpm(i,j,k),
		dxx = mLS->Diff2Xpm(i,j,k),
		dyy = mLS->Diff2Ypm(i,j,k),
		dzz = mLS->Diff2Zpm(i,j,k),
		dxy = mLS->Diff2XYpm(i,j,k),
		dyz = mLS->Diff2YZpm(i,j,k),
		dxz = mLS->Diff2ZXpm(i,j,k);
		float curv = ( dx*dx*(dyy+dzz) - 2 *dy*dz*dyz + dy*dy*(dxx+dzz) - 2*dx*dz*dxz + dz*dz*(dxx + dyy) - 2 * dx*dy*dxy  ) / (2*powf(dx*dx+dy*dy+dz*dz,1.5));
		
		float ddx2, ddy2, ddz2;
		Godunov(i,j,k, curv, ddx2, ddy2, ddz2);
		//float normgrad = sqrt(ddx2+ddy2+ddz2);
		float normgrad = sqrt(dx*dx+dy*dy+dz*dz);
		return mAlpha*curv*normgrad;
	}


};

#endif
