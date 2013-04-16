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
#ifndef _LOOP_SUBDIVISION_MESH_
#define _LOOP_SUBDIVISION_MESH_

#include "Subdivision.h"
#include "Geometry/HalfEdgeMesh.h"

/*! \brief Subdivision mesh that implements the Loop scheme
*/
class LoopSubdivisionMesh : public Subdivision, public HalfEdgeMesh
{
public :

  LoopSubdivisionMesh(const HalfEdgeMesh & m, unsigned int s) : HalfEdgeMesh(m), mNumSubDivs(s) { }
  LoopSubdivisionMesh() : mNumSubDivs(0) { }

  virtual ~LoopSubdivisionMesh() { }

  //! Subdivides the mesh uniformly one step
  virtual void Subdivide();

  virtual const char * GetTypeName() { return typeid(LoopSubdivisionMesh).name(); }

  //! Return weights for interior verts
  static float Beta(unsigned int valence);

protected :
  //! The number of accumulated subdivisions
  unsigned int mNumSubDivs;

  //! Subdivides the face at faceIndex and returns a vector of faces
  virtual std::vector< std::vector<Vector3<float> > > Subdivide(unsigned int faceIndex);

  //! Computes a new vertex, replacing a vertex in the old mesh
  virtual Vector3<float> VertexRule(unsigned int vertexIndex);

  //! Computes a new vertex, placed along an edge in the old mesh
  virtual Vector3<float> EdgeRule(unsigned int edgeIndex);
};

#endif
