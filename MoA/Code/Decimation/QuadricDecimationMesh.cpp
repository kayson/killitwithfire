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
#include "QuadricDecimationMesh.h"
#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related

const QuadricDecimationMesh::VisualizationMode QuadricDecimationMesh::QuadricIsoSurfaces = NewVisualizationMode("Quadric Iso Surfaces");

void QuadricDecimationMesh::Initialize()
{
  // Allocate memory for the quadric array
  unsigned int numVerts = mVerts.size();
  mQuadrics.reserve(numVerts);
  std::streamsize width = std::cerr.precision(); // store stream precision
  for (unsigned int i = 0; i < numVerts; i++) {

    // Compute quadric for vertex i here
    mQuadrics.push_back(createQuadricForVert(i));


    // Calculate initial error, should be numerically close to 0

    Vector3<float> v0 = mVerts[i].pos;
    Vector4<float> v(v0[0],v0[1],v0[2],1);
    Matrix4x4<float> m = mQuadrics.back();

    float error = v*(m*v);
    //std::cerr << std::scientific << std::setprecision(2) << error << " ";
  }
  std::cerr << std::setprecision(width) << std::fixed; // reset stream precision

  // Run the initialize for the parent class to initialize the edge collapses
  DecimationMesh::Initialize();
}

/*! \lab2 Implement the computeCollapse here */
/*!
 * \param[in,out] collapse The edge collapse object to (re-)compute, DecimationMesh::EdgeCollapse
 */
void QuadricDecimationMesh::computeCollapse(EdgeCollapse * collapse)
{
	// Compute collapse->position and collapse->cost here
	// based on the quadrics at the edge endpoints
	unsigned int e1 = collapse->halfEdge;
	unsigned int e2 = e(e1).pair;

	unsigned int v1 = e(e1).vert;
	unsigned int v2 = e(e2).vert;
	
	Matrix4x4<float> Q = mQuadrics.at(v1) + mQuadrics.at(v2);

	if(!Q.IsSingular())
	{
		Matrix4x4<float> QI = Q;
		QI(3,0) = 0; QI(3,1) = 0; QI(3,2) = 0; QI(3,3) = 1; 

		Vector4<float> inv = QI.Inverse()*Vector4<float>(0, 0, 0, 1);
		collapse->position = Vector3<float>(inv[0], inv[1], inv[2]);
	
		collapse->cost = inv * (Q* inv);
	}
	else
	{
		Vector3<float> v0 = (v(v1).pos + v(v2).pos)/2;
		Vector4<float> hv = Vector4<float>(v0[0], v0[1], v0[2], 1);
		collapse->position = v0;
		collapse->cost = hv * (Q* hv);
	}

	//Egen metod
	unsigned int f1 = mEdges[e1].face;
	unsigned int f2 = mEdges[e2].face;

	float c1 = v(v1).curvature; //f(f1).curvature;
	float c2 = v(v2).curvature; //f(f2).curvature;
	double area = 0;
	area += calcScreenSpaceArea(f1);
	area += calcScreenSpaceArea(f2);
	//collapse->position = (v(v1).pos + v(v2).pos)/2;
	
	Vector3<float> dir = Vector3<float>(0,0,1);
	Vector3<float> n1 = f(f1).normal; //v(v1).normal;
	Vector3<float> n2 = f(f2).normal; //v(v1).normal;

	if(n1*dir <= 0 && n2*dir <= 0) collapse->cost = (2 + n1*dir + n2*dir)/2.0 * area;
	else if((n1*dir*n2*dir) <= 0) collapse->cost =	(1 + abs(n1*dir - n2*dir)) * area;
	else collapse->cost = area;
}


/*! After each edge collapse the vertex properties need to be updated */
void QuadricDecimationMesh::updateVertexProperties(unsigned int ind)
{
  DecimationMesh::updateVertexProperties(ind);
  mQuadrics[ind] = createQuadricForVert(ind);
}

/*!
 * \param[in] indx vertex index, points into HalfEdgeMesh::mVerts
 */
Matrix4x4<float> QuadricDecimationMesh::createQuadricForVert(unsigned int indx) const{
	float q[4][4] = {{0,0,0,0},
					 {0,0,0,0},
					 {0,0,0,0},
					 {0,0,0,0}};
	Matrix4x4<float> Q(q);

	// The quadric for a vertex is the sum of all the quadrics for the adjacent faces
	// Tip: Matrix4x4 has an operator +=
	std::vector<unsigned int> faces = FindNeighborFaces(indx);
	for(unsigned int i = 0; i < faces.size(); ++i)
	{
		Q += createQuadricForFace(faces.at(i));
	}

	return Q;
}

/*!
 * \param[in] indx face index, points into HalfEdgeMesh::mFaces
 */
