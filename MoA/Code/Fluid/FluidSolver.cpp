
#include "FluidSolver.h"

void FluidSolver::AddSolid(Implicit * impl)
{
  mSolids.insert(impl);
  std::cerr << "'" << impl->GetName() << "' added as a solid" << std::endl;
}

void FluidSolver::AddFluid(LevelSet * LS)
{
  if (mFluids.find(LS) != mFluids.end()) {
    std::cerr << "'" << LS->GetName() << "' already added as a fluid" << std::endl;
	return;
  }
	
  mFluids.insert(LS);
  std::cerr << "'" << LS->GetName() << "' added as a fluid" << std::endl;

  // Update the bounding box (in world coordinates)
  mBox = BoxUnion(mBox, LS->GetBoundingBox());

  // Compute size of the grid (given dx)
  Vector3<float> extent = mBox.pMax - mBox.pMin;
  int dimX = (int)Round(extent[0]/mDx) + 1;
  int dimY = (int)Round(extent[1]/mDx) + 1;
  int dimZ = (int)Round(extent[2]/mDx) + 1;

  std::cerr << "Creating fluid grid of size " << dimX << " x " << dimY << " x " << dimZ << std::endl;

  // Create new velocity, voxel grid and solid mask
  mVelocityField = Volume<Vector3<float> >(dimX, dimY, dimZ);
  mVoxels = Volume<float>(dimX, dimY, dimZ);
  mSolidMask = Volume<bool>(dimX, dimY, dimZ);

  // Add the volume
  mInitialVolume += LS->ComputeVolume(LS->GetDx());
}


Vector3<float> FluidSolver::GetValue(float x, float y, float z) const
{
  // Transform to grid coordinates
  x = (x - mBox.pMin[0]) / mDx;
  y = (y - mBox.pMin[1]) / mDx;
  z = (z - mBox.pMin[2]) / mDx;

  return mVelocityField.GetValue(x,y,z);
}


Vector3<float> FluidSolver::GetMaxValue() const
{
  Vector3<float> maxVal(0.0,0.0,0.0);
  for (int i = 0; i < mVoxels.GetDimX(); i++) {
    for (int j = 0; j < mVoxels.GetDimY(); j++) {
      for (int k = 0; k < mVoxels.GetDimZ(); k++) {

        if (maxVal.Norm() < mVelocityField.GetValue(i,j,k).Norm())
          maxVal = mVelocityField.GetValue(i,j,k);
      }
    }
  }

  return maxVal;
}


int FluidSolver::Solve(float time)
{
  // Propagate the solution until requested time is reached
  int iterations = 0;
  for (float elapsed = 0; elapsed < time;) {

    // Determine timestep for stability
    float dt = ComputeTimestep();
    std::cerr << "Propagating solution with dt = " << dt << std::endl;

    if (dt > time-elapsed)
      dt = time-elapsed;
    elapsed += dt;

    // Compute current volume
    mCurrentVolume = 0;
    std::set<LevelSet *>::const_iterator iter = mFluids.begin();
    std::set<LevelSet *>::const_iterator iend = mFluids.end();
    while (iter != iend) {
      mCurrentVolume += (*iter)->ComputeVolume((*iter)->GetDx());
      iter++;
    }
    std::cout << "Current volume: " << mCurrentVolume << std::endl;
    std::cout << "Initial volume: " << mInitialVolume << std::endl;
    std::cout << "Loss of volume: " << mInitialVolume - mCurrentVolume << std::endl;


    // Classify all voxels as either solid, fluid or empty
    std::cerr << "Classifying voxels..." << std::endl;
    ClassifyVoxels();

    // Self advection
    std::cerr << "Self advection..." << std::endl;
    SelfAdvection(dt, 6);

    // Add the external forces
    std::cerr << "Adding external forces..." << std::endl;
    ExternalForces(dt);

	 // Add the friction/diffusion forces
    std::cerr << "Adding diffusion forces..." << std::endl;
	const float amount = 8;
	//Diffusion(amount,dt,0);
	//Diffusion(amount,dt,1);
	//Diffusion(amount,dt,2);

    // Enforce boundary conditions
    std::cerr << "Boundary conditions..." << std::endl;
    EnforceDirichletBoundaryCondition();

    // Compute the projection for preserving volume
    std::cerr << "Projection..." << std::endl;
    Projection();

    // The projection should not violate the boundary conditions,
    // but it might becacuse of numerical errors (the solution is not
    // exact). Enforce the boundary conditions again to safeguard
    // against this.
    std::cerr << "Boundary conditions..." << std::endl;
    EnforceDirichletBoundaryCondition();

    // Extend the velocites to "air" so the entire level set narrowband
    // is advected by the velocity field
    VelocityExtension();

    std::cerr << "Done with iteration" << std::endl;
    iterations++;
  }

  return iterations;
}


