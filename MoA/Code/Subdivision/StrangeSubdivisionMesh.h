#ifndef _strange_dubdivmesh_
#define _strange_dubdivmesh_

#include "AdaptiveLoopSubdivisionMesh.h"
#include <iostream>

class StrangeSubdivisionMesh : public AdaptiveLoopSubdivisionMesh
{
public:
  virtual void Subdivide() {
    // ....
	unsigned int faces = mFaces.size();
    AdaptiveLoopSubdivisionMesh::Subdivide();

	std::cout << "Number of faces before and after sub: " << faces << ", " << mFaces.size() << std::endl <<  std::endl;
  }

protected:


	bool Subdividable(unsigned int fi)
	{
		// Every 4th face is not subdividable - kinda strange!
		// Do something more interesting...

		// find neighbor faces
		unsigned int f1, f2, f3;
		EdgeIterator eit = GetEdgeIterator( f(fi).edge );
		f1 = eit.Pair().GetEdgeFaceIndex(); eit.Pair();
		f2 = eit.Next().Pair().GetEdgeFaceIndex(); eit.Pair();
		f3 = eit.Next().Pair().GetEdgeFaceIndex();

		Vector3<float> n = f(fi).normal;
		Vector3<float> n1 = f(f1).normal;
		Vector3<float> n2 = f(f2).normal;
		Vector3<float> n3 = f(f3).normal;

		float m = maxAngle(fi);
		if(maxAngle(f1) < m) m = maxAngle(f1);
		if(maxAngle(f2) < m) m = maxAngle(f2);
		if(maxAngle(f3) < m) m = maxAngle(f3);

		return m < 0.98;
	}

	//max == 1 => 0 grader
	//max = -1 => -180 grader
	float maxAngle(unsigned int fi) const
	{
		unsigned int f1, f2, f3;
		EdgeIterator eit = GetEdgeIterator( f(fi).edge );
		f1 = eit.Pair().GetEdgeFaceIndex(); eit.Pair();
		f2 = eit.Next().Pair().GetEdgeFaceIndex(); eit.Pair();
		f3 = eit.Next().Pair().GetEdgeFaceIndex();

		Vector3<float> n = f(fi).normal;
		Vector3<float> n1 = f(f1).normal;
		Vector3<float> n2 = f(f2).normal;
		Vector3<float> n3 = f(f3).normal;

		float max = n*n1;
		if(n*n2 < max) max = n*n2;
		if(n*n3 < max) max = n*n3;

		return max;
	}
};

#endif
