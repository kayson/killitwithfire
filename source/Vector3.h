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
    void operator*= (const double &v);
    Vector3 operator* (const double &v);
    Vector3 operator* (const Vector3 &v);
    Vector3 operator+ (const Vector3 &v);
    Vector3 operator- (const Vector3 &v);

public:
	// Variables
	double x,y,z;
};
#endif