// Compute a stable timestep given the external forces
float FluidSolver::ComputeTimestep()
{
  // If there are no external forces the system is unconditionally stable
  if (mExternalForces == NULL)  return std::numeric_limits<float>::max();

  // Get the max value from the vector field
  Vector3<float> maxVal = mExternalForces->GetMaxValue();

  return 0.7*mDx/maxVal.Length();
}


float FluidSolver::ComputePotentialEnergy()
{
  return 0;
}

float FluidSolver::ComputeKineticEnergy()
{
  return 0;
}


// Add the external forces
void FluidSolver::ExternalForces(float dt)
{
  if (mExternalForces == NULL)  return;

  float x, y, z;
  for (int i = 0; i < mVoxels.GetDimX(); i++) {
    for (int j = 0; j < mVoxels.GetDimY(); j++) {
      for (int k = 0; k < mVoxels.GetDimZ(); k++) {

        // If we're in fluid (see FluidSolver::IsFluid()), sample the external force field
        // (using world coordinates, see FluidSolver::TransformGridToWorld()) and perform
        // the integration to update the velocity field (mVelocityField). The simplest
        // possible integrator is the explicit Euler.
        // TODO: Add code here
		  if(IsFluid(i,j,k))
		  {
			TransformGridToWorld(i,j,k,x,y,z);
			Vector3<float> newV = mVelocityField.GetValue(i,j,k) + dt*(mExternalForces->GetValue(x,y,z));
			mVelocityField.SetValue(i,j,k,newV);
		  }
      }
    }
  }
}

void FluidSolver::Diffusion(float amount,float dt, int axis)
{
	// Compute number of elements in the grid
	int elements = mVoxels.GetDimX()*mVoxels.GetDimY()*mVoxels.GetDimZ();

	// Create sparse matrix and guess that we have 7 non-zero elements
	// per grid point
	CoordMatrix<float, unsigned int> A(elements, elements);
	A.reserve(elements*7);
	A.beginPush();

	// Create vectors x, b in the linear system of equations Ax=b
	std::vector<float> x(elements, 0), b(elements, 0);

	float scale = amount*dt;

	std::cerr << "Building A matrix and b vector..." << std::endl;
	for (int i = 0; i < mVoxels.GetDimX(); i++) {
		for (int j = 0; j < mVoxels.GetDimY(); j++) {
			for (int k = 0; k < mVoxels.GetDimZ(); k++) {

				// If we're in fluid...
				if (IsFluid(i,j,k)) {

					unsigned int ind = mVoxels.ComputeLinearIndex(i,j,k);
					unsigned int ind_ip = mVoxels.ComputeLinearIndex(i+1,j,k);
					unsigned int ind_im = mVoxels.ComputeLinearIndex(i-1,j,k);
					unsigned int ind_jp = mVoxels.ComputeLinearIndex(i,j+1,k);
					unsigned int ind_jm = mVoxels.ComputeLinearIndex(i,j-1,k);
					unsigned int ind_kp = mVoxels.ComputeLinearIndex(i,j,k+1);
					unsigned int ind_km = mVoxels.ComputeLinearIndex(i,j,k-1);

					//x
					if (IsFluid(i-1,j,k))
						A(ind,ind_im) = -scale;
					if (IsFluid(i+1,j,k))
						A(ind,ind_ip) = -scale;
					//y
					if (IsFluid(i,j-1,k))
						A(ind,ind_jm) = -scale;

					if (IsFluid(i,j+1,k))
						A(ind,ind_jp) = -scale;
					//z
					if (IsFluid(i,j,k-1))
						A(ind,ind_km) = -scale;
					if (IsFluid(i,j,k+1))
						A(ind,ind_kp) = -scale;

					A(ind,ind) = 1.0 + 6.0*scale;
					b.at(ind) = mVelocityField.GetValue(i,j,k)[axis];
				}
			}
		}
	}

	// Rebuild the sparse matrix structure
	A.endPush();

	// Solve Ax=b using conjugate gradient
	std::cerr << "Conjugate gradient solver... ";
	ConjugateGradient<CoordMatrix<float, unsigned int>, std::vector<float>, float> CG(100, 1e-3);
	CG.solve(A,x,b);
	std::cerr << "finished with tolerance " << CG.getTolerance() << " in " << CG.getNumIter() << " iterations" << std::endl;

	// Subtract the gradient of x to preserve the volume
	for (int i = 0; i < mVoxels.GetDimX(); i++) {
		for (int j = 0; j < mVoxels.GetDimY(); j++) {
			for (int k = 0; k < mVoxels.GetDimZ(); k++) {

				// If we're in fluid...
				if (IsFluid(i,j,k)) {

					// Compute the linear indices of (i,j,k) and its neighbors
					unsigned int ind = mVoxels.ComputeLinearIndex(i,j,k);

					Vector3<float> v = mVelocityField.GetValue(i,j,k);
					v[axis] = x.at(ind);
					mVelocityField.SetValue(i,j,k,v);
				}
			}
		}
	}
}


