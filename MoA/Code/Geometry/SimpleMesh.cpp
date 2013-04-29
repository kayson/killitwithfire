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
#include "Geometry/SimpleMesh.h"
#include "Util/ColorMap.h"

//-----------------------------------------------------------------------------
SimpleMesh::SimpleMesh()
{
}

//-----------------------------------------------------------------------------
SimpleMesh::~SimpleMesh()
{
}

//-----------------------------------------------------------------------------
bool SimpleMesh::AddFace(const std::vector<Vector3<float> > &verts){

  unsigned int ind1, ind2, ind3;
  AddVertex(verts.at(0), ind1);
  AddVertex(verts.at(1), ind2);
  AddVertex(verts.at(2), ind3);

  Face tri(ind1, ind2, ind3);
  mFaces.push_back(tri);
  // Compute and assign a normal
  mFaces.back().normal = FaceNormal(mFaces.size() - 1);

  return true;
}

//-----------------------------------------------------------------------------
bool SimpleMesh::AddVertex(const Vector3<float> & v, unsigned int &indx){
  std::map<Vector3<float>,unsigned int>::iterator it = mUniqueVerts.find(v);
  if (it != mUniqueVerts.end()){
    indx = (*it).second; // (*it).second == it->second, which to me was not as clear...
    return false;
  }

  mUniqueVerts[v] = indx = mVerts.size(); // op. [ ] constructs a new entry in map
  Vertex vert;
  vert.pos = v;
  mVerts.push_back(vert);

  return true;
}

//-----------------------------------------------------------------------------
Vector3<float> SimpleMesh::FaceNormal(unsigned int faceIndex) const{
  const Face& tri = mFaces.at(faceIndex);
  Vector3<float> e1 = mVerts.at(tri.v2).pos - mVerts.at(tri.v1).pos;
  Vector3<float> e2 = mVerts.at(tri.v3).pos - mVerts.at(tri.v1).pos;
  return Cross(e1, e2).Normalize();
}
//-----------------------------------------------------------------------------
Vector3<float> SimpleMesh::VertexNormal(unsigned int vertexIndex) const{

  std::vector<unsigned int> neighborFaces = FindNeighborFaces(vertexIndex);

  Vector3<float> n(0,0,0);

  const unsigned int numCandidates = neighborFaces.size();
  for (unsigned int i = 0; i < numCandidates; i++){
    const Face& triangle = mFaces.at(neighborFaces.at(i));

    // NB Assumes face normals already calculated
    n += triangle.normal;
  }
  n.Normalize();
  return n;
}
//-----------------------------------------------------------------------------
float SimpleMesh::VertexCurvature(unsigned int vertexIndex) const{

  std::vector<unsigned int> oneRing = FindNeighborVertices(vertexIndex);
  assert(oneRing.size() != 0);

  unsigned int curr, next;
  const Vector3<float> &vi = mVerts.at(vertexIndex).pos;
  float angleSum = 0;
  float area = 0;
  for(unsigned int i=0; i<oneRing.size(); i++){
    // connections
    curr = oneRing.at(i);
    if(i < oneRing.size() - 1 )
      next = oneRing.at(i+1);
    else
      next = oneRing.front();

    // find vertices in 1-ring according to figure 5 in lab text
    // next - beta
    const Vector3<float> &nextPos = mVerts.at(next).pos;
    const Vector3<float> &vj = mVerts.at(curr).pos;

    // compute angle and area
    angleSum +=  acos( (vj-vi)*(nextPos-vi) / ( (vj-vi).Length()*(nextPos-vi).Length() ) );
    area += Cross((vi-vj), (nextPos-vj)).Length()*.5;
  }
  return ( 2*M_PI - angleSum ) / area;
}

float SimpleMesh::FaceCurvature(unsigned int faceIndex) const{
  // NB Assumes vertex curvature already computed
  const Face& tri = mFaces.at(faceIndex);
  return (mVerts.at(tri.v1).curvature + mVerts.at(tri.v2).curvature + mVerts.at(tri.v3).curvature) / 3.f;
}

/*! Loops over the neighborhood of a vertex and collects all the vertices sorted counter clockwise.
 * \param [in] vertexIndex  the index to vertex, unsigned int
 * \return a vector containing the indices to all the found vertices.
 */
std::vector<unsigned int> SimpleMesh::FindNeighborVertices(unsigned int vertexIndex) const{
  std::vector<unsigned int> neighborFaces = FindNeighborFaces(vertexIndex);
  std::vector<unsigned int> oneRing;
  unsigned int currVert;

  // pick next counter clock wise vert
  if(mFaces.at(neighborFaces.at(0)).v1 == vertexIndex) currVert = mFaces.at(neighborFaces.at(0)).v2;
  if(mFaces.at(neighborFaces.at(0)).v2 == vertexIndex) currVert = mFaces.at(neighborFaces.at(0)).v3;
  if(mFaces.at(neighborFaces.at(0)).v3 == vertexIndex) currVert = mFaces.at(neighborFaces.at(0)).v1;
  oneRing.push_back(currVert);

  // collect one ring vertices
  for(unsigned int i=0; i< neighborFaces.size() - 1; i++){
    if(mFaces.at(neighborFaces.at(i)).v1 == currVert)
      currVert = mFaces.at(neighborFaces.at(i)).v2;
    else if(mFaces.at(neighborFaces.at(i)).v2 == currVert)
      currVert = mFaces.at(neighborFaces.at(i)).v3;
    else if(mFaces.at(neighborFaces.at(i)).v3 == currVert)
      currVert = mFaces.at(neighborFaces.at(i)).v1;
    oneRing.push_back(currVert);
  }
  return oneRing;
}

/*! Loops over the neighborhood of a vertex and collects all the faces sorted counter clockwise.
 * \param [in] vertexIndex  the index to vertex, unsigned int
 * \return a vector containing the indices to all the found faces.
 */
std::vector<unsigned int> SimpleMesh::FindNeighborFaces(unsigned int vertexIndex) const{
  std::vector<unsigned int> foundFaces;

  // Find other triangles that include this vertex
  std::vector<Face>::const_iterator iter = mFaces.begin();
  std::vector<Face>::const_iterator iend = mFaces.end();
  while (iter != iend) {
    if ((*iter).v1 == vertexIndex ||
      (*iter).v2 == vertexIndex ||
      (*iter).v3 == vertexIndex)
      {
        foundFaces.push_back( iter - mFaces.begin());
      }
    iter++;
  }

  // Pick prev vertex
  unsigned int currVertex = 0;
  const unsigned int v1 = mFaces.at(foundFaces.at(0)).v1;
  const unsigned int v2 = mFaces.at(foundFaces.at(0)).v2;
  const unsigned int v3 = mFaces.at(foundFaces.at(0)).v3;
  if(vertexIndex == v1) currVertex = v3;
  else if(vertexIndex == v2) currVertex = v1;
  else if(vertexIndex == v3) currVertex = v2;

  for (unsigned int i=1; i<foundFaces.size()-1; i++){
    for(unsigned int j=i; j<foundFaces.size(); j++){
      if (mFaces.at(foundFaces.at(j)).v1 == currVertex){
        // pick the next vert
        currVertex = mFaces.at(foundFaces.at(j)).v2;
        // and swap
        std::swap(foundFaces.at(i), foundFaces.at(j));
        break;
      }
      if (mFaces.at(foundFaces.at(j)).v2 == currVertex){
        // pick the next vert
        currVertex = mFaces.at(foundFaces.at(j)).v3;
        // and swap
        std::swap(foundFaces.at(i), foundFaces.at(j));
        break;
      }
      if (mFaces.at(foundFaces.at(j)).v3 == currVertex){
        // pick the next vert
        currVertex = mFaces.at(foundFaces.at(j)).v1;
        // and swap
        std::swap(foundFaces.at(i), foundFaces.at(j));
        break;
      }
    }
  }

  return foundFaces;
}

//-----------------------------------------------------------------------------
void SimpleMesh::Initialize(){
  // Calculate and store all differentials and area

  // First update all face normals and triangle areas
  for(unsigned int i = 0; i < mFaces.size(); i++){
    mFaces.at(i).normal = FaceNormal(i);
  }
  // Then update all vertex normals and curvature
  for(unsigned int i = 0; i < mVerts.size(); i++){
    // Vertex normals are just weighted averages
    mVerts.at(i).normal = VertexNormal(i);
  }

  // Then update vertex curvature
  for(unsigned int i = 0; i < mVerts.size(); i++){
    mVerts.at(i).curvature = VertexCurvature(i);
    // std::cerr <<   mVerts.at(i).curvature << "\n";
  }

  // Finally update face curvature
  for(unsigned int i = 0; i < mFaces.size(); i++){
    mFaces.at(i).curvature = FaceCurvature(i);
  }
}


//-----------------------------------------------------------------------------
void SimpleMesh::Update(){

  // Update vertex and face colors
  if (mVisualizationMode == CurvatureVertex) {
    std::vector<Vertex>::iterator iter = mVerts.begin();
    std::vector<Vertex>::iterator iend = mVerts.end();
    float minCurvature = (std::numeric_limits<float>::max)();
    float maxCurvature = -(std::numeric_limits<float>::max)();
    while (iter != iend) {
      if (minCurvature > (*iter).curvature)  minCurvature = (*iter).curvature;
      if (maxCurvature < (*iter).curvature)  maxCurvature = (*iter).curvature;
      iter++;
    }
    if(mAutoMinMax == false){
      std::cerr << "Mapping color based on vertex curvature with range [" << mMinCMap << "," << mMaxCMap << "]" << std::endl;
      minCurvature = mMinCMap;
      maxCurvature = mMaxCMap;
    }
    else {
      std::cerr << "Automatic mapping of color based on vertex curvature with range [" << minCurvature << "," << maxCurvature << "]" << std::endl;
      mMinCMap = minCurvature;
      mMaxCMap = maxCurvature;
    }
    iter = mVerts.begin();
    while (iter != iend) {
      (*iter).color = mColorMap->Map((*iter).curvature, minCurvature, maxCurvature);
      iter++;
    }
  }
  else if (mVisualizationMode == CurvatureFace) {
    std::vector<Face>::iterator iter = mFaces.begin();
    std::vector<Face>::iterator iend = mFaces.end();
    float minCurvature = (std::numeric_limits<float>::max)();
    float maxCurvature = -(std::numeric_limits<float>::max)();
    while (iter != iend) {
      if (minCurvature > (*iter).curvature)  minCurvature = (*iter).curvature;
      if (maxCurvature < (*iter).curvature)  maxCurvature = (*iter).curvature;
      iter++;
    }
    std::cerr << "Mapping color based on face curvature with range [" << minCurvature << "," << maxCurvature << "]" << std::endl;
    iter = mFaces.begin();
    while (iter != iend) {
      (*iter).color = mColorMap->Map((*iter).curvature, minCurvature, maxCurvature);
      iter++;
    }
  }

}


int SimpleMesh::Genus() const {
  std::set<MyEdge> uniqueEdges;
  for(unsigned int i=0; i<mFaces.size(); i++){
    uniqueEdges.insert(MyEdge(mFaces[i].v1, mFaces[i].v2));
    uniqueEdges.insert(MyEdge(mFaces[i].v1, mFaces[i].v3));
    uniqueEdges.insert(MyEdge(mFaces[i].v2, mFaces[i].v3));
  }
  int E = uniqueEdges.size();
  int V = mVerts.size();
  int F = mFaces.size();

  std::cerr << "Number of edges: " << E << ", F: " << F << ", V: " << V << "\n";
  return -(V-E+F-2)/2;
}


void SimpleMesh::Dilate(float amount)
{
  std::vector<Vertex>::iterator iter = mVerts.begin();
  std::vector<Vertex>::iterator iend = mVerts.end();
  while (iter != iend) {
    (*iter).pos += amount*(*iter).normal;
    iter++;
  }

  Initialize();
  Update();
}

