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

#include "LevelSet.h"
#include "Util/Util.h"


const LevelSet::VisualizationMode LevelSet::NarrowBand = NewVisualizationMode("Narrowband");

LevelSet::LevelSet(float dx) : mDx(dx)
{
}


LevelSet::LevelSet(float dx, const Implicit & impl) : mDx(dx)
{
  // Get the bounding box (in world space) from the implicit
  // function to initialize the level set's bounding box.
  Bbox b = impl.GetBoundingBox();
  SetBoundingBox(b);

  // Loop over volume and sample the implicit function
  int i = 0, j = 0, k = 0;
  for(float x = mBox.pMin[0]; x < mBox.pMax[0]+0.5*mDx; x += mDx, i++) {
    for(float y = mBox.pMin[1]; y < mBox.pMax[1]+0.5*mDx; y += mDx, j++) {
      for(float z = mBox.pMin[2]; z < mBox.pMax[2]+0.5*mDx; z += mDx, k++ ) {
        mGrid.SetValue(i,j,k, impl.GetValue(x,y,z));
      }
      k = 0;
    }
    j = 0;
  }
}

/*! Assigns the level-set from a volume. Sets the dimensions of the bounding box to match
 * the dimensions of the grid scaled by dx
 */
LevelSet::LevelSet(float dx, const Volume<float> & vol) : mDx(dx)
{
  // Get the bounding box (in world space) from the size of the grid
  Bbox b(Vector3<float>(0,0,0), Vector3<float>((vol.GetDimX()-1)*mDx, (vol.GetDimY()-1)*mDx, (vol.GetDimZ()-1)*mDx));
  SetBoundingBox(b);

  // Loop over volume and copy the volume
  for(int i=0; i< vol.GetDimX(); i++){
    for(int j=0; j< vol.GetDimY(); j++){
      for(int k=0; k< vol.GetDimZ(); k++){
        mGrid.SetValue(i, j, k, vol.GetValue(i,j,k)*mDx);
      }
    }
  }
}


LevelSet::LevelSet(float dx, const Implicit & impl, const Bbox & box) : mDx(dx)
{
  SetBoundingBox(box);

  // Loop over volume and sample the implicit function
  int i = 0, j = 0, k = 0;
  for(float x = mBox.pMin[0]; x < mBox.pMax[0]+0.5*mDx; x += mDx, i++) {
    for(float y = mBox.pMin[1]; y < mBox.pMax[1]+0.5*mDx; y += mDx, j++) {
      for(float z = mBox.pMin[2]; z < mBox.pMax[2]+0.5*mDx; z += mDx, k++ ) {
        mGrid.SetValue(i,j,k, impl.GetValue(x,y,z));
      }
      k = 0;
    }
    j = 0;
  }
}

float LevelSet::GetValue(float x, float y, float z) const
{
  // We use the convention that world coordinates are represented by (x,y,z)
  // while grid coordinates are written as (i,j,k)
  TransformWorldToGrid(x,y,z);

  int i = (int)x;
  int j = (int)y;
  int k = (int)z;

  float bx = x - i;
  float by = y - j;
  float bz = z - k;

  i = Clamp(i, 0, mGrid.GetDimX()-1);
  j = Clamp(j, 0, mGrid.GetDimY()-1);
  k = Clamp(k, 0, mGrid.GetDimZ()-1);

  bx = Clamp(bx, 0.0f, 1.0f);
  by = Clamp(by, 0.0f, 1.0f);
  bz = Clamp(bz, 0.0f, 1.0f);

  if (i == mGrid.GetDimX()-1) {
    i--;
    bx = 1;
  }
  if (j == mGrid.GetDimY()-1) {
    j--;
    by = 1;
  }
  if (k == mGrid.GetDimZ()-1) {
    k--;
    bz = 1;
  }

  float val =
  mGrid.GetValue(i,   j,   k  ) * (1-bx) * (1-by) * (1-bz) +
  mGrid.GetValue(i+1, j,   k  ) *  bx    * (1-by) * (1-bz) +
  mGrid.GetValue(i+1, j+1, k  ) *  bx    *  by    * (1-bz) +
  mGrid.GetValue(i,   j+1, k  ) * (1-bx) *  by    * (1-bz) +
  mGrid.GetValue(i,   j,   k+1) * (1-bx) * (1-by) *  bz    +
  mGrid.GetValue(i+1, j,   k+1) *  bx    * (1-by) *  bz    +
  mGrid.GetValue(i+1, j+1, k+1) *  bx    *  by    *  bz    +
  mGrid.GetValue(i,   j+1, k+1) * (1-bx) *  by    *  bz;

  return val;
}