// Compute the self advection term
void FluidSolver::SelfAdvection(float dt, int steps)
{
  // Copy the current velocity field
  Volume<Vector3<float> > velocities = mVelocityField;

  for (int i = 0; i < mVoxels.GetDimX(); i++) {
    for (int j = 0; j < mVoxels.GetDimY(); j++) {
      for (int k = 0; k < mVoxels.GetDimZ(); k++) {

        // If we're in fluid, sample the current velocity field at (i,j,k).
        // Then, trace a particle at initial position (i,j,k) back in time through
        // the velocity field using 'steps' number of steps. Note that each
        // step is dt/steps time units long. Note also that the velocities are
        // given in world space, but you perform the trace in grid space so you
        // need to scale the velocities accordingly (grid spacing: mDx). When you
        // trace the particle you interpolate the velocities inbetween the grid
        // points, use mVelocityField.GetValue(float i, float j, float k) for trilinear
        // interpolation.
        // TODO: Add code here
		if (IsFluid(i,j,k))
		{
			Vector3<float> vel = mVelocityField.GetValue(i,j,k);
			Vector3<float> pos(i,j,k);
			for (int n = 0; n < steps; ++n)
			{
				pos = pos - dt/(float)steps * vel/mDx;//??
				vel = mVelocityField.GetValue(pos[0],pos[1],pos[2]);
			}
			velocities.SetValue(i,j,k,vel);
		}
      }
    }
  }

  // Update the current velocity field
  mVelocityField = velocities;
}


// Enforce the Dirichlet boundary conditions
void FluidSolver::EnforceDirichletBoundaryCondition()
{
	for (int i = 0; i < mVoxels.GetDimX(); i++) 
	{
		for (int j = 0; j < mVoxels.GetDimY(); j++) 
		{
			for (int k = 0; k < mVoxels.GetDimZ(); k++) 
			{
				// If we're in fluid, check the neighbors of (i,j,k) to
				// see if it's next to a solid boundary. If so, project
				// the velocity to the boundary plane by setting the
				// velocity to zero along the given dimension.
				// TODO: Add code here
				if(IsFluid(i,j,k))
				{
					Vector3<float> V = mVelocityField.GetValue(i,j,k);
					
					if((IsSolid(i-1,j,k) && V[0] < 0.0f) || (IsSolid(i+1,j,k) && V[0] > 0.0f))
						V[0] = 0;
					if((IsSolid(i,j-1,k) && V[1] < 0.0f) || (IsSolid(i,j+1,k) && V[1] > 0.0f))
						V[1] = 0;
					if((IsSolid(i,j,k-1) && V[2] < 0.0f) || (IsSolid(i,j,k+1) && V[2] > 0.0f))
						V[2] = 0;

					mVelocityField.SetValue(i,j,k,V);
				}
			}
		}
	}
}


