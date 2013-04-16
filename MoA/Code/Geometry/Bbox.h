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
#ifndef __bbox_h__
#define __bbox_h__

#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include <iostream>
#include <algorithm>

struct Bbox
{
  Vector3<float> pMin, pMax;

  //! Default constructor initializes to 'zero' cube
  Bbox(const Vector3<float> &pmin=Vector3<float>(0,0,0), const Vector3<float> & pmax=Vector3<float>(0,0,0))
    : pMin(pmin), pMax(pmax) { }

  Bbox(float pmin, float pmax)
    : pMin(Vector3<float>(pmin, pmin, pmin)), pMax(Vector3<float>(pmax, pmax, pmax)) { }

  friend std::ostream&operator << (std::ostream& os, const Bbox& b )
  {
    os << b.pMin << " -> " << b.pMax;
    return os;
  }

  //! Union of two Bbox :es
  friend Bbox BoxUnion(const Bbox& b1, const Bbox &b2)
  {
    Bbox b;
    b.pMin[0] = std::min(b1.pMin[0], b2.pMin[0]);
    b.pMin[1] = std::min(b1.pMin[1], b2.pMin[1]);
    b.pMin[2] = std::min(b1.pMin[2], b2.pMin[2]);

    b.pMax[0] = std::max(b1.pMax[0], b2.pMax[0]);
    b.pMax[1] = std::max(b1.pMax[1], b2.pMax[1]);
    b.pMax[2] = std::max(b1.pMax[2], b2.pMax[2]);
    return b;
  }

  friend Bbox PointUnion(const Bbox& b, const Vector4<float> & v)
  {
    Bbox b2;
    b2.pMin[0] = std::min(b.pMin[0], v[0]);
    b2.pMin[1] = std::min(b.pMin[1], v[1]);
    b2.pMin[2] = std::min(b.pMin[2], v[2]);

    b2.pMax[0] = std::max(b.pMax[0], v[0]);
    b2.pMax[1] = std::max(b.pMax[1], v[1]);
    b2.pMax[2] = std::max(b.pMax[2], v[2]);
    return b2;
  }

	//! Intersection of two Bbox :es
  friend Bbox BoxIntersection(const Bbox& b1, const Bbox &b2)
	{
		Bbox b;
		b.pMin[0] = std::max(b1.pMin[0], b2.pMin[0]);
		b.pMin[1] = std::max(b1.pMin[1], b2.pMin[1]);
		b.pMin[2] = std::max(b1.pMin[2], b2.pMin[2]);

		b.pMax[0] = std::min(b1.pMax[0], b2.pMax[0]);
		b.pMax[1] = std::min(b1.pMax[1], b2.pMax[1]);
		b.pMax[2] = std::min(b1.pMax[2], b2.pMax[2]);
		return b;
	}

  Bbox Transform(const Matrix4x4<float> & t) const
  {
    Bbox b;
    Vector4<float> v;

    v = t * Vector4<float>(pMin[0], pMin[1], pMin[2], 1.f);
    b.pMax = b.pMin = Vector3<float>(v[0], v[1], v[2]);

    v = t * Vector4<float>(pMax[0], pMin[1], pMin[2], 1.f);  b = PointUnion(b, v);
    v = t * Vector4<float>(pMax[0], pMax[1], pMin[2], 1.f);  b = PointUnion(b, v);
    v = t * Vector4<float>(pMin[0], pMax[1], pMin[2], 1.f);  b = PointUnion(b, v);

    v = t * Vector4<float>(pMin[0], pMin[1], pMax[2], 1.f);  b = PointUnion(b, v);
    v = t * Vector4<float>(pMax[0], pMin[1], pMax[2], 1.f);  b = PointUnion(b, v);
    v = t * Vector4<float>(pMax[0], pMax[1], pMax[2], 1.f);  b = PointUnion(b, v);
    v = t * Vector4<float>(pMin[0], pMax[1], pMax[2], 1.f);  b = PointUnion(b, v);

    return b;
  }
};

#endif
