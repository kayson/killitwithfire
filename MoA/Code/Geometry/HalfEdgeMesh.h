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
#ifndef __HALF_EDGE_H__
#define __HALF_EDGE_H__

#include "Geometry/Mesh.h"
#include "Util/ObjIO.h"
#include <map>
#include <limits>
#include <cassert>
#include <set>

/*! \brief A half edge triangle mesh class.
 * A mesh data structure with fast access to neighborhood entities.
 * The neighborhood links are stored through indices.
 */
class HalfEdgeMesh : public Mesh {
public:
  HalfEdgeMesh();
  ~HalfEdgeMesh();
  virtual void Update();
  virtual void Initialize();

  class EdgeIterator{
    friend class HalfEdgeMesh; // why friend?
  protected:
    // should not change what mesh the pointer points to, can change mesh though
    HalfEdgeMesh const * mHem;
    mutable unsigned int mIndex;
    EdgeIterator(HalfEdgeMesh const * he, unsigned int index){
      mHem = he;
      // Guard against outside access
      assert(index < mHem->GetNumEdges());
      mIndex = index;
    }
  public:
    EdgeIterator& Next(){ mIndex = mHem->e(mIndex).next; return *this; }
    EdgeIterator& Prev(){ mIndex = mHem->e(mIndex).prev; return *this; }
    EdgeIterator& Pair(){ mIndex = mHem->e(mIndex).pair; return *this; }
    const EdgeIterator& Next() const { mIndex = mHem->e(mIndex).next; return *this; }
    const EdgeIterator& Prev() const{ mIndex = mHem->e(mIndex).prev; return *this; }
    const EdgeIterator& Pair() const{ mIndex = mHem->e(mIndex).pair; return *this; }
    unsigned int GetEdgeIndex() const { return mIndex; }
    unsigned int GetEdgeVertexIndex() const { return mHem->e(mIndex).vert; }
    unsigned int GetEdgeFaceIndex() const { return mHem->e(mIndex).face; }
    const bool operator ==(const EdgeIterator & eit){ return this->mIndex == eit.mIndex; }
    const bool operator !=(const EdgeIterator & eit){ return this->mIndex != eit.mIndex; }
  };

  EdgeIterator GetEdgeIterator(unsigned int i) { return EdgeIterator(this, i); }
  const EdgeIterator GetEdgeIterator(unsigned int i) const { return EdgeIterator(this, i); }


  //! Adds a triangle to the mesh.
  virtual bool AddFace(const std::vector<Vector3<float> > &verts);

  //! Calculates the area of the mesh
  virtual float Area() const;

  //! Calculates the volume of the mesh
  virtual float Volume() const;

  //! Calculates the genus of a mesh
  virtual int Genus() const;

  //! Calculates the number of shells
  virtual int Shells() const;

  //! Calculates the curvature at a vertex
  virtual float VertexCurvature(unsigned int vertexIndex) const;

  //! Calculates the curvature at a vertex
  virtual float FaceCurvature(unsigned int faceIndex) const;

  //! Calculates the normal at a face
  virtual Vector3<float> FaceNormal(unsigned int faceIndex) const;

  //! Calculates the normal at a vertex
  virtual Vector3<float> VertexNormal(unsigned int vertexIndex) const;

  //! Checks to see if the mesh is valid
  void Validate();

  virtual void Render();

protected:

  //! Denotes a reference to a border, only for face pointers
  const static unsigned int BORDER;
  //! Denotes a reference to a non-existing object
  const static unsigned int UNINITIALIZED;

  /*! \brief The core half-edge struct
   *  Implements the linked data structure edge type
   */
  struct HalfEdge {
    HalfEdge() : vert(UNINITIALIZED), face(UNINITIALIZED), next(UNINITIALIZED),
                 prev(UNINITIALIZED), pair(UNINITIALIZED) { }
    unsigned int vert;  //!< index into mVerts (the origin vertex)
    unsigned int face;  //!< index into mFaces
    unsigned int next;  //!< index into mEdges
    unsigned int prev;  //!< index into mEdges
    unsigned int pair;  //!< index into mEdges
  };

  /*! \brief A vertex is a point and an edge index
   */
  struct Vertex : public Mesh::Vertex {
    Vertex() : edge(UNINITIALIZED) { }
    unsigned int edge;     //!< index into mEdges
  };

  /*! \brief A face has a normal and an index to an edge
   */
  struct Face : public Mesh::Face {
    Face() : edge(UNINITIALIZED) { }
    unsigned int edge; //!< index into mEdges
  };

  //! The edges of the mesh
  std::vector<HalfEdge> mEdges;
  //! The vertices in the mesh
  std::vector<Vertex> mVerts;
  //! The faces in the mesh
  std::vector<Face> mFaces;

  //! A utility data structure to speed up removal of redundant vertices
  std::map<Vector3<float>, unsigned int> mUniqueVerts;

  struct OrderedPair{
    unsigned int p1, p2;
    OrderedPair(unsigned int i1, unsigned int i2) {
      p1 = std::min(i1, i2);
      p2 = std::max(i1, i2);
    }
    bool operator < (const OrderedPair & rhs) const{
      if (this->p1 < rhs.p1){
        return true;
      } else if(this->p1 == rhs.p1){
        if(this->p2 < rhs.p2){
          return true;
        }
      }
      return false;
    }
  };
  //! A utility data structure to speed up removal of redundant edges
  std::map<OrderedPair, unsigned int> mUniqueEdgePairs;

  //! Adds a vertex to the mesh
  virtual bool AddVertex(const Vector3<float>& v, unsigned int &indx);

  //! Adds a half edge pair, from vertex 1 to vertex 2, to the mesh
  bool AddHalfEdgePair(unsigned int v1, unsigned int v2, unsigned int &indx1, unsigned int &indx2);

  //! Merges the outer (uninitialized) edge for a newly created triangle
  void MergeOuterBoundaryEdge(unsigned int innerEdge);

  //! Finds all triangles that includes a given vertex.
  virtual std::vector<unsigned int> FindNeighborFaces(unsigned int vertexIndex) const;

  //! Finds all vertices that includes a given vertex.
  virtual std::vector<unsigned int> FindNeighborVertices(unsigned int vertexIndex) const;

  //! Return the edge at index i
  HalfEdge& e(unsigned int i) { return mEdges.at(i); }
  const HalfEdge& e(unsigned int i) const { return mEdges.at(i); }
  //! Return the face at index i
  Face& f(unsigned int i) { return mFaces.at(i); }
  const Face& f(unsigned int i) const { return mFaces.at(i); }
  //! Return the Vertex at index i
  Vertex& v(unsigned int i) { return mVerts.at(i); }
  const Vertex v(unsigned int i) const { return mVerts.at(i); }
  //! Return number of vertices
  unsigned int GetNumVerts() const { return mVerts.size(); }
  //! Return number of faces
  unsigned int GetNumFaces() const { return mFaces.size(); }
  //! Return number of edges
  unsigned int GetNumEdges() const { return mEdges.size(); }

  virtual void Dilate(float amount);
  virtual void Erode(float amount);
  virtual void Smooth(float amount);

  virtual bool save(std::ostream &os){
    os << "# HalfEdgeMesh obj streamer\n# M&A 2008\n\n";
    os << "# Vertices\n";
    for(unsigned int i=0; i<GetNumVerts(); i++){
      os << "v " << v(i).pos[0] << " " <<  v(i).pos[1] << " " <<  v(i).pos[2] << "\n";
    }
    os << "\n# Faces\n";
    for(unsigned int i=0; i<GetNumFaces(); i++){
      unsigned int ind = f(i).edge;
      os << "f ";
      os << e(ind).vert+1 << " ";
      ind = e(ind).next;
      os << e(ind).vert+1 << " ";
      ind = e(ind).next;
      os << e(ind).vert+1 << "\n";
    }
    return os.good();
  }
};

#endif
