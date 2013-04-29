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
#include "Implicit.h"

#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

const Implicit::VisualizationMode Implicit::Gradients = NewVisualizationMode("Gradients");
const Implicit::VisualizationMode Implicit::Curvature = NewVisualizationMode("Curvature");

Implicit::Implicit() : mMesh(NULL), mMeshSampling(0.1), mDelta(0.1) {}

Implicit::~Implicit()
{
  if (mMesh != NULL) {
    delete mMesh;
    mMesh = NULL;
  }
}


void Implicit::Update()
{
  if (mVisualizationMode == Curvature) {
    if(typeid(*mMesh) == typeid(SimpleMesh)) {

      SimpleMesh * ptr = static_cast<SimpleMesh*>(mMesh);
      std::vector<SimpleMesh::Vertex>& verts = ptr->GetVerts();

      Matrix4x4<float> M = GetTransform().Transpose();

      // Compute curvature of implicit geometry and assign to the vertex property
      for(unsigned int i=0; i < verts.size(); i++){
        const Vector3<float> vObject = verts.at(i).pos;

        // Transform vertex position to world space
        Vector4<float> vWorld = GetTransform() * Vector4<float>(vObject[0],vObject[1],vObject[2],1);

        // Get curvature in world space
        verts.at(i).curvature = GetCurvature(vWorld[0], vWorld[1], vWorld[2]);

        // Get gradient in world space (used for lighting)
        Vector3<float> nWorld = GetGradient(vWorld[0], vWorld[1], vWorld[2]);

        // Transform gradient to object space
        Vector4<float> nObject = M * Vector4<float>(nWorld[0],nWorld[1],nWorld[2],0);
        verts.at(i).normal = Vector3<float>(nObject[0], nObject[1], nObject[2]).Normalize();
      }

      ptr->mAutoMinMax = mAutoMinMax;
      ptr->mMinCMap = mMinCMap;
      ptr->mMaxCMap = mMaxCMap;
      ptr->SetVisualizationMode(Mesh::CurvatureVertex);
      ptr->Update();
    }
    else {
      std::cerr << "No Curvature visualization mode implemented for mesh type: " << typeid(*mMesh).name() << std::endl;
    }
  }
}


void Implicit::Initialize() {
  Geometry * mesh = dynamic_cast<Geometry *>(mMesh);
  if (mesh == NULL)
    std::cerr << "Error: implicit geometry not triangulated, add call to triangulate()" << std::endl;
  else {
    std::cerr << "Computing normals etc... ";
    mesh->Initialize();
    std::cerr << " done" << std::endl;
  }
}


/*!
 * Evaluates gradient at (x,y,z) through discrete finite difference scheme.
 */
Vector3<float> Implicit::GetGradient(float x, float y, float z) const
{
	// Implement finite difference evaluation of gradient at world coordinates (x,y,z)
	// Use mDelta variable as epsilon in eqn. 16 in lab text
	float h = mDelta;
	float Dx = (GetValue(x+h, y, z) - GetValue(x-h, y, z))/(2.0*h);
	float Dy = (GetValue(x, y+h, z) - GetValue(x, y-h, z))/(2.0*h);
	float Dz = (GetValue(x, y, z+h) - GetValue(x, y, z-h))/(2.0*h);

	return Vector3<float>(Dx, Dy, Dz);
}


/*!
 * Evaluates curvature at (x,y,z) through discrete finite difference scheme.
 */
float Implicit::GetCurvature(float x, float y, float z) const
{
  // Implement finite difference evaluation of curvature at world coordinates (x,y,z)
  // Use mDelta variable as epsilon in eqn. 16 in lab text
	float h = mDelta;

	float D2x = (GetValue(x+h, y, z) - 2.0*GetValue(x, y, z) + GetValue(x-h, y, z))/(h*h);
	float D2y = (GetValue(x, y+h, z) - 2.0*GetValue(x, y, z) + GetValue(x, y-h, z))/(h*h);
	float D2z = (GetValue(x, y, z+h) - 2.0*GetValue(x, y, z) + GetValue(x, y, z-h))/(h*h);

	return (D2x + D2y + D2z)/(2.0*GetGradient(x, y, z).Length());
}


float Implicit::ComputeArea(float dx) const
{
  return 0;
}


float Implicit::ComputeVolume(float dx) const
{
  Bbox box = GetBoundingBox();
  float volume = 0;

  float H;
  for (float x = box.pMin[0]; x <= box.pMax[0] + 0.5*dx; x += dx) {
    for (float y = box.pMin[1]; y <= box.pMax[1] + 0.5*dx; y += dx) {
      for (float z = box.pMin[2]; z <= box.pMax[2] + 0.5*dx; z += dx) {
        float val = GetValue(x,y,z);
        if (val < -dx)      H = 1;
        else if (val > dx)  H = 0;
        else
          H = 0.5*(1 + val/dx + sin(-M_PI*val/dx)/M_PI);

        volume += H;
      }
    }
  }

  return volume * std::pow(dx, 3);
}


