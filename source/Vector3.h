#ifndef VECTOR3_H
#define VECTOR3_H
#include <iostream>
class Vector3
{
public:
	// Constructors
	Vector3();
	Vector3(double xf, double yf, double zf);
    ~Vector3();
	// Functions
	void mult(double f);
    void substract(Vector3 *v);
    void rotate(Vector3 *axisRotate, double *angle ,double a, double b, double c);
    
    void normalize();
    double dot(Vector3 &vTemp);
	double norm();
	double angle(Vector3 *vTemp);
    double cosA(Vector3 *vTemp);

    void translate(Vector3 *v);
    void description();
    
    Vector3 cross(Vector3 *vTemp);
    Vector3 from(Vector3 *v);
    Vector3 add(Vector3 *v);
    Vector3 projectionOnVector(Vector3 v);
    //Overloaded operators
    void operator+= (const Vector3 &v);
    void operator-= (const Vector3 &v);
    void operator*= (const Vector3 &v);
    Vector3 operator* (const Vector3 &v) const;
    Vector3 operator+ (const Vector3 &v) const;
    Vector3 operator- (const Vector3 &v) const;
	
	void operator*= (const double f);
	Vector3 operator* (const double f) const;
	Vector3 operator/ (const double f) const;
    Vector3 operator+ (const double f) const;
    Vector3 operator- (const double f) const;
	
	friend Vector3 operator+ (const double f, const Vector3 &v);
	friend Vector3 operator- (const double f, const Vector3 &v);
	friend Vector3 operator* (const double f, const Vector3 &v);
	
	friend std::ostream& operator<<(std::ostream& out, Vector3& v )
	{
		out << v.x << " " << v.y << " " << v.z << std::endl;
		return out;
	}


public:
	// Variables
	double x,y,z;
};
#endif