// Project the velocity field to preserve the volume
void FluidSolver::Projection()
{

  // Compute number of elements in the grid
  int elements = mVoxels.GetDimX()*mVoxels.GetDimY()*mVoxels.GetDimZ();

  // Create sparse matrix and guess that we have 7 non-zero elements
  // per grid point
  CoordMatrix<float, unsigned int> A(elements, elements);
  A.reserve(elements*7);
  A.beginPush();

  // Create vectors x, b in the linear system of equations Ax=b
  std::vector<float> x(elements, 0), b(elements, 0);

  float dx2 = mDx*mDx;
  float i_dx2 = 1.0/dx2;
  float dt = ComputeTimestep();
  int count = 0;

  std::cerr << "Building A matrix and b vector..." << std::endl;
  for (int i = 0; i < mVoxels.GetDimX(); i++) {
    for (int j = 0; j < mVoxels.GetDimY(); j++) {
      for (int k = 0; k < mVoxels.GetDimZ(); k++) {

        // If we're in fluid...
        if (IsFluid(i,j,k)) {

          // Compute the linear indices of (i,j,k) and its neighbors
          // (you need these to index into the A matrix and x,b vectors)
          unsigned int ind = mVoxels.ComputeLinearIndex(i,j,k);
          unsigned int ind_ip = mVoxels.ComputeLinearIndex(i+1,j,k);
          unsigned int ind_im = mVoxels.ComputeLinearIndex(i-1,j,k);
          unsigned int ind_jp = mVoxels.ComputeLinearIndex(i,j+1,k);
          unsigned int ind_jm = mVoxels.ComputeLinearIndex(i,j-1,k);
          unsigned int ind_kp = mVoxels.ComputeLinearIndex(i,j,k+1);
          unsigned int ind_km = mVoxels.ComputeLinearIndex(i,j,k-1);

          // Compute entry for b vector (divergence of the velocity field: \nabla \dot w_i,j,k)
          // TODO: Add code here

          // Compute entries for A matrix (discrete Laplacian operator).
          // The A matrix is a sparse matrix but can be used like a regular
          // matrix. That is, you access the elements by A(row, column).
          // However, due to the matrix data structure you cannot read
          // elements at this point (until you do A.endPush(), see below).
          // So, only use A(row, column) = ... to set a value in the matrix,
          // don't use A(row, column) to get a value.
          // Remember to enforce the boundary conditions if we're next to
          // a solid (allow no change of flow in that direction).
          // Remember to treat the boundaries of (i,j,k).
          // TODO: Add code here
		  int nonSolid = 0;
		  float div_V = 0.0;

		  if(!IsSolid(i+1,j,k))
		  {
			  A(ind, ind_ip) = i_dx2;
			  ++nonSolid;
			  div_V += mVelocityField.GetValue(i+1,j,k)[0];
		  }
		  if(!IsSolid(i-1,j,k))
		  {
			  A(ind, ind_im) = i_dx2;
			  ++nonSolid;
			  div_V -= mVelocityField.GetValue(i-1,j,k)[0];
		  }
		  if(!IsSolid(i,j+1,k))
		  {
			  A(ind, ind_jp) = i_dx2;
			  ++nonSolid;
			  div_V += mVelocityField.GetValue(i,j+1,k)[1];
		  }
		  if(!IsSolid(i,j-1,k))
		  {
			  A(ind, ind_jm) = i_dx2;
			  ++nonSolid;
			   div_V -= mVelocityField.GetValue(i,j-1,k)[1];
		  }
		  if(!IsSolid(i,j,k+1))
		  {
			  A(ind, ind_kp) = i_dx2;
			  ++nonSolid;
			 div_V += mVelocityField.GetValue(i,j,k+1)[2];
		  }
		  if(!IsSolid(i,j,k-1))
		  {
			  A(ind, ind_km) = i_dx2;
			  ++nonSolid;
			  div_V -= mVelocityField.GetValue(i,j,k-1)[2];
		  }

		  A(ind, ind) = -nonSolid*i_dx2;
		  
		  div_V /= 2.0*mDx;
		  b.at(ind) = div_V;
		  ++count;
        }
      }
    }
  }
	
  //Fixa volymförlusten
    for (int i = 0; i < mVoxels.GetDimX(); i++) {
    for (int j = 0; j < mVoxels.GetDimY(); j++) {
      for (int k = 0; k < mVoxels.GetDimZ(); k++) {
        // If we're in fluid...
        if (IsFluid(i,j,k)) 
		{
			 unsigned int ind = mVoxels.ComputeLinearIndex(i,j,k);
			 float volume_loss = (mInitialVolume - mCurrentVolume)/((float)count*dt);
			 //float volume_loss = (mInitialVolume - mCurrentVolume)/((float)count);
			 b.at(ind) -= volume_loss;
        }
      }
    }
  }

  // Rebuild the sparse matrix structure
  A.endPush();

  // Solve Ax=b using conjugate gradient
  std::cerr << "Conjugate gradient solver... ";
  ConjugateGradient<CoordMatrix<float, unsigned int>, std::vector<float>, float> CG(100, 1e-3);
  CG.solve(A,x,b);
  std::cerr << "finished with tolerance " << CG.getTolerance() << " in " << CG.getNumIter() << " iterations" << std::endl;

  // Subtract the gradient of x to preserve the volume
  for (int i = 0; i < mVoxels.GetDimX(); i++) {
    for (int j = 0; j < mVoxels.GetDimY(); j++) {
      for (int k = 0; k < mVoxels.GetDimZ(); k++) {

        // If we're in fluid...
        if (IsFluid(i,j,k)) {

          // Compute the linear indices of (i,j,k) and its neighbors
          unsigned int ind_ip = mVoxels.ComputeLinearIndex(i+1,j,k);
          unsigned int ind_im = mVoxels.ComputeLinearIndex(i-1,j,k);
          unsigned int ind_jp = mVoxels.ComputeLinearIndex(i,j+1,k);
          unsigned int ind_jm = mVoxels.ComputeLinearIndex(i,j-1,k);
          unsigned int ind_kp = mVoxels.ComputeLinearIndex(i,j,k+1);
          unsigned int ind_km = mVoxels.ComputeLinearIndex(i,j,k-1);

          // Compute the gradient of x at (i,j,k) using central differencing
          // and subtract this gradient from the velocity field.
          // Thereby removing divergence - preserving volume.
          // TODO: Add code here
		  float qip = x.at(ind_ip); 
		  float qim = x.at(ind_im);
		  float qjp = x.at(ind_jp); 
		  float qjm = x.at(ind_jm);
		  float qkp = x.at(ind_kp); 
		  float qkm = x.at(ind_km);
		  Vector3<float> grad_q = (1.0/(2.0*mDx))*Vector3<float>(qip - qim, qjp - qjm, qkp - qkm);
		  mVelocityField.SetValue(i,j,k,mVelocityField.GetValue(i,j,k) - grad_q);
        }
      }
    }
  }
}


