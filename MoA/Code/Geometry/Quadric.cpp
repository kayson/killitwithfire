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
#include "Quadric.h"

Quadric::Quadric(const Matrix4x4<float> & q){
  this->mQuadric = q;
  transformQ = q;
}

Quadric::~Quadric(){}

/*
Vet inte riktigt hur tanken med att jag skall lösa denna med GetTransform. Så kör på setTransform istället
och skapat en ny Matris, som är Mi'*Q*M som skapats efter varje transform. 
*/
void Quadric::SetTransform(const Matrix4x4<float> & transform) {
  Implicit::SetTransform(transform);
  transformQ = (mWorld2Obj.Transpose()*mQuadric*mWorld2Obj);
}

/*!
 * Evaluation of world coordinates are done through either transformation
 * of the world-coordinates by mWorld2Obj, or transformation of the quadric
 * coefficient matrix by GetTransform() ONCE (see Section 2.2 in lab text).
 */
float Quadric::GetValue(float x, float y, float z) const
{
	Matrix4x4<float> Minv = GetTransform().Inverse();
	Vector4<float> pW(x, y, z, 1); 

	return pW*(transformQ*pW); 
}

/*!
 * Use the quadric matrix to evaluate the gradient.
 */
Vector3<float> Quadric::GetGradient(float x, float y, float z) const
{
	Matrix4x4<float> Q = mQuadric;
	Q(3,0) = 0; Q(3,1) = 0; Q(3,2) = 0; Q(3,3) = 0;
	
	Vector4<float> pW(x, y, z, 1);

	Vector4<float> a = 2.0*(Q*mWorld2Obj*pW);

	return Vector3<float>(a[0], a[1], a[2]);
}
/*
float Quadric::GetCurvature(float x, float y, float z) const
{
	Matrix4x4<float> Q = mQuadric;

	return (Q(0,0) + Q(1,1) + Q(2,2))/GetGradient(x,y,z).Length();
}*/