void SimpleMesh::Erode(float amount)
{
  std::vector<Vertex>::iterator iter = mVerts.begin();
  std::vector<Vertex>::iterator iend = mVerts.end();
  while (iter != iend) {
    (*iter).pos -= amount*(*iter).normal;
    iter++;
  }

  Initialize();
  Update();
}

void SimpleMesh::Smooth(float amount)
{
  std::vector<Vertex>::iterator iter = mVerts.begin();
  std::vector<Vertex>::iterator iend = mVerts.end();
  while (iter != iend) {
    (*iter).pos -= amount*(*iter).normal * (*iter).curvature;
    iter++;
  }

  Initialize();
  Update();
}

void SimpleMesh::Render()
{
  glEnable(GL_LIGHTING);
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Apply transform
  glPushMatrix(); // Push modelview matrix onto stack

  // Convert transform-matrix to format matching GL matrix format
  // Load transform into modelview matrix
  glMultMatrixf( mTransform.ToGLMatrix().GetArrayPtr() );

  if (mWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Draw geometry
  glBegin(GL_TRIANGLES);
  const int numFaces = mFaces.size();
  for (int i = 0; i < numFaces; i++){
    Face& triangle = mFaces[i];

    Vector3<float>& p0 = mVerts[triangle.v1].pos;
    Vector3<float>& p1 = mVerts[triangle.v2].pos;
    Vector3<float>& p2 = mVerts[triangle.v3].pos;

    if (mVisualizationMode == CurvatureVertex) {
      Vector3<float>& c1 = mVerts.at(triangle.v1).color;
      glColor4f(c1[0], c1[1], c1[2], mOpacity);
      glNormal3fv(mVerts.at(triangle.v1).normal.GetArrayPtr());
      glVertex3fv(p0.GetArrayPtr());

      Vector3<float>& c2 = mVerts.at(triangle.v2).color;
      glColor4f(c2[0], c2[1], c2[2], mOpacity);
      glNormal3fv(mVerts.at(triangle.v2).normal.GetArrayPtr());
      glVertex3fv(p1.GetArrayPtr());

      Vector3<float>& c3 = mVerts.at(triangle.v3).color;
      glColor4f(c3[0], c3[1], c3[2], mOpacity);
      glNormal3fv(mVerts.at(triangle.v3).normal.GetArrayPtr());
      glVertex3fv(p2.GetArrayPtr());
    }
    else {
      Vector3<float>& color = triangle.color;
      glColor4f(color[0], color[1], color[2], mOpacity);
      glNormal3fv(triangle.normal.GetArrayPtr());

      glVertex3fv(p0.GetArrayPtr());
      glVertex3fv(p1.GetArrayPtr());
      glVertex3fv(p2.GetArrayPtr());
    }

  }
  glEnd();

  // Mesh normals by courtesy of Richard Khoury
  if (mShowNormals)
  {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < mFaces.size(); i++){

      Face & face = mFaces.at(i);

      Vertex & v1 = mVerts.at(face.v1);
      Vertex & v2 = mVerts.at(face.v2);
      Vertex & v3 = mVerts.at(face.v3);

      Vector3<float> faceStart = (v1.pos + v2.pos + v3.pos) / 3.0f;
      Vector3<float> faceEnd = faceStart + face.normal*0.1;

      glColor3f(1,0,0); // Red for face normal
      glVertex3fv(faceStart.GetArrayPtr());
      glVertex3fv(faceEnd.GetArrayPtr());

      glColor3f(0,1,0); // Vertex normals in Green
      glVertex3fv(v1.pos.GetArrayPtr());
      glVertex3fv((v1.pos + v1.normal*0.1).GetArrayPtr());
      glVertex3fv(v2.pos.GetArrayPtr());
      glVertex3fv((v2.pos + v2.normal*0.1).GetArrayPtr());
      glVertex3fv(v3.pos.GetArrayPtr());
      glVertex3fv((v3.pos + v3.normal*0.1).GetArrayPtr());
    }
    glEnd();
    glEnable(GL_LIGHTING);

  }

  if (mWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Restore modelview matrix
  glPopMatrix();

  GLObject::Render();

}