Matrix4x4<float> QuadricDecimationMesh::createQuadricForFace(unsigned int indx) const
{
  // Calculate the quadric (outer product of plane parameters) for a face
  // here using the formula from Garland and Heckbert
	Vector3<float> p = v(e(f(indx).edge).vert).pos;

	float a = f(indx).normal[0];
	float b = f(indx).normal[1];
	float c = f(indx).normal[2];
	float d = -(a*p[0] + b*p[1] + c*p[2]);
	
	float k[4][4] = {{a*a, b*a, c*a, d*a},
					 {a*b, b*b, c*b, d*b},
					 {a*c, b*c, c*c, d*c},
					 {a*d, b*d, c*d, d*d}};

	return Matrix4x4<float>(k);
}

//indx = faceindex
float  QuadricDecimationMesh::calcScreenSpaceArea(unsigned int indx)
{
	unsigned int e1 = f(indx).edge;
	unsigned int e2 = e(e1).next;
	unsigned int e3 = e(e2).next;

	Vector3<float> p1 = v(e(e1).vert).pos;
	Vector3<float> p2 = v(e(e2).vert).pos;
	Vector3<float> p3 = v(e(e3).vert).pos;

	GLint viewport[4]; //var to hold the viewport info
	GLdouble modelview[16]; //var to hold the modelview info
	GLdouble projection[16]; //var to hold the projection matrix info

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
	glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
	glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

	Vector3<double> sp1 = v(e(e1).vert).pos;
	Vector3<double> sp2 = v(e(e2).vert).pos;
	Vector3<double> sp3 = v(e(e3).vert).pos;

	//get the world coordinates from the screen coordinates
	//gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
	gluProject((GLdouble)p1[0],(GLdouble)p1[1],(GLdouble)p1[2],modelview,projection,viewport,&sp1[0],&sp1[1],&sp1[2]);
	sp1[1] = (float)viewport[3] - (float)sp1[1];

	gluProject((GLdouble)p2[0],(GLdouble)p2[1],(GLdouble)p2[2],modelview,projection,viewport,&sp2[0],&sp2[1],&sp2[2]);
	sp2[1] = (float)viewport[3] - (float)sp2[1];

	gluProject((GLdouble)p3[0],(GLdouble)p3[1],(GLdouble)p3[2],modelview,projection,viewport,&sp3[0],&sp3[1],&sp3[2]);
	sp3[1] = (float)viewport[3] - (float)sp3[1];

	sp1 = v(e(e1).vert).pos;
	sp2 = v(e(e2).vert).pos;
	sp3 = v(e(e3).vert).pos;

	return Cross((sp2-sp1), (sp3-sp1)).Length()*.5;
}

// indx = edge index
float QuadricDecimationMesh::calcScreenSpaceLength(unsigned int indx)
{
	Vector3<float> p1 = v(e(indx).vert).pos;
	Vector3<float> p2 = v(e(e(indx).pair).vert).pos;

	GLint viewport[4]; //var to hold the viewport info
	GLdouble modelview[16]; //var to hold the modelview info
	GLdouble projection[16]; //var to hold the projection matrix info

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
	glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
	glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

	Vector3<double> sp1;
	Vector3<double> sp2;

	//get the world coordinates from the screen coordinates
	//gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
	gluProject((GLdouble)p1[0],(GLdouble)p1[1],(GLdouble)p1[2],modelview,projection,viewport,&sp1[0],&sp1[1],&sp1[2]);
	sp1[1] = (float)viewport[3] - (float)sp1[1];

	gluProject((GLdouble)p2[0],(GLdouble)p2[1],(GLdouble)p2[2],modelview,projection,viewport,&sp2[0],&sp2[1],&sp2[2]);
	sp2[1] = (float)viewport[3] - (float)sp2[1];

	return (sp1 - sp2).Length();
}

void QuadricDecimationMesh::Render()
{
	DecimationMesh::Render();

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);

	if (mVisualizationMode == QuadricIsoSurfaces)
    {
		// Apply transform
		glPushMatrix(); // Push modelview matrix onto stack

		// Implement the quadric visualization here
		//std::cout << "Quadric visualization not implemented" << std::endl;
		
		float error = 1;
		for(unsigned int i = 0; i < mQuadrics.size(); ++i)
		{
			if(!isVertexCollapsed(i))
			{
			Matrix4x4<float> Q = mQuadrics.at(i);
			Matrix4x4<float> R;

			if(Q.CholeskyFactorization(R))
			{
				if(!R.IsSingular())
				{
					R = R.Inverse();

					glPushMatrix();
						Matrix4x4<float> R_GL = R.ToGLMatrix();

						//glTranslatef(mVerts.at(i).pos[0], mVerts.at(i).pos[1], mVerts.at(i).pos[2]);
						glMultMatrixf(R_GL.GetArrayPtr());
						glColor3d(1,0,0);
						glutSolidSphere(error, 10, 10);

					glPopMatrix();
				}
			}
			}
		}

		// Restore modelview matrix
		glPopMatrix();
    }
}

