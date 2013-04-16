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
#ifndef _DECIMATION_MESH
#define _DECIMATION_MESH

#include "Decimation/DecimationInterface.h"
#include "Geometry/HalfEdgeMesh.h"
#include "Util/Heap.h"
#include "Util/ColorMap.h"

class DecimationMesh : public DecimationInterface, public HalfEdgeMesh
{
public :

  static const VisualizationMode CollapseCost;
  virtual std::list<VisualizationMode> GetVisualizationModes() {
    std::list<VisualizationMode> L = Mesh::GetVisualizationModes();
    L.push_back(CollapseCost);
    return L;
  }

  DecimationMesh() : mNumCollapsedVerts(0), mNumCollapsedEdges(0), mNumCollapsedFaces(0) { }
  virtual ~DecimationMesh() { }

  /*! The EdgeCollapse is a Heapable type */
  struct EdgeCollapse : public Heap::Heapable
  {
    unsigned int halfEdge;
    Vector3<float> position;
  };

  virtual void Initialize();

  virtual void Update();

  virtual bool decimate();

  virtual bool decimate(unsigned int targetFaces);

  virtual void Render();

  virtual const char * GetTypeName() { return typeid(DecimationMesh).name(); }

protected :
  virtual void updateVertexProperties(unsigned int ind);

  virtual void updateFaceProperties(unsigned int ind);

  virtual void computeCollapse(EdgeCollapse * collapse) = 0;

  virtual void Cleanup();

  bool isValidCollapse(EdgeCollapse * collapse);

  inline bool isVertexCollapsed(unsigned int ind) { return mCollapsedVerts[ind]; }
  inline bool isEdgeCollapsed(unsigned int ind) { return mCollapsedEdges[ind]; }
  inline bool isFaceCollapsed(unsigned int ind) { return mCollapsedFaces[ind]; }

  inline void collapseVertex(unsigned int ind) {
    mCollapsedVerts[ind] = true;
    mNumCollapsedVerts++;
  }
  inline void collapseEdge(unsigned int ind) {
    mHalfEdge2EdgeCollapse[ind] = NULL;
    mCollapsedEdges[ind] = true;
    mNumCollapsedEdges++;
  }
  inline void collapseFace(unsigned int ind) {
    mCollapsedFaces[ind] = true;
    mNumCollapsedFaces++;
  }

  //! State array of 'active' verts
  std::vector<bool> mCollapsedVerts;
  //! State array of 'active' edges
  std::vector<bool> mCollapsedEdges;
  //! State array of 'active' faces
  std::vector<bool> mCollapsedFaces;

  //! Number of collapsed verts
  unsigned int mNumCollapsedVerts;
  //! Number of collapsed edges
  unsigned int mNumCollapsedEdges;
  //! Number of collapsed faces
  unsigned int mNumCollapsedFaces;

  //! Utility mapping between half edges and collapses
  std::vector<EdgeCollapse *> mHalfEdge2EdgeCollapse;

  //! The heap that stores the edge collapses
  Heap mHeap;

  void drawText(const Vector3<float> & pos, const char * str);

  virtual bool save(std::ostream &os){
    os << "# DecimationMesh obj streamer\n# M&A 2008\n\n";
    os << "# Vertices\n";
    for(unsigned int i=0; i<GetNumVerts(); i++){
      os << "v " << v(i).pos[0] << " " <<  v(i).pos[1] << " " <<  v(i).pos[2] << "\n";
    }
    os << "\n# Faces\n";
    for(unsigned int i=0; i<GetNumFaces(); i++){
      if(!isFaceCollapsed(i)){
          unsigned int ind = f(i).edge;
          os << "f ";
          os << e(ind).vert+1 << " ";
          ind = e(ind).next;
          os << e(ind).vert+1 << " ";
          ind = e(ind).next;
          os << e(ind).vert+1 << "\n";
      }
    }
    return os.good();
  }
};

#endif