Bbox Implicit::GetBoundingBox() const
{
  // transform returns a copy
  return mBox.Transform(GetTransform());
}


void Implicit::SetBoundingBox(const Bbox &b)
{
  mBox = b.Transform(mWorld2Obj);
}


void Implicit::SetTransform(const Matrix4x4<float> & transform) {
  Geometry::SetTransform(transform);
  mWorld2Obj = GetTransform().Inverse();
}


void Implicit::TransformW2O(float & x, float & y, float & z) const
{
  Vector4<float> vprim, v = Vector4<float>(x, y, z, 1.f);
  vprim = mWorld2Obj*v;
  x = vprim[0]; y = vprim[1]; z = vprim[2];
}


void Implicit::Render()
{
  // Draw bounding box for debugging
  Bbox b = GetBoundingBox();

  Vector3<float> & v0 = b.pMin;
  Vector3<float> & v1 = b.pMax;

  if(mSelected) {
    glLineWidth(2.0f);
    glColor3f(0.8f,0.8f,0.8f);
  }
  else glColor3f(0.1f,0.1f,0.1f);

  glBegin(GL_LINE_STRIP);
  glVertex3f(v0[0], v0[1], v0[2]);
  glVertex3f(v1[0], v0[1], v0[2]);
  glVertex3f(v1[0], v1[1], v0[2]);
  glVertex3f(v0[0], v1[1], v0[2]);
  glVertex3f(v0[0], v0[1], v0[2]);
  glEnd();

  glBegin(GL_LINE_STRIP);
  glVertex3f(v0[0], v0[1], v1[2]);
  glVertex3f(v1[0], v0[1], v1[2]);
  glVertex3f(v1[0], v1[1], v1[2]);
  glVertex3f(v0[0], v1[1], v1[2]);
  glVertex3f(v0[0], v0[1], v1[2]);
  glEnd();

  glBegin(GL_LINES);
  glVertex3f(v0[0], v0[1], v0[2]);
  glVertex3f(v0[0], v0[1], v1[2]);

  glVertex3f(v1[0], v0[1], v0[2]);
  glVertex3f(v1[0], v0[1], v1[2]);

  glVertex3f(v0[0], v1[1], v0[2]);
  glVertex3f(v0[0], v1[1], v1[2]);

  glVertex3f(v1[0], v1[1], v0[2]);
  glVertex3f(v1[0], v1[1], v1[2]);
  glEnd();
  glLineWidth(1.0f);
  glPushMatrix();
  glMultMatrixf(mTransform.ToGLMatrix().GetArrayPtr());

  Geometry * mesh = dynamic_cast<Geometry *>(mMesh);
  if (mesh == NULL)
    std::cerr << "Error: implicit geometry not triangulated, add call to triangulate()" << std::endl;
  else {
    mesh->SetShowNormals(mShowNormals);
    mesh->SetWireframe(mWireframe);
    mesh->SetOpacity(mOpacity);

    mesh->Render();
  }

  if (mVisualizationMode == Gradients) {
    if(typeid(*mMesh) == typeid(SimpleMesh)){

      SimpleMesh * ptr = static_cast<SimpleMesh*>(mMesh);
      const std::vector<SimpleMesh::Vertex>& verts = ptr->GetVerts();

      glDisable(GL_LIGHTING);

      Matrix4x4<float> M = GetTransform().Transpose();

      glColor3f(0, 0, 1);
      glBegin(GL_LINES);
      for(unsigned int i=0; i < verts.size(); i++){
        const Vector3<float> vObject = verts.at(i).pos;

        // Transform vertex position to world space
        Vector4<float> vWorld = GetTransform() * Vector4<float>(vObject[0],vObject[1],vObject[2],1);

        // Get gradient in world space
        Vector3<float> nWorld = GetGradient(vWorld[0], vWorld[1], vWorld[2]);

        // Transform gradient to object space
        Vector4<float> nObject = M * Vector4<float>(nWorld[0],nWorld[1],nWorld[2],0);
        Vector3<float> n = Vector3<float>(nObject[0], nObject[1], nObject[2]);

        glVertex3fv(vObject.GetArrayPtr());
        glVertex3fv((vObject + n*0.1).GetArrayPtr());
      }
      glEnd();
    }
    else {
      std::cerr << "No Gradient visualization mode implemented for mesh type: " << typeid(*mMesh).name() << std::endl;
    }
  }

  glPopMatrix();

  GLObject::Render();
}


void Implicit::SetVisualizationMode(const VisualizationMode & source)
{
  Geometry::SetVisualizationMode(source);
  Update();
}


void Implicit::SetColorMap(ColorMap * colormap)
{
  Geometry * mesh = dynamic_cast<Geometry *>(mMesh);
  if (mesh != NULL)
    mesh->SetColorMap(colormap);

  Geometry::SetColorMap(colormap);
  Update();
}

