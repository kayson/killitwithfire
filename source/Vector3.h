#ifndef VECTOR3_H
#define VECTOR3_H
#include <iostream>




class Vector3{
public:
	// Constructors
	Vector3();
	Vector3(double xf, double yf, double zf);
    Vector3(double v);
    ~Vector3();
	// Functions
	void mult(double f);
    void substract(Vector3 *v);
    void rotate(Vector3 *axisRotate, double *angle ,double a, double b, double c);
    
    void normalize();
    double dot(const Vector3 &vTemp);
	static double dot(const Vector3 &v1, const Vector3 &v2);

	double norm() const;
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
	bool operator==(const Vector3 &v) const;
	
	void operator*= (const double f);
	Vector3 operator* (const double f) const;
	Vector3 operator/ (const double f) const;
    Vector3 operator+ (const double f) const;
    Vector3 operator- (const double f) const;
	
	friend Vector3 operator+ (const double f, const Vector3 &v);
	friend Vector3 operator- (const double f, const Vector3 &v);
	friend Vector3 operator* (const double f, const Vector3 &v);

	Vector3 operator- () const;
	bool operator> (const Vector3 &v) const;
	
	friend std::ostream& operator<<(std::ostream& out, Vector3& v )
	{
		out << v.x << " " << v.y << " " << v.z << std::endl;
		return out;
	}

	static double planeLineIntersection(const Vector3 &point, const Vector3 &normal, const Vector3 &lineDirection, const Vector3 &linePos);
	static bool pointInRectangle(const Vector3 &point, const Vector3 &r1, const Vector3 &r2, const Vector3 &r3);

	static bool rayBoxIntersection(const Vector3 &minbox, const Vector3 &maxbox, const Vector3 &lineOrigin, const Vector3 &lineDirection, double *tmin, double *tmax);

public:
	// Variables
	double x,y,z;
};
#endif