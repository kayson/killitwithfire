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
#ifndef __simplemesh_h__
#define __simplemesh_h__

#include "Mesh.h"

#include <set>
#include <map>
#include <cassert>
#include <limits>
#include <algorithm>
#include <cmath>
#include <utility>

#include "Math/Vector3.h"
#include "Util/JetColorMap.h"

struct MyEdge{
  MyEdge(unsigned int a, unsigned int b) : a(std::min(a, b)), b(std::max(a,b)) {}
  bool operator < (const MyEdge &eb) const {
    if (a<eb.a)
      return true;
    else if(a==eb.a)
      if(b<eb.b)
        return true;
    return false;
  }
  unsigned int a, b;
};


class SimpleMesh : public Mesh {
public:
  struct Face : public Mesh::Face {
    Face(unsigned int v1, unsigned int v2, unsigned int v3)
      : Mesh::Face(), v1(v1), v2(v2), v3(v3) {}
    unsigned int v1, v2, v3;
  };

  struct Vertex : public Mesh::Vertex {
    Vertex() : Mesh::Vertex() {}
  };

protected:
  //! Computes a facenormal for a given face
  Vector3<float> FaceNormal(unsigned int faceindx) const;

  //! Computes a facenormal for a given face
  Vector3<float> VertexNormal(unsigned int vertexindx) const;

  std::vector<Vertex> mVerts;
  std::vector<Face> mFaces;

  std::map<Vector3<float>, unsigned int> mUniqueVerts;

  //! Adds a vertex to the mesh
  virtual bool AddVertex(const Vector3<float>& v, unsigned int &indx);

  // Given a vertex, find all triangles that includes this vertex
  virtual std::vector<unsigned int> FindNeighborFaces(unsigned int vertexIndex) const;

  // Given a vertex, find the one-ring neighborhood. Ie all vertices connected to this vertex, sorted counter clockwise
  virtual std::vector<unsigned int> FindNeighborVertices(unsigned int vertexIndex) const;

  //! Compute curvature at vertex
  float VertexCurvature(unsigned int vtxindx) const;
  float VertexCurvature2(unsigned int vtxindx) const;

  //! Compute curvature at face as average of nb faces
  float FaceCurvature(unsigned int vtxindx) const;

public:
  SimpleMesh();
  ~SimpleMesh();

  virtual void Update();
  virtual void Initialize();

  //! Adds a triangle to the mesh.
  virtual bool AddFace(const std::vector<Vector3<float> > &verts);

  //! Access to internal vertex data
  const std::vector<Vertex>& GetVerts() const { return mVerts; }
  const std::vector<Face>& GetFaces() const { return mFaces; }
  std::vector<Vertex>& GetVerts() { return mVerts; }
  std::vector<Face>& GetFaces() { return mFaces; }

  virtual void Dilate(float amount);
  virtual void Erode(float amount);
  virtual void Smooth(float amount);

  int Genus() const;

  virtual void Render();

  virtual bool save(std::ostream &os){
    os << "# SimpleMesh obj streamer\n# M&A 2008\n\n";
    os << "# Vertices\n";
    for(unsigned int i=0; i<mVerts.size(); i++){
      os << "v " << mVerts.at(i).pos[0] << " " <<  mVerts.at(i).pos[1] << " " << mVerts.at(i).pos[2] << "\n";
    }
    os << "\n# Faces\n";
    for(unsigned int i=0; i<mFaces.size(); i++){
      os << "f " << mFaces.at(i).v1+1 << " "
         << mFaces.at(i).v2+1
         << " " <<  mFaces.at(i).v3+1 << "\n";
    }
    return os.good();
  }
};


#endif
