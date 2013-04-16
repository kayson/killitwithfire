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

#include "LoopSubdivisionMesh.h"
#include <cassert>

/*! Subdivides the mesh uniformly one step
*/
void LoopSubdivisionMesh::Subdivide()
{
  // Create new mesh and copy all the attributes
  HalfEdgeMesh subDivMesh;
  subDivMesh.SetTransform(GetTransform());
  subDivMesh.SetName(GetName());
  subDivMesh.SetColorMap(GetColorMap());
  subDivMesh.SetWireframe(GetWireframe());
  subDivMesh.SetShowNormals(GetShowNormals());
  subDivMesh.SetOpacity(GetOpacity());
  if (IsHovering()) subDivMesh.Hover();
  if (IsSelected()) subDivMesh.Select();
  subDivMesh.mMinCMap = mMinCMap;
  subDivMesh.mMaxCMap = mMaxCMap;
  subDivMesh.mAutoMinMax = mAutoMinMax;


  // loop over each face and create 4 new ones
  for (unsigned int i=0; i<mFaces.size(); i++){
    // subdivide face
    std::vector< std::vector<Vector3<float> > > faces = Subdivide(i);

    // add new faces to subDivMesh
    for(unsigned int j=0; j<faces.size(); j++){
      subDivMesh.AddFace(faces.at(j));
    }
  }

  // Assigns the new mesh
  *this = LoopSubdivisionMesh(subDivMesh, ++mNumSubDivs);
  Update();
  std::cout << "Number of faces = " << mFaces.size() << std::endl;
}


/*! Subdivides the face at faceindex into a vector of faces
*/
std::vector< std::vector<Vector3<float> > > LoopSubdivisionMesh::Subdivide(unsigned int faceIndex)
{
  std::vector< std::vector<Vector3<float> > > faces;
  EdgeIterator eit = GetEdgeIterator( f(faceIndex).edge );

  // get the inner halfedges
  unsigned int e0, e1, e2;
  // and their vertex indices
  unsigned int v0, v1, v2;

  e0 = eit.GetEdgeIndex();
  v0 = eit.GetEdgeVertexIndex();
  eit.Next();
  e1 = eit.GetEdgeIndex();
  v1 = eit.GetEdgeVertexIndex();
  eit.Next();
  e2 = eit.GetEdgeIndex();
  v2 = eit.GetEdgeVertexIndex();

  // Compute positions of the vertices
  Vector3<float> pn0 = VertexRule(v0);
  Vector3<float> pn1 = VertexRule(v1);
  Vector3<float> pn2 = VertexRule(v2);

  // Compute positions of the edge vertices
  Vector3<float> pn3 = EdgeRule(e0);
  Vector3<float> pn4 = EdgeRule(e1);
  Vector3<float> pn5 = EdgeRule(e2);

  // add the four new triangles to new mesh
  std::vector<Vector3<float> > verts;
  verts.push_back(pn0); verts.push_back(pn3); verts.push_back(pn5);
  faces.push_back(verts);
  verts.clear();
  verts.push_back(pn3); verts.push_back(pn4); verts.push_back(pn5);
  faces.push_back(verts);
  verts.clear();
  verts.push_back(pn3); verts.push_back(pn1); verts.push_back(pn4);
  faces.push_back(verts);
  verts.clear();
  verts.push_back(pn5); verts.push_back(pn4); verts.push_back(pn2);
  faces.push_back(verts);
  return faces;
}


/*! Computes a new vertex, replacing a vertex in the old mesh
*/
Vector3<float> LoopSubdivisionMesh::VertexRule(unsigned int vertexIndex)
{
	// Get the current vertex
	Vector3<float> vtx = v(vertexIndex).pos;
	/*
	std::vector<unsigned int> nbors = FindNeighborVertices(vertexIndex);
	float beta = 0;
	unsigned int k = nbors.size();
	if(k == 3)
	{
		beta = 3.0/16.0;
	}
	else if(k > 3)
	{
		beta = 3.0/(8.0*(double)k);
	}
	else//Boundry?
	{
		beta = 1.0/8.0;
	}

	vtx *= (1.0-(double)k*beta);
	for(unsigned int i = 0; i < k; ++i)
	{
		vtx += beta*v(nbors.at(i)).pos;
	}*/

	return vtx;
}


/*! Computes a new vertex, placed along an edge in the old mesh
*/
Vector3<float> LoopSubdivisionMesh::EdgeRule(unsigned int edgeIndex)
{
	// Place the edge vertex halfway along the edge
	HalfEdge & e0 = e(edgeIndex);
	HalfEdge & e1 = e(e0.pair);
	Vector3<float> & v0 = v(e0.vert).pos;
	Vector3<float> & v1 = v(e1.vert).pos;

	if(e0.face != BORDER && e1.face != BORDER)
	{
		Vector3<float> & v2 = v(e(e0.prev).vert).pos;
		Vector3<float> & v3 = v(e(e1.prev).vert).pos;

		return 1.0/8.0*((v2+v3) + 3.0*(v0 + v1));
	}
	else//boundry?
	{
		return (v0 + v1)/2.0;
	}
}

//! Return weights for interior verts
float LoopSubdivisionMesh::Beta(unsigned int valence){
  if(valence == 6){
    return 1.f / 16.f;
  } else if(valence == 3){
    return 3.f / 16.f;
  } else{
    return  3.f / (8.f * valence);
  }
}
