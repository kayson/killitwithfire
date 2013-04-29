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
#ifndef __implicit_h__
#define __implicit_h__

#include "Geometry/Geometry.h"
#include "Geometry/Bbox.h"
#include "Geometry/Mesh.h"
#include "Geometry/SimpleMesh.h"
#include "Util/MarchingCubes.h"

/*!  \brief Implicit base class */
class Implicit : public Geometry{
public:

  static const VisualizationMode Gradients;
  static const VisualizationMode Curvature;

  virtual std::list<VisualizationMode> GetVisualizationModes() {
    std::list<VisualizationMode> L = Geometry::GetVisualizationModes();
    L.push_back(Gradients);
    L.push_back(Curvature);
    return L;
  }

  Implicit();
  virtual ~Implicit();

  virtual void Update();
  virtual void Initialize();

  // function evaluation operators
  //! evaluate the implicit at world coordinates x y z
  virtual float GetValue(float x, float y, float z) const = 0;
  //! calculate the gradient of the implicit at world coordinates x y z
  virtual Vector3<float> GetGradient(float x, float y, float z) const;
  //! calculate the curvature of the implicit at world coordinates x y z
  virtual float GetCurvature(float x, float y, float z) const;

  //! Creates a drawable mesh by running marching cubes over the bounding box
  template <class MeshType>
  void Triangulate();

  //! Returns the mesh for outside manipulation. Decimation etc.
  Mesh * GetMesh() { return mMesh; }

  virtual void SetMeshSampling(float sampling) { mMeshSampling = sampling; }
  inline float GetMeshSampling() { return mMeshSampling; }

  void SetDifferentialScale(float delta) { mDelta = delta; std::cout << "Setting diff. scale: " << mDelta << std::endl; }
  inline float GetDifferentialScale() { return mDelta; }

  //! Returns axis aligned bounding box in world space
  Bbox GetBoundingBox() const;
  //! Sets the bounding box in current frame coordinates
  virtual void SetBoundingBox(const Bbox & b);

  //! Compute area of implicit
  virtual float ComputeArea(float dx = 0.01) const;

  //! Compute volume of implicit
  virtual float ComputeVolume(float dx = 0.01) const;

  //! Set transformation
  virtual void SetTransform(const Matrix4x4<float> & transform);

  virtual void SetOpacity(float opacity) { Geometry::SetOpacity(opacity); mMesh->SetOpacity(opacity); }

  virtual void Render();

  virtual const char * GetTypeName() { return typeid(Implicit).name(); }

  virtual void SetVisualizationMode(const VisualizationMode & source);

  virtual void SetColorMap(ColorMap * colormap);

  virtual bool save(std::ostream &os){
    if (mMesh != NULL)  return mMesh->save(os);
    return os.good();
  }

protected:
  void TransformW2O(float & x, float & y, float & z) const;

  Mesh * mMesh;
  Bbox mBox;
  Matrix4x4<float> mWorld2Obj;
  float mMeshSampling;
  float mDelta;
};


/*!
 * Marching cubes mesh extraction. Runs over the bounding box.
 */
template <class MeshType>
void Implicit::Triangulate()
{
  // Delete previous (old) mesh object
  if (mMesh != NULL) {
    delete mMesh;
    mMesh = NULL;
  }

  // Create new mesh object
  mMesh = new MeshType();

  // Get axis aligned bounding box (in world space)
  Bbox b = GetBoundingBox();
  Vector3<float> & pmin = b.pMin;
  Vector3<float> & pmax = b.pMax;

  std::cerr << b << std::endl;

  // Prepare progress bar
  unsigned int samplesX = static_cast<unsigned int>( (pmax[0] - pmin[0]) / mMeshSampling );
  unsigned int samplesY = static_cast<unsigned int>( (pmax[1] - pmin[1]) / mMeshSampling );
  unsigned int samplesZ = static_cast<unsigned int>( (pmax[2] - pmin[2]) / mMeshSampling );
  unsigned int totalSamples = samplesX*samplesY*samplesZ;
  unsigned int currentSample = 0;
  unsigned int reportFreq = totalSamples / 30;

  // Loop over bounding box
  std::cerr << "Triangulating [";
  for(float i = pmin[0]; i < pmax[0]-0.5*mMeshSampling; i += mMeshSampling ) {
    for(float j = pmin[1]; j < pmax[1]-0.5*mMeshSampling; j += mMeshSampling ) {
      for(float k = pmin[2]; k < pmax[2]-0.5*mMeshSampling; k += mMeshSampling ) {
        float voxelValues[8] = {
          GetValue(i,j,k),
          GetValue(i+mMeshSampling, j, k),
          GetValue(i+mMeshSampling, j+mMeshSampling, k),
          GetValue(i, j+mMeshSampling, k),
          GetValue(i, j, k+mMeshSampling),
          GetValue(i+mMeshSampling, j, k+mMeshSampling),
          GetValue(i+mMeshSampling, j+mMeshSampling, k+mMeshSampling),
          GetValue(i, j+mMeshSampling, k+mMeshSampling)
        };
        std::vector<Vector3<float> > tris = ::Triangulate(voxelValues, i, j, k, mMeshSampling);
        for(unsigned int n = 0; n < tris.size(); n+=3){
          std::vector<Vector3<float> > verts;
          std::copy(tris.begin() + n, tris.begin() + n + 3, std::back_inserter(verts));

          // Transform to local coordinates
          for (unsigned int v = 0; v < 3; v++)
            TransformW2O(verts[v][0], verts[v][1], verts[v][2]);

          mMesh->AddFace(verts);
        }

        currentSample++;
        if (currentSample % reportFreq == 0)
          std::cerr << "=";
      }
    }
  }
  std::cerr << "] done" << std::endl;
}


#endif
