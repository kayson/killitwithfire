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
#ifndef __levelset_grid_h__
#define __levelset_grid_h__

#include "Math/Volume.h"
#include "Math/Vector3.h"
#include "Util/BitMask3D.h"
#include <iostream>
#include <limits>

class LevelSetGrid
{
protected:
  Volume<float> mPhi;
  BitMask3D mMask;
  float mInsideConstant, mOutsideConstant;


public:

  LevelSetGrid(int dimX=0, int dimY=0, int dimZ=0,
               float insideConstant=-(std::numeric_limits<float>::max)(),
               float outsideConstant=(std::numeric_limits<float>::max)())
    : mPhi(dimX, dimY, dimZ, outsideConstant), mMask(dimX, dimY, dimZ),
      mInsideConstant(insideConstant), mOutsideConstant(outsideConstant) { }

  ~LevelSetGrid() { }



  class Iterator
  {
    friend class LevelSetGrid;

  protected:
    const BitMask3D * mask;
    int i,j,k;
    int iMax, jMax, kMax;
    bool endState;

    Iterator(const BitMask3D * mask, int i=0, int j=0, int k=0);


  public :

    inline Iterator & operator ++ (int) {
      endState = false;
      if (endState != true){
        do{
          ++k;
          if(k >= kMax) {
            k = 0;
            ++j;
            if(j >= jMax){
              j = 0;
              ++i;
              if(i >= iMax){
                            k = kMax;
                            j = jMax;
                            i = iMax;
                endState = true;
              }
            }
          }
        } while (!endState && (mask->GetValue(i,j,k) != true));
      }
      return *this;
    }

    bool operator !=(const Iterator & b) const {
      return (this->i != b.i || this->j != b.j || this->k != b.k);
    }

    int GetI() const { return i; }
    int GetJ() const { return j; }
    int GetK() const { return k; }
  };


  Iterator BeginNarrowBand() { return Iterator(&mMask); }
  const Iterator BeginNarrowBand() const { return Iterator(&mMask); }

  Iterator EndNarrowBand() { return Iterator(&mMask, mMask.GetDimX(), mMask.GetDimY(), mMask.GetDimZ()); }
  const Iterator EndNarrowBand() const { return Iterator(&mMask, mMask.GetDimX(), mMask.GetDimY(), mMask.GetDimZ()); }

  inline int GetDimX() const { return mPhi.GetDimX(); }
  inline int GetDimY() const { return mPhi.GetDimY(); }
  inline int GetDimZ() const { return mPhi.GetDimZ(); }

  //! Return grid dimensions as measured in number of grid cells
  Vector3<int> GetDimensions();

  inline float GetValue(int i, int j, int k) const { return mPhi.GetValue(i,j,k); }
  inline void SetValue(int i, int j, int k, float f) {
    SetMask(i,j,k,true);
    mPhi.SetValue(i,j,k, f);
  }

  inline bool GetMask(int i, int j, int k) const { return mMask.GetValue(i,j,k); }
  inline void SetMask(int i, int j, int k, bool b) { mMask.SetValue(i,j,k,b); }

  void SetInsideConstant(float insideConstant) { mInsideConstant = insideConstant; }
  inline const float GetInsideConstant() const { return mInsideConstant; }

  void SetOutsideConstant(float outsideConstant) { mOutsideConstant = outsideConstant; }
  inline const float GetOutsideConstant() const { return mOutsideConstant; }

  //! Dilates the narrow band with 6 connectivity
  void Dilate();

  //! Rebuild the narrow band by culling too large values from mask
  void Rebuild();



  friend std::ostream& operator << (std::ostream &os, const LevelSetGrid &grid)
  {
    os << "Grid dimensions: " << "(" << grid.GetDimX() << "x" << grid.GetDimY() << "x" << grid.GetDimZ() << ")" << std::endl;
    Iterator iter = grid.BeginNarrowBand();
    Iterator iend = grid.EndNarrowBand();
    while (iter != iend) {
      int i = iter.GetI(), j = iter.GetJ(), k = iter.GetK();
      os << "(" << i << "," << j << "," << k << "): " << grid.GetValue(i,j,k) << std::endl;
      iter++;
    }
    return os;
  }
};


#endif
