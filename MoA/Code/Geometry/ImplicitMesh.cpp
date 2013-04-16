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
#include "Geometry/ImplicitMesh.h"
#include "Util/ColorMap.h"

//-----------------------------------------------------------------------------
ImplicitMesh::ImplicitMesh(SimpleMesh * mesh) : mSourceMesh(mesh), mData(NULL)
{
  // Loop through all vertices in the mesh to determine the smallest
  // bounding box needed
  Vector3<float> pMin((std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)());
  Vector3<float> pMax(-(std::numeric_limits<float>::max)(), -(std::numeric_limits<float>::max)(), -(std::numeric_limits<float>::max)());
  const std::vector<SimpleMesh::Vertex>& verts = mSourceMesh->GetVerts();
  for(unsigned int i=0; i < verts.size(); i++){
    const SimpleMesh::Vertex &v = verts.at(i);
    for (int j = 0; j < 3; j++) {
      if (pMin[j] > v.pos[j]) pMin[j] = v.pos[j];
      if (pMax[j] < v.pos[j]) pMax[j] = v.pos[j];
    }
  }

  // Pad with 0.1 to get around border issues
  Vector3<float> pad(0.1, 0.1, 0.1);
  pMin -= pad;
  pMax += pad;
  mBox = Bbox(pMin, pMax);
  std::cout << "Bounding box of implicit mesh: " << mBox << std::endl;
}

//-----------------------------------------------------------------------------
ImplicitMesh::~ImplicitMesh()
{
  delete mSourceMesh;
  mSourceMesh = NULL;

  delete mData;
  mData = NULL;
}


//-----------------------------------------------------------------------------
// Sample distances to the mesh in the entire bounding box
void ImplicitMesh::Initialize()
{
  // First, delete old data grid
  delete mData;
  mData = NULL;

  // Create new grid
  Vector3<float> dim = (mBox.pMax - mBox.pMin) / mMeshSampling;
  mData = new Volume<float>(ceil(dim[0]), ceil(dim[1]), ceil(dim[2]));

  // Setup progress bar
  unsigned int totalSamples = mData->GetDimX()*mData->GetDimY()*mData->GetDimZ();
  unsigned int currentSample = 0;
  unsigned int reportFreq = totalSamples / 30;

  // Start sampling...
  std::cerr << "Computing distances to mesh [";
  int i, j, k;
  i = 0;
  for(float x = mBox.pMin[0]; x < mBox.pMax[0]-0.5*mMeshSampling; x += mMeshSampling, i++) {
    j = 0;
    for(float y = mBox.pMin[1]; y < mBox.pMax[1]-0.5*mMeshSampling; y += mMeshSampling, j++) {
      k = 0;
      for(float z = mBox.pMin[2]; z < mBox.pMax[2]-0.5*mMeshSampling; z += mMeshSampling, k++) {
        mData->SetValue(i,j,k, DistanceToPoint(x,y,z, *mSourceMesh));

        currentSample++;
        if (currentSample % reportFreq == 0)
          std::cerr << "=";
      }
    }
  }
  std::cerr << "] done" << std::endl;

  SimpleMesh dilatedMesh = *mSourceMesh;
  dilatedMesh.Initialize();
  dilatedMesh.Dilate(0.0001);

  std::cerr << "Determining inside/outside [";
  i = 0;
  currentSample = 0;
  for(float x = mBox.pMin[0]; x < mBox.pMax[0]-0.5*mMeshSampling; x += mMeshSampling, i++) {
    j = 0;
    for(float y = mBox.pMin[1]; y < mBox.pMax[1]-0.5*mMeshSampling; y += mMeshSampling, j++) {
      k = 0;
      for(float z = mBox.pMin[2]; z < mBox.pMax[2]-0.5*mMeshSampling; z += mMeshSampling, k++) {
        float distance = DistanceToPoint(x,y,z, dilatedMesh);
        if (mData->GetValue(i,j,k) - distance < 0)
          mData->SetValue(i,j,k, -mData->GetValue(i,j,k));

        currentSample++;
        if (currentSample % reportFreq == 0)
          std::cerr << "=";
      }
    }
  }

  std::cerr << "] done" << std::endl;
  Implicit::Update();
}


