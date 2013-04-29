#ifndef __FLUID_VOXEL_CUT_PLANE_H__
#define __FLUID_VOXEL_CUT_PLANE_H__

#include "Geometry/Geometry.h"
#include "Fluid/FluidSolver.h"
#include "Util/ColorMap.h"
#include "Math/Vector3.h"

#include <vector>

class FluidVoxelCutPlane:	public Geometry
{
public:

	FluidVoxelCutPlane(const std::string & name, const FluidSolver* solver);

	virtual const char * GetTypeName() { return typeid(FluidVoxelCutPlane).name(); }

  virtual void Render();

  virtual void Initialize() { }

  virtual void Update();

  virtual void SetTransform(const Matrix4x4<float> & transform);

protected :

	const FluidSolver* mSolver;

	float mDx;

	std::vector< bool > mFluidVoxels;
	std::vector< Vector3<float> > mFluidVoxelsPositions;
};

#endif
