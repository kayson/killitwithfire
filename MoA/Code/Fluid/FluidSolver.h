
#ifndef _FLUID_SOLVER_H
#define _FLUID_SOLVER_H

#include "Math/Function3D.h"
#include "Math/Volume.h"
#include "Math/CoordMatrix.h"
#include "Geometry/Implicit.h"
#include "Levelset/LevelSet.h"
#include "Util/Util.h"
#include <set>
#include <cassert>

class FluidSolver : public Function3D<Vector3<float> >
{
public :

	friend class FluidVoxelCutPlane;

  FluidSolver(float dx) : mDx(dx), mInitialVolume(0), mCurrentVolume(0), mExternalForces(NULL) { }

  void AddSolid(Implicit * impl);
  void AddFluid(LevelSet * LS);

  std::set<Implicit *> & GetSolids() { return mSolids; }
  std::set<LevelSet *> & GetFluids() { return mFluids; }

  void SetExternalForces(Function3D<Vector3<float> > * forces) { mExternalForces = forces; }

  int Solve(float time);

  const Bbox & GetBoundingBox() const { return mBox; }

  float ComputeTimestep();

  float ComputePotentialEnergy();

  float ComputeKineticEnergy();

  virtual Vector3<float> GetValue(float x, float y, float z) const;

  virtual Vector3<float> GetMaxValue() const;

protected :

  std::set<Implicit *> mSolids;
  std::set<LevelSet *> mFluids;

  Bbox mBox;
  float mDx;
  float mInitialVolume, mCurrentVolume;

  Volume<Vector3<float> > mVelocityField;
  Volume<float> mVoxels;
  Volume<bool> mSolidMask;
  Function3D<Vector3<float> > * mExternalForces;

  void ExternalForces(float dt);
  void Diffusion(float amount,float dt, int axis);
  void SelfAdvection(float dt, int steps);
  void EnforceDirichletBoundaryCondition();
  void Projection();
  void VelocityExtension();

  bool IsSolid(int i, int j, int k) const;
  bool IsFluid(int i, int j, int k) const;

  void ClassifyVoxels();
  void ClassifyVoxel(int i, int j, int k);

  inline void TransformGridToWorld(int i, int j, int k, float & x, float & y, float & z) {
    x = mBox.pMin[0] + i*mDx;
    y = mBox.pMin[1] + j*mDx;
    z = mBox.pMin[2] + k*mDx;
  }

};


/*
 * Below follows a set of operators used in the conjugate gradient solver
 */

template <typename Real>
Real dot(const std::vector<Real> & v1, const std::vector<Real> & v2)
{
  assert(v1.size() == v2.size());

  Real sum = 0;
  const unsigned int size = v1.size();
  for (unsigned int i = 0; i < size; i++)
    sum += v1[i]*v2[i];

  return sum;
}

template <typename Real>
Real norm(const std::vector<Real> & v)
{
	return std::sqrt(dot(v,v));
}

template <typename Real>
std::vector<Real> operator - (const std::vector<Real> & v1, const std::vector<Real> & v2)
{
  assert(v1.size() == v2.size());

  const unsigned int size = v1.size();
  std::vector<Real> v(size);
  for (unsigned int i = 0; i < size; i++)
    v[i] = v1[i] - v2[i];

  return v;
}

template <typename Real>
std::vector<Real> operator + (const std::vector<Real> & v1, const std::vector<Real> & v2)
{
  assert(v1.size() == v2.size());

  const unsigned int size = v1.size();
  std::vector<Real> v(size);
  for (unsigned int i = 0; i < size; i++)
    v[i] = v1[i] + v2[i];

  return v;
}

template <typename Real>
std::vector<Real> operator * (Real r, const std::vector<Real> & v1)
{
  const unsigned int size = v1.size();
  std::vector<Real> v(size);
  for (unsigned int i = 0; i < size; i++)
    v[i] = v1[i]*r;

  return v;
}

#include "Math/ConjugateGradient.h"


#endif