/*!
 * Evaluates gradient at (x,y,z) through discrete finite difference scheme.
 */
Vector3<float> LevelSet::GetGradient(float x, float y, float z) const
{
	//return Implicit::GetGradient(x, y, z);
	
	float i = x;
    float j = y;
    float k = z;
	TransformWorldToGrid(i, j, k);

	float Dx = DiffXpm(i,j,k);
	float Dy = DiffYpm(i,j,k);
	float Dz = DiffZpm(i,j,k);

	return Vector3<float>(Dx, Dy, Dz);
}


/*!
 * Evaluates curvature at (x,y,z) through discrete finite difference scheme.
 */
float LevelSet::GetCurvature(float x, float y, float z) const
{
	//return Implicit::GetCurvature(x, y, z);

	float i = x;
    float j = y;
    float k = z;
	TransformWorldToGrid(i, j, k);

	float dx = DiffXpm(i,j,k),
	dy = DiffYpm(i,j,k),
	dz = DiffZpm(i,j,k),
	dxx = Diff2Xpm(i,j,k),
	dyy = Diff2Ypm(i,j,k),
	dzz = Diff2Zpm(i,j,k),
	dxy = Diff2XYpm(i,j,k),
	dyz = Diff2YZpm(i,j,k),
	dxz = Diff2ZXpm(i,j,k);
	return ( dx*dx*(dyy+dzz) - 2 *dy*dz*dyz + dy*dy*(dxx+dzz) - 2*dx*dz*dxz + dz*dz*(dxx + dyy) - 2 * dx*dy*dxy  ) / (2*powf(dx*dx+dy*dy+dz*dz,1.5));    
}

void LevelSet::SetBoundingBox(const Bbox & b)
{
  // Loop over existing grid to find the maximum and minimum values
  // stored. These are used to initialize the new grid with decent values.
  LevelSetGrid::Iterator iter = mGrid.BeginNarrowBand();
  LevelSetGrid::Iterator iend = mGrid.EndNarrowBand();
  float maxVal = -(std::numeric_limits<float>::max)();
  float minVal = (std::numeric_limits<float>::max)();
  while (iter != iend) {
    int i = iter.GetI();
    int j = iter.GetJ();
    int k = iter.GetK();

    float val = mGrid.GetValue(i,j,k);
    if (maxVal < val)  maxVal = val;
    if (minVal > val)  minVal = val;
    iter++;
  }

  // Create a new grid with requested size
  Vector3<float> extent = b.pMax - b.pMin;
  int dimX = (int)Round(extent[0]/mDx) + 1;
  int dimY = (int)Round(extent[1]/mDx) + 1;
  int dimZ = (int)Round(extent[2]/mDx) + 1;
  LevelSetGrid grid(dimX, dimY, dimZ, minVal, maxVal);

  // Copy all old values to new grid
  iter = mGrid.BeginNarrowBand();
  while (iter != iend) {
    int i = iter.GetI();
    int j = iter.GetJ();
    int k = iter.GetK();

    // Get the (x,y,z) coordinates of grid point (i,j,k)
    float x = i*mDx + mBox.pMin[0];
    float y = j*mDx + mBox.pMin[1];
    float z = k*mDx + mBox.pMin[2];

    // Check that (x,y,z) is inside the new bounding box
    if (x < b.pMin[0] || x > b.pMax[0] ||
        y < b.pMin[1] || y > b.pMax[1] ||
        z < b.pMin[2] || z > b.pMax[2]) {
      iter++;
      continue;
    }

    // Compute the new grid point (l,m,n)
    int l = (int)Round((x - b.pMin[0]) / mDx);
    int m = (int)Round((y - b.pMin[1]) / mDx);
    int n = (int)Round((z - b.pMin[2]) / mDx);

    grid.SetValue(l,m,n, mGrid.GetValue(i,j,k));
    iter++;
  }

  // Set inside and outside constants
  grid.SetInsideConstant( mGrid.GetInsideConstant() );
  grid.SetOutsideConstant( mGrid.GetOutsideConstant() );

  // Set the new bounding box
  Implicit::SetBoundingBox(b);

  // Reassign the new grid
  mGrid = grid;

  std::cerr << "Level set created with grid size: " << mGrid.GetDimensions() << std::endl;
}


void LevelSet::SetNarrowBandWidth(int width)
{
  mGrid.SetInsideConstant(-width*0.5*mDx);
  mGrid.SetOutsideConstant(width*0.5*mDx);
  mGrid.Rebuild();
}

int LevelSet::GetNarrowBandWidth()
{
  float width = mGrid.GetOutsideConstant() - mGrid.GetInsideConstant();
  return width/mDx;
}


