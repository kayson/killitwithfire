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
#ifndef __operatorreinitialize_h__
#define __operatorreinitialize_h__

#include "Levelset/LevelSetOperator.h"

/*! \brief A level set operator that re-initializes a level set towards a signed distance function.
 *
 * This class implements level set re-initialization by solving the PDE
 *
 *  \f{eqnarray*}
 *  \dfrac{\partial \phi}{\partial t} = S(\phi) (1 - |\nabla \phi|)\\
 *  S = \dfrac{1}{\sqrt{\phi^2+|\nabla \phi|^2\Delta x^2}}
 *  \f}
 *
 */
class OperatorReinitialize : public LevelSetOperator
{
public :

  OperatorReinitialize(LevelSet * LS) : LevelSetOperator(LS) { }

  virtual float ComputeTimestep()
  {
    // Compute and return a stable timestep
    return 0.5 * mLS->GetDx();
  }

  virtual void Propagate(float time)
  {
    // Determine timestep for stability
    float dt = ComputeTimestep();

    // Report maximum norm of gradient before reinitialization
    std::cerr << "Maximum gradient before reinit: " << MaxNormGradient() << std::endl;

    // Propagate level set with stable timestep dt
    // until requested time is reached
    for (float elapsed = 0; elapsed < time;) {

      GetGrid().Dilate();

      if (dt > time-elapsed)
        dt = time-elapsed;
      elapsed += dt;

      IntegrateEuler(dt);
	 // IntegrateRungeKutta(dt);

      GetGrid().Rebuild();
    }

    // Report maximum norm of gradient after reinitialization
    std::cerr << "Maximum gradient after reinit: " << MaxNormGradient() << std::endl;
  }


  virtual float Evaluate(unsigned int i, unsigned int j, unsigned int k)
  {
    // Compute the sign function (from central differencing)
    float dx = mLS->GetDx();
    float ddxc = mLS->DiffXpm(i,j,k);
    float ddyc = mLS->DiffYpm(i,j,k);
    float ddzc = mLS->DiffZpm(i,j,k);
    float normgrad2 = ddxc*ddxc + ddyc*ddyc + ddzc*ddzc;
    float val = GetGrid().GetValue(i,j,k);
    float sign = val / std::sqrt(val*val + normgrad2*dx*dx);

    // Compute upwind differences using Godunov's scheme
    float ddx2, ddy2, ddz2;
    Godunov(i,j,k, sign, ddx2, ddy2, ddz2);

    // Compute the rate of change (dphi/dt)
    return sign * (1 - std::sqrt(ddx2 + ddy2 + ddz2));
  }


protected :

  float MaxNormGradient()
  {
    float maxGrad = -(std::numeric_limits<float>::max)();
    LevelSetGrid::Iterator iter = GetGrid().BeginNarrowBand();
    LevelSetGrid::Iterator iend = GetGrid().EndNarrowBand();
    while (iter != iend) {
      unsigned int i = iter.GetI();
      unsigned int j = iter.GetJ();
      unsigned int k = iter.GetK();

      float ddxc = mLS->DiffXpm(i,j,k);
      float ddyc = mLS->DiffYpm(i,j,k);
      float ddzc = mLS->DiffZpm(i,j,k);
      float normgrad2 = ddxc*ddxc + ddyc*ddyc + ddzc*ddzc;
      if (maxGrad < normgrad2) maxGrad = normgrad2;

      iter++;
    }
   return std::sqrt(maxGrad);
  }

};

#endif
