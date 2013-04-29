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

#include "LevelSetGrid.h"
#include "Util/Util.h"


LevelSetGrid::Iterator::Iterator(const BitMask3D * mask, int i, int j, int k)
{
  // Initialize counters
  this->mask = mask;
  this->iMax = mask->GetDimX();
  this->jMax = mask->GetDimY();
  this->kMax = mask->GetDimZ();
  this->i = Clamp(i, (int)0, iMax);
  this->j = Clamp(j, (int)0, jMax);
  this->k = Clamp(k, (int)0, kMax);

  // Check if we're within range
  if(i == iMax && j == jMax && k == kMax){
    endState = true;
  } else {
    endState = false;
  }

  // Go to the first valid entry
  if (!endState && !mask->GetValue(i,j,k)) (*this)++;
}


void LevelSetGrid::Dilate()
{
  BitMask3D newMask(mMask);
  Iterator it = BeginNarrowBand();
  Iterator iend = EndNarrowBand();
  while(it!=iend){
    int i = it.GetI();
    int j = it.GetJ();
    int k = it.GetK();
    newMask.SetValue(i, j, k, true);
    if (k < GetDimZ()-1)  newMask.SetValue(i, j, k+1, true);
    if (k > 0)            newMask.SetValue(i, j, k-1, true);
    if (j < GetDimY()-1)  newMask.SetValue(i, j+1, k, true);
    if (j > 0)            newMask.SetValue(i, j-1, k, true);
    if (i < GetDimX()-1)  newMask.SetValue(i+1, j, k, true);
    if (i > 0)            newMask.SetValue(i-1, j, k, true);
    it++;
  }
  mMask = newMask;
}


void LevelSetGrid::Rebuild()
{
  Iterator it = BeginNarrowBand();
  Iterator iend = EndNarrowBand();
  while(it!=iend){
    int i = it.GetI();
    int j = it.GetJ();
    int k = it.GetK();

    //    std::cerr << mPhi.GetValue(i,j,k) << " -> " ;
    if(mPhi.GetValue(i,j,k) > mOutsideConstant) {
      mPhi.SetValue(i, j, k, mOutsideConstant);
      mMask.SetValue(i, j, k, false);
    }
    else if(mPhi.GetValue(i,j,k) < mInsideConstant) {
      mPhi.SetValue(i, j, k, mInsideConstant);
      mMask.SetValue(i, j, k, false);
    }
    //    std::cerr << mPhi.GetValue(i,j,k) << ", " ;
    it++;
  }
}


Vector3<int> LevelSetGrid::GetDimensions()
{
  return Vector3<int>(GetDimX(), GetDimY(), GetDimZ());
}