//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffXm(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j, k) - mGrid.GetValue(i-1, j, k))/mDx;
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffXp(int i, int j, int k) const
{
	return (mGrid.GetValue(i+1, j, k) - mGrid.GetValue(i, j, k))/mDx;
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffXpm(int i, int j, int k) const
{
	return (mGrid.GetValue(i+1, j, k) - mGrid.GetValue(i-1, j, k))/(2.0*mDx);
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::Diff2Xpm(int i, int j, int k) const
{
	return (mGrid.GetValue(i+1, j, k) - 2.0*mGrid.GetValue(i, j, k) + mGrid.GetValue(i-1, j, k))/(mDx*mDx);
}


//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffYm(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j, k) - mGrid.GetValue(i, j-1, k))/mDx;
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffYp(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j+1, k) - mGrid.GetValue(i, j, k))/mDx;
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffYpm(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j+1, k) - mGrid.GetValue(i, j-1, k))/(2.0*mDx);
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::Diff2Ypm(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j+1, k) - 2.0*mGrid.GetValue(i, j, k) + mGrid.GetValue(i, j-1, k))/(mDx*mDx);
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffZm(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j, k) - mGrid.GetValue(i, j, k-1))/mDx;
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffZp(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j, k+1) - mGrid.GetValue(i, j, k))/mDx;
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::DiffZpm(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j, k+1) - mGrid.GetValue(i, j, k-1))/(2.0*mDx);
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::Diff2Zpm(int i, int j, int k) const
{
	return (mGrid.GetValue(i, j, k+1) - 2.0*mGrid.GetValue(i, j, k) + mGrid.GetValue(i, j, k-1))/(mDx*mDx);
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::Diff2XYpm(int i, int j, int k) const
{
	return (  mGrid.GetValue(i+1, j+1, k) - mGrid.GetValue(i+1, j-1, k) 
			+ mGrid.GetValue(i-1, j-1, k) - mGrid.GetValue(i-1, j+1, k) )
			/ (4.0*mDx*mDx);
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::Diff2YZpm(int i, int j, int k) const
{
	return (  mGrid.GetValue(i, j+1, k+1) - mGrid.GetValue(i, j-1, k+1) 
			+ mGrid.GetValue(i, j-1, k-1) - mGrid.GetValue(i, j+1, k-1) )
			/ (4.0*mDx*mDx);
}

//! \lab4
/*! Use the values in the grid (mGrid.GetValue) to compute the differentials */
// By convention, we use (i,j,k) to represent grid coordinates, while (x,y,z)
// represents world coordinates.
float LevelSet::Diff2ZXpm(int i, int j, int k) const
{
	return (  mGrid.GetValue(i+1, j, k+1) - mGrid.GetValue(i-1, j, k+1) 
			+ mGrid.GetValue(i-1, j, k-1) - mGrid.GetValue(i+1, j, k-1) )
			/ (4.0*mDx*mDx);
}


float LevelSet::WENO(float v1, float v2, float v3, float v4, float v5) const
{
  return 0;
}


void LevelSet::Render()
{
  Bbox box = GetBoundingBox();
  Vector3<float> extent = box.pMax - box.pMin;
  float dx = extent[0] / mGrid.GetDimX();
  float dy = extent[1] / mGrid.GetDimY();
  float dz = extent[2] / mGrid.GetDimZ();

  if (mVisualizationMode == NarrowBand) {
    glPointSize(3.0);
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    LevelSetGrid::Iterator iter = mGrid.BeginNarrowBand();
    LevelSetGrid::Iterator iend = mGrid.EndNarrowBand();
    while (iter != iend) {
      unsigned int i = iter.GetI();
      unsigned int j = iter.GetJ();
      unsigned int k = iter.GetK();

      float x = i*dx + box.pMin[0];
      float y = j*dy + box.pMin[1];
      float z = k*dz + box.pMin[2];

      glVertex3f(x,y,z);
      iter++;
    }
    glEnd();
    glPointSize(1.0);
  }

  Implicit::Render();
}


void LevelSet::TransformWorldToGrid(float & i, float & j, float & k) const
{
  TransformW2O(i,j,k);
  i = (i - mBox.pMin[0]) / mDx;
  j = (j - mBox.pMin[1]) / mDx;
  k = (k - mBox.pMin[2]) / mDx;
}

void LevelSet::TransformGridToWorld(float & x, float & y, float & z) const
{
  x = x*mDx + mBox.pMin[0];
  y = y*mDx + mBox.pMin[1];
  z = z*mDx + mBox.pMin[2];

  Vector4<float> v(x,y,z,1);
  v = mTransform*v;
  x = v[0]; y = v[1]; z = v[2];
}