float ImplicitMesh::DistanceToPoint(float x, float y, float z, const SimpleMesh & mesh) const{

  // just loop over all faces and take the min distance.
  // uses normals to determine direction and resulting sign (negative inside)
  std::pair<float, bool> pr((std::numeric_limits<float>::max)(), true);
  const std::vector<SimpleMesh::Vertex>& verts = mesh.GetVerts();
  const std::vector<SimpleMesh::Face>& faces = mesh.GetFaces();
  Vector3<float> p(x,y,z);
  for(unsigned int i=0; i<faces.size(); i++){
    const SimpleMesh::Vertex &v1 = verts.at(faces.at(i).v1);
    const SimpleMesh::Vertex &v2 = verts.at(faces.at(i).v2);
    const SimpleMesh::Vertex &v3 = verts.at(faces.at(i).v3);

    std::pair<float, bool> pt = DistanceSquared(p, v1.pos, v2.pos, v3.pos);
    if(pt.first < pr.first)
      pr = pt;
  }
  pr.first = std::sqrt(pr.first);
  return pr.first; //pr.second ? pr.first : -pr.first;
}


// The function below is taken from:

// Wild Magic Source Code
// David Eberly
// http://www.geometrictools.com
// Copyright (c) 1998-2007
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.  The license is available for reading at
// either of the locations:
//     http://www.gnu.org/copyleft/lgpl.html
//     http://www.geometrictools.com/License/WildMagicLicense.pdf
// The license applies to versions 0 through 4 of Wild Magic.
//
// Version: 4.0.0 (2006/06/28)


