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
#ifndef __volume_h__
#define __volume_h__

#include <vector>
#include <cassert>
#include <cstdio>
#include "Math/Vector3.h"
#include "Util/Util.h"

/*!
 * A 3D volume of templated type T.
 * Stores values in an stl vector<T>. Uses row major storage, i.e. dimZ's is changing most
 * rapidly.
 */
template<class T>
class Volume{
protected:
  //! An stl vector to hold actual data
  std::vector<T> mData;
  //! The dimensions of the volume
  int mDimX, mDimY, mDimZ;
  //! premult = dimY*dimZ, avoids this multiplication for each getValue
  int mPremult;

public:
  //! Default constructor initializes to zero volume
  Volume() : mData(0), mDimX(0), mDimY(0), mDimZ(0), mPremult(0) {}
  //! Sized constructor initializes volume of size dimXxdimYxdimZ to default value for type T
  Volume(int dimX, int dimY, int dimZ)
    : mData(dimX*dimY*dimZ, T()), mDimX(dimX), mDimY(dimY), mDimZ(dimZ), mPremult(dimY*dimZ){}

  Volume(int dimX, int dimY, int dimZ, T defaultVal)
    : mData(dimX*dimY*dimZ, defaultVal), mDimX(dimX), mDimY(dimY), mDimZ(dimZ), mPremult(dimY*dimZ){}

  inline int GetDimX() const { return mDimX; }
  inline int GetDimY() const { return mDimY; }
  inline int GetDimZ() const { return mDimZ; }
  //! Returns the value at i,j,k
  inline T GetValue(int i, int j, int k) const {
    i = Clamp(i, 0, mDimX-1);
    j = Clamp(j, 0, mDimY-1);
    k = Clamp(k, 0, mDimZ-1);
    //return mData.at(i*mPremult + j*mDimZ + k); // .at() does bound checking, throws exception
    return mData[i*mPremult + j*mDimZ + k]; // op [] does no bound checking
  }

  //! Returns the value at x,y,z (uses trilinear interpolation
  T GetValue(float x, float y, float z) const
  {
    int i = (int)x;
    int j = (int)y;
    int k = (int)z;

    float bx = x - i;
    float by = y - j;
    float bz = z - k;

    T val =
      GetValue(i,   j,   k  ) * (1-bx) * (1-by) * (1-bz) +
      GetValue(i+1, j,   k  ) *  bx    * (1-by) * (1-bz) +
      GetValue(i+1, j+1, k  ) *  bx    *  by    * (1-bz) +
      GetValue(i,   j+1, k  ) * (1-bx) *  by    * (1-bz) +
      GetValue(i,   j,   k+1) * (1-bx) * (1-by) *  bz    +
      GetValue(i+1, j,   k+1) *  bx    * (1-by) *  bz    +
      GetValue(i+1, j+1, k+1) *  bx    *  by    *  bz    +
      GetValue(i,   j+1, k+1) * (1-bx) *  by    *  bz;

    return val;
  }


  unsigned int ComputeLinearIndex(int i, int j, int k) const
  {
    i = Clamp(i, 0, mDimX-1);
    j = Clamp(j, 0, mDimY-1);
    k = Clamp(k, 0, mDimZ-1);
    return i*mPremult + j*mDimZ + k;
  }

  void ComputeIndices(unsigned int ind, int & i, int & j, int & k) const
  {
    i = i / mPremult;
    j = (i % mPremult) / mDimZ;
    k = (i % mPremult) % mDimZ;
  }

  //! Sets the value at i,j,k to val
  void SetValue(int i, int j, int k, const T& val){
    assert(i < mDimX && i >= 0 && j < mDimY && j >= 0 && k < mDimZ && k >= 0);
    mData.at(i*mPremult + j*mDimZ + k) = val; // .at() does bound checking, throws exception
    // mData[i*premult + j*dimZ + k] = val;
  }

  //! Load a volume from binary stream is
  void Load(std::istream & is){
    is.read((char *)&mDimX, sizeof(mDimX));
    is.read((char *)&mDimY, sizeof(mDimY));
    is.read((char *)&mDimZ, sizeof(mDimZ));

    if(IsBigEndian()){
      mDimX = EndianSwap(mDimX);
      mDimY = EndianSwap(mDimY);
      mDimZ = EndianSwap(mDimZ);
    }

    std::cerr << "Loading dims: " << mDimX << ", " << mDimY << ", " << mDimZ << std::endl;
    mData.resize(mDimX*mDimY*mDimZ); // resize (only upwards)
    std::vector<T>(mData).swap(mData); // shrink to fit (if needed)
    mPremult = mDimY*mDimZ;

    is.read((char *)&mData.at(0), sizeof(T)*mDimX*mDimY*mDimZ);
    if(IsBigEndian()){
      EndianSwap(&mData.at(0), mData.size());
    }
  }

  void Load(const std::string & path)
  {
    FILE * file;
    if ((file = fopen(path.c_str(), "rb")) != NULL) {
      int w;
      fscanf(file, "%d \n", &w);
      if (w == 4) {
        // only float supported!
        fscanf(file, "%d %d %d \n", &mDimZ, &mDimY, &mDimX);
      }
      else {
        fclose(file);
        std::cerr << "Error: Only float volumes supported, file bit depth = " << w << std::endl;
        return;
      }

      std::cerr << "Loading dims: " << mDimX << ", " << mDimY << ", " << mDimZ << std::endl;
      mData.resize(mDimX*mDimY*mDimZ); // resize (only upwards)
      std::vector<T>(mData).swap(mData); // shrink to fit (if needed)
      mPremult = mDimY*mDimZ;

      fread((char *)&mData.at(0), sizeof(T), mDimX*mDimY*mDimZ, file);
      fclose(file);
      if(IsBigEndian()){
        EndianSwap(&mData.at(0), mData.size());
      }
    }
  }

  //! Save volume to binary stream os
  void Save(std::ostream & os) const{
    os.write((const char *)&mDimX, sizeof(mDimX));
    os.write((const char *)&mDimY, sizeof(mDimY));
    os.write((const char *)&mDimZ, sizeof(mDimZ));

    os.write((const char *)&mData.at(0), sizeof(T)*mDimX*mDimY*mDimZ);
  }
};

#endif
