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
#ifndef __MESH_H__
#define __MESH_H__

#include "Geometry/Geometry.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include <vector>


class Mesh : public Geometry{
public :

  static const VisualizationMode CurvatureVertex;
  static const VisualizationMode CurvatureFace;

  virtual std::list<VisualizationMode> GetVisualizationModes() {
    std::list<VisualizationMode> L = Geometry::GetVisualizationModes();
    L.push_back(CurvatureVertex);
    L.push_back(CurvatureFace);
    return L;
  }

  virtual bool save(std::ostream & os) = 0;

protected:

  //! Adds a vertex to the mesh
  virtual bool AddVertex(const Vector3<float>& v, unsigned int &indx) = 0;

  //! Given a vertex, find all triangles that includes this vertex (sorted counter clockwise)
  virtual std::vector<unsigned int> FindNeighborFaces(unsigned int vertexIndex) const = 0;

  //! Given a vertex, find the one-ring neighborhood. Ie all vertices that are connected to this vertex (sorted counter clockwise)
  virtual std::vector<unsigned int> FindNeighborVertices(unsigned int vertexIndex) const = 0;

  //! Compute and return the curvature at vertex at vertexIndex
  virtual float VertexCurvature(unsigned int vertexIndex) const = 0;

  //! Compute and return the curvature at face
  virtual float FaceCurvature(unsigned int faceIndex) const = 0;

  //! Compute and return the normal at face at faceIndex
  virtual Vector3<float> FaceNormal(unsigned int faceIndex) const = 0;

  //! Compute and return the normal at vertex at vertexIndex
  virtual Vector3<float> VertexNormal(unsigned int vertexIndex) const = 0;

  bool mVisualizeNormals;

public:

  //! Minimal requirements for all meshes, inherited
  struct Face{
    Face(const Vector3<float> & n = Vector3<float>(0,0,0),
         const Vector3<float> & c = Vector3<float>(0.5,0.1,0.7),
         float u = 0) : normal(n), color(c), curvature(u) {}
    Vector3<float> normal;
    Vector3<float> color;
    float curvature;
  };
  //! Minimal requirements for all meshes, inherited
  struct Vertex{
    Vertex(const Vector3<float> & p = Vector3<float>(0,0,0),
           const Vector3<float> & n = Vector3<float>(0,0,0),
           const Vector3<float> & c = Vector3<float>(0.5,0.1,0.7),
           float u = 0)
      : pos(p),
        normal(n),
        color(c),
        curvature(u) {}
    Vector3<float> pos;
    Vector3<float> normal;
    Vector3<float> color;
    float curvature;
  };

  Mesh() : mVisualizeNormals(false) { mVisualizationMode = CurvatureFace; }
  virtual ~Mesh() { }

  //! Adds a face to the mesh.
  virtual bool AddFace(const std::vector<Vector3<float> > &verts) = 0;

  //! Compute area of mesh
  virtual float Area() const;
  //! Compute volume of mesh
  virtual float Volume() const;

  //! Compute genus of mesh
  virtual int Genus() const;

  virtual void Dilate(float epsilon) { }

  virtual void SetColorMap(ColorMap * colormap) { GLObject::SetColorMap(colormap); Update(); }

  virtual void VisualizeNormals(bool flag = true) { mVisualizeNormals = flag; }

  virtual void SetVisualizationMode(const VisualizationMode & mode) { GLObject::SetVisualizationMode(mode); Update(); }

  virtual const char * GetTypeName() { return typeid(Mesh).name(); }
};


#endif