/*
 * http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
 Remember to use consistent orientation
*/
std::pair<float, bool> ImplicitMesh::DistanceSquared(const Vector3<float> & p,
							   const Vector3<float> &v1,
							   const Vector3<float> &v2,
							   const Vector3<float> &v3){

  Vector3<float> kDiff = v1 - p;
  Vector3<float> kEdge0 = v2 - v1;
  Vector3<float> kEdge1 = v3 - v1;
  float fA00 = kEdge0.Norm();
  float fA01 = kEdge0*kEdge1;
  float fA11 = kEdge1.Norm();
  float fB0 = kDiff*kEdge0;
  float fB1 = kDiff*kEdge1;
  float fC = kDiff.Norm();
  float fDet = std::abs(fA00*fA11-fA01*fA01);
  float fS = fA01*fB1-fA11*fB0;
  float fT = fA01*fB0-fA00*fB1;
  float fSqrDistance;

  if (fS + fT <= fDet)
    {
      if (fS < 0.0)
        {
	  if (fT < 0.0)  // region 4
            {
	      if (fB0 < 0.0)
                {
		  fT = 0.0;
		  if (-fB0 >= fA00)
                    {
		      fS = 1.0;
		      fSqrDistance = fA00+(2.0)*fB0+fC;
                    }
		  else
                    {
		      fS = -fB0/fA00;
		      fSqrDistance = fB0*fS+fC;
                    }
                }
	      else
                {
		  fS = 0.0;
		  if (fB1 >= 0.0)
                    {
		      fT = 0.0;
		      fSqrDistance = fC;
                    }
		  else if (-fB1 >= fA11)
                    {
		      fT = 1.0;
		      fSqrDistance = fA11+(2.0)*fB1+fC;
                    }
		  else
                    {
		      fT = -fB1/fA11;
		      fSqrDistance = fB1*fT+fC;
                    }
                }
            }
	  else  // region 3
            {
	      fS = 0.0;
	      if (fB1 >= 0.0)
                {
		  fT = 0.0;
		  fSqrDistance = fC;
                }
	      else if (-fB1 >= fA11)
                {
		  fT = 1.0;
		  fSqrDistance = fA11+(2.0)*fB1+fC;
                }
	      else
                {
		  fT = -fB1/fA11;
		  fSqrDistance = fB1*fT+fC;
                }
            }
        }
      else if (fT < 0.0)  // region 5
        {
	  fT = 0.0;
	  if (fB0 >= 0.0)
            {
	      fS = 0.0;
	      fSqrDistance = fC;
            }
	  else if (-fB0 >= fA00)
            {
	      fS = 1.0;
	      fSqrDistance = fA00+(2.0)*fB0+fC;
            }
	  else
            {
	      fS = -fB0/fA00;
	      fSqrDistance = fB0*fS+fC;
            }
        }
      else  // region 0
        {
	  // minimum at interior point
	  float fInvDet = (1.0)/fDet;
	  fS *= fInvDet;
	  fT *= fInvDet;
	  fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0)*fB0) +
	    fT*(fA01*fS+fA11*fT+(2.0)*fB1)+fC;
        }
    }
  else
    {
      float fTmp0, fTmp1, fNumer, fDenom;

      if (fS < 0.0)  // region 2
        {
	  fTmp0 = fA01 + fB0;
	  fTmp1 = fA11 + fB1;
	  if (fTmp1 > fTmp0)
            {
	      fNumer = fTmp1 - fTmp0;
	      fDenom = fA00-2.0f*fA01+fA11;
	      if (fNumer >= fDenom)
                {
		  fS = 1.0;
		  fT = 0.0;
		  fSqrDistance = fA00+(2.0)*fB0+fC;
                }
	      else
                {
		  fS = fNumer/fDenom;
		  fT = 1.0 - fS;
		  fSqrDistance = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
		    fT*(fA01*fS+fA11*fT+(2.0)*fB1)+fC;
                }
            }
	  else
            {
	      fS = 0.0;
	      if (fTmp1 <= 0.0)
                {
		  fT = 1.0;
		  fSqrDistance = fA11+(2.0)*fB1+fC;
                }
	      else if (fB1 >= 0.0)
                {
		  fT = 0.0;
		  fSqrDistance = fC;
                }
	      else
                {
		  fT = -fB1/fA11;
		  fSqrDistance = fB1*fT+fC;
                }
            }
        }
      else if (fT < 0.0)  // region 6
        {
	  fTmp0 = fA01 + fB1;
	  fTmp1 = fA00 + fB0;
	  if (fTmp1 > fTmp0)
            {
	      fNumer = fTmp1 - fTmp0;
	      fDenom = fA00-(2.0)*fA01+fA11;
	      if (fNumer >= fDenom)
                {
		  fT = 1.0;
		  fS = 0.0;
		  fSqrDistance = fA11+(2.0)*fB1+fC;
                }
	      else
                {
		  fT = fNumer/fDenom;
		  fS = 1.0 - fT;
		  fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0)*fB0) +
		    fT*(fA01*fS+fA11*fT+(2.0)*fB1)+fC;
                }
            }
	  else
            {
	      fT = 0.0;
	      if (fTmp1 <= 0.0)
                {
		  fS = 1.0;
		  fSqrDistance = fA00+(2.0)*fB0+fC;
                }
	      else if (fB0 >= 0.0)
                {
		  fS = 0.0;
		  fSqrDistance = fC;
                }
	      else
                {
		  fS = -fB0/fA00;
		  fSqrDistance = fB0*fS+fC;
                }
            }
        }
      else  // region 1
        {
	  fNumer = fA11 + fB1 - fA01 - fB0;
	  if (fNumer <= 0.0)
            {
	      fS = 0.0;
	      fT = 1.0;
	      fSqrDistance = fA11+(2.0)*fB1+fC;
            }
	  else
            {
	      fDenom = fA00-2.0f*fA01+fA11;
	      if (fNumer >= fDenom)
                {
		  fS = 1.0;
		  fT = 0.0;
		  fSqrDistance = fA00+(2.0)*fB0+fC;
                }
	      else
                {
		  fS = fNumer/fDenom;
		  fT = 1.0 - fS;
		  fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0)*fB0) +
		    fT*(fA01*fS+fA11*fT+(2.0)*fB1)+fC;
                }
            }
        }
    }

  // account for numerical round-off error
  if (fSqrDistance < 0.0)
    {
      fSqrDistance = 0.0;
    }

  //    Could be used for fun stuff...
  //    Vector3<float> closest = v1 + fS*kEdge0 + fT*kEdge1;
  //    float u = fs,v fT, w = std::max<float>(0.0, 1-u-v);

  bool outside = true;
  if (kDiff*(Cross(kEdge0,kEdge1)) < 0)
    outside = false;

  return std::make_pair(fSqrDistance, outside);
}
