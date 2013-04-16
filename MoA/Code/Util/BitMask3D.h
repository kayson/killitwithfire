/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Söderström (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#ifndef __bitmask3d_h__
#define __bitmask3d_h__

#include <vector>
#include <cassert>
/*!
 * A 3D bit mask with a boolean value denoting true or false for each i,j,k.
 *  Uses stl's vector<bool> which is specialized to pack data and use one bit per bool.
 */
class BitMask3D
{
protected:
  //! An stl vector to hold actual data
  std::vector<bool> mData;
  //! The dimensions of the mask
  int mDimX, mDimY, mDimZ;
  //! premult = mDimY*mDimZ, avoids this multiplication for each getValue
  int premult;

public:
  //! Default constructor initializes to zero volume
  BitMask3D() : mData(0), mDimX(0), mDimY(0), mDimZ(0), premult(0) {}
  //! Sized constructor initializes volume of size mDimXxmDimYxmDimZ to false
  BitMask3D(int dimX, int dimY, int dimZ)
    : mData(dimX*dimY*dimZ, false), mDimX(dimX), mDimY(dimY), mDimZ(dimZ), premult(dimY*dimZ){}
  inline int GetDimX() const { return mDimX; }
  inline int GetDimY() const { return mDimY; }
  inline int GetDimZ() const { return mDimZ; }
  //! Returns the value at i,j,k
  bool GetValue(int i, int j, int k) const {
    i = Clamp(i, 0, mDimX-1);
    j = Clamp(j, 0, mDimY-1);
    k = Clamp(k, 0, mDimZ-1);
    return mData.at(i*premult + j*mDimZ + k); // .at() does bound checking, throws exception
    //return mData[i*premult + j*mDimZ + k]; // op [] does no bound checking
  }
  //! Sets the value at i,j,k to val
  void SetValue(int i, int j, int k, bool val){
    assert(i < mDimX && i >= 0 && j < mDimY && j >= 0 && k < mDimZ && k >= 0);
    mData.at(i*premult + j*mDimZ + k) = val; // .at() does bound checking, throws exception
    // mData[i*premult + j*mDimZ + k] = val;
  }
};

#endif