// Extent the velocities to "air"
void FluidSolver::VelocityExtension()
{
  int iterations = 0;
  float narrowbandWidth = 0;
  float dt = 0.7;
  std::set<LevelSet *>::const_iterator iter = mFluids.begin();
  std::set<LevelSet *>::const_iterator iend = mFluids.end();
  while (iter != iend) {
    iterations = std::max(iterations, (int)((*iter)->GetNarrowBandWidth()*0.5f/dt));
    narrowbandWidth = std::max(narrowbandWidth, (*iter)->GetNarrowBandWidth()*(*iter)->GetDx()*0.5f);
    iter++;
  }

  iterations = std::min(iterations, (int)(mVoxels.GetDimX()/dt));
  iterations = std::min(iterations, (int)(mVoxels.GetDimY()/dt));
  iterations = std::min(iterations, (int)(mVoxels.GetDimZ()/dt));

  std::cerr << "Velocity extension (" << iterations << " iterations)..." << std::endl;
  for (int iter = 0; iter < iterations; iter++) {

    Volume<Vector3<float> > velocities = mVelocityField;

    for (int i = 0; i < mVoxels.GetDimX(); i++) {
      for (int j = 0; j < mVoxels.GetDimY(); j++) {
        for (int k = 0; k < mVoxels.GetDimZ(); k++) {

          const float val = mVoxels.GetValue(i,j,k);

          // Extend only in air or solid (don't extend beyond narrowband for efficiency)
          if (IsFluid(i,j,k) || val >= narrowbandWidth) continue;

          Vector3<float> normal(mVoxels.GetValue(i+1,j,k) - mVoxels.GetValue(i-1,j,k),
                                mVoxels.GetValue(i,j+1,k) - mVoxels.GetValue(i,j-1,k),
                                mVoxels.GetValue(i,j,k+1) - mVoxels.GetValue(i,j,k-1));

          if (normal.Length() > 0)
            normal.Normalize();

          Vector3<float> v = mVelocityField.GetValue(i,j,k);

          Vector3<float> ip = mVelocityField.GetValue(i+1,j,k);
          Vector3<float> im = mVelocityField.GetValue(i-1,j,k);

          Vector3<float> jp = mVelocityField.GetValue(i,j+1,k);
          Vector3<float> jm = mVelocityField.GetValue(i,j-1,k);

          Vector3<float> kp = mVelocityField.GetValue(i,j,k+1);
          Vector3<float> km = mVelocityField.GetValue(i,j,k-1);

          Vector3<float> v0 =  v - dt*(std::max(0.0f, normal[0])*(v - im) + std::min(0.0f, normal[0])*(ip - v) +
                                       std::max(0.0f, normal[1])*(v - jm) + std::min(0.0f, normal[1])*(jp - v) +
                                       std::max(0.0f, normal[2])*(v - km) + std::min(0.0f, normal[2])*(kp - v));

          velocities.SetValue(i,j,k, v0);
        }
      }
    }

    mVelocityField = velocities;
  }
}



