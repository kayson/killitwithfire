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
#ifndef _A_LOOP_SUBDIVISION_MESH_
#define _A_LOOP_SUBDIVISION_MESH_

#include "Subdivision/LoopSubdivisionMesh.h"

/*! \brief Abstract base class for Adaptive Subdivision that implements the Loop scheme
 */
class AdaptiveLoopSubdivisionMesh : public LoopSubdivisionMesh
{
public :

  AdaptiveLoopSubdivisionMesh(const HalfEdgeMesh & m, unsigned int s) : LoopSubdivisionMesh(m, s) { }
  AdaptiveLoopSubdivisionMesh()  { }

  virtual ~AdaptiveLoopSubdivisionMesh() { }

  //! Subdivides the mesh uniformly one step
  virtual void Subdivide();

protected :
  virtual bool Subdividable(unsigned int faceIndex) { return true; }

  //! Subdivides the face at faceIndex given 1 not subdividable neighbor
  virtual std::vector< std::vector<Vector3<float> > > Subdivide1(unsigned int faceIndex);

  //! Subdivides the face at faceIndex given 2 not subdividable neighbors
  virtual std::vector< std::vector<Vector3<float> > > Subdivide2(unsigned int faceIndex);

  //! Subdivides the face at faceIndex given 3 not subdividable neighbors (trivial)
  virtual std::vector< std::vector<Vector3<float> > > Subdivide3(unsigned int faceIndex);

  //! Computes a new vertex, replacing a vertex in the old mesh
  virtual Vector3<float> VertexRule(unsigned int vertexIndex);
};

#endif