void FluidSolver::ClassifyVoxels()
{
  // Iterate the domain and classify all voxels as either
  // fluid, solid or empty
  for (int i = 0; i < mVoxels.GetDimX(); i++) {
    for (int j = 0; j < mVoxels.GetDimY(); j++) {
      for (int k = 0; k < mVoxels.GetDimZ(); k++) {

        ClassifyVoxel(i,j,k);
      }
    }
  }
}


void FluidSolver::ClassifyVoxel(int i, int j, int k)
{
  // Transform grid to world coordinates
  float x,y,z;
  TransformGridToWorld(i,j,k, x,y,z);

  // Check if we're inside a solid and update the solid mask
  bool solid = false;
  std::set<Implicit *>::const_iterator iterSolid = mSolids.begin();
  std::set<Implicit *>::const_iterator iendSolid = mSolids.end();
  while (iterSolid != iendSolid && !solid) {
    if ((*iterSolid)->GetValue(x,y,z) <= 0)  solid = true;
    iterSolid++;
  }
  mSolidMask.SetValue(i,j,k, solid);

  // Compute the closest distance to all fluids
  float distance = std::numeric_limits<float>::max();
  std::set<LevelSet *>::const_iterator iterFluid = mFluids.begin();
  std::set<LevelSet *>::const_iterator iendFluid = mFluids.end();
  while (iterFluid != iendFluid) {
    distance = std::min(distance, (*iterFluid)->GetValue(x,y,z));
    iterFluid++;
  }
  mVoxels.SetValue(i,j,k, distance);
}


bool FluidSolver::IsSolid(int i, int j, int k) const
{
  return mSolidMask.GetValue(i,j,k);
}

bool FluidSolver::IsFluid(int i, int j, int k) const
{
  return mVoxels.GetValue(i,j,k) <= 0.5*mDx;
}
