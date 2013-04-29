#include "Vector3.h"
#include <math.h>

// Constructors
Vector3::Vector3()
{
    x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(double xf, double yf, double zf){
	x = xf;
	y = yf;
	z = zf;
}

Vector3::Vector3(double v){
    x = v;
	y = v;
	z = v;
}
Vector3::~Vector3(){
    
}

// Dot product between two vectors, returns float
double Vector3::dot(const Vector3 &vTemp)
{
	double dotProduct = x*vTemp.x + y*vTemp.y + z*vTemp.z;
	return dotProduct;
}

double Vector3::dot(const Vector3 &v1, const Vector3 &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

// Multiplikation by f to all elements, void
void Vector3::mult(double f)
{
	x *= f;
	y *= f;
	z *= f;
}

void Vector3::substract(Vector3 *v){
    
    x -= v->x;
    y -= v->y;
    z -= v->z;
    
}

Vector3 Vector3::from(Vector3 *v){
    double dx = x-v->x;
    double dy = y-v->y;
    double dz = z-v->z;
    Vector3 vec = Vector3(dx,dy,dz);

    return vec;
}

void Vector3::translate(Vector3 *v){
    x += v->x;
    y += v->y;
    z += v->z;
}

Vector3 Vector3::add(Vector3 *v){
    double dx = x+v->x;
    double dy = y+v->y;
    double dz = z+v->z;
    Vector3 vec = Vector3(dx,dy,dz);
    
    return vec;
}


// Norm (length) of the vector, returns float
double Vector3::norm() const
{
	double norm = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	return norm;
}

// Normalize the vector
void Vector3::normalize()
{
    double size = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    double invSize = 1/size;
    x *= invSize;
    y *= invSize;
    z *= invSize;
}

// stolen from : http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/ 
//Rotate around vector axisRotate 
void Vector3::rotate(Vector3 *axisRotate, double *angle, double a, double b, double c){
   
    //En smula snabbare
    if ((a == 0) && (b == 0) && (c == 0)) {
        double u,v,w;
        u = axisRotate->x;
        v = axisRotate->y;
        w = axisRotate->z;
        
        // Set some intermediate values.
        double cosT = cos(*angle);
        double oneMinusCosT = 1 - cosT;
        double sinT = sin(*angle);
        
        // Use the formula in the paper.
        x = ((-u*(-u*x-v*y-w*z))*oneMinusCosT+x*cosT+(-w*y+v*z)*sinT);
        
        y = (- v*(- u*x - v*y - w*z)) * oneMinusCosT
             + y*cosT
             + (w*x - u*z)*sinT;
        
        z = ((- w*(- u*x - v*y - w*z)) * oneMinusCosT
             + z*cosT
             + (- v*x + u*y)*sinT);
    }else{
    
        double u,v,w;
        u = axisRotate->x;
        v = axisRotate->y;
        w = axisRotate->z;
    
        // Set some intermediate values.
        double u2 = u*u;
        double v2 = v*v;
        double w2 = w*w;
        double cosT = cos(*angle);
        double oneMinusCosT = 1 - cosT;
        double sinT = sin(*angle);
    
    
        // Use the formula in the paper.
        x = ((a*(v2 + w2)-u*(b*v+c*w-u*x-v*y-w*z))*oneMinusCosT+x*cosT+(-c*v+b*w-w*y+v*z)*sinT);
    
        y = ((b*(u2 + w2) - v*(a*u + c*w - u*x - v*y - w*z)) * oneMinusCosT
         + y*cosT
         + (c*u - a*w + w*x - u*z)*sinT);
    
        z = ((c*(u2 + v2) - w*(a*u + b*v - u*x - v*y - w*z)) * oneMinusCosT
         + z*cosT
         + (-b*u + a*v - v*x + u*y)*sinT);
    }
}


// Smallest angle between two vectors, returns float
double Vector3::angle(Vector3 *vTemp)
{
	if(*this == *vTemp)
		return 0;
	else if(*vTemp == Vector3(0.,0.,0))
		return 0;
	else {
		double angle = acos(dot(*vTemp)/(norm()*vTemp->norm()));
		return angle;
	}
}

// Smallest angle between two vectors, returns float
double Vector3::cosA(Vector3 *vTemp)
{
	double cosA = dot(*vTemp)/(norm()*vTemp->norm());
    
	return cosA;
}



// Cross product, returns a Vector3
Vector3 Vector3::cross(Vector3 *vTemp)
{
	double xTemp = y*vTemp->z - z*vTemp->y;
	double yTemp = -(x*vTemp->z - z*vTemp->x);
	double zTemp = x*vTemp->y - y*vTemp->x;

	return Vector3(xTemp, yTemp, zTemp);
}

//Projection http://en.wikipedia.org/wiki/Vector_projection
Vector3 Vector3::projectionOnVector(Vector3 v)
{
    double lenght = norm()*cosA(&v);
    
    v.normalize();
    
    v.x *= lenght;
    v.y *= lenght;
    v.z *= lenght;

    
	return v;
}



//Overloaded operators :)
void Vector3::operator+= (const Vector3 &v)
{
    x +=v.x;
    y +=v.y;
    z +=v.z;
}

void Vector3::operator-= (const Vector3 &v){
    x -=v.x;
    y -=v.y;
    z -=v.z;
}

void Vector3::operator*= (const Vector3 &v){
    
    x *=v.x;
    y *=v.y;
    z *=v.z;
}

Vector3 Vector3::operator+ (const Vector3 &v) const {
    
    return Vector3(x+v.x,y+v.y,z+v.z);
}

Vector3 Vector3::operator- (const Vector3 &v) const {
    
    return Vector3(x-v.x,y-v.y,z-v.z);
}

Vector3 Vector3::operator* (const Vector3 &v) const {
    
    return Vector3(x*v.x,y*v.y,z*v.z);
}

bool Vector3::operator==(const Vector3 &v) const {
	return (x == v.x) && (y == v.y) && (z == v.z);
}

void Vector3::description(){
    
    std::cout << "(" << x << " " << y << " " << z << ")";
    
}

void Vector3::operator*= (const double f){
    
    x *= f;
    y *= f;
    z *= f;
}

Vector3 Vector3::operator+ (const double f) const
{
	return Vector3(x+f,y+f,z+f);
}

Vector3 Vector3::operator- (const double f) const
{
	return Vector3(x-f,y-f,z-f);
}

Vector3 Vector3::operator* (const double f) const
{
	return Vector3(x*f,y*f,z*f);
}

Vector3 Vector3::operator/ (const double f) const {
    if(f == 0)
		throw;
    return Vector3(x/f,y/f,z/f);
}

Vector3 operator+ (const double f, const Vector3 &v)
{
	return v+f;
}

Vector3 operator- (const double f, const Vector3 &v)
{
	return v-f;
}

Vector3 operator* (const double f, const Vector3 &v)
{
	return v;
}

Vector3 Vector3::operator- () const
{
	return Vector3(-x, -y, -z);
}

bool Vector3::operator> (const Vector3 &v) const
{
	return norm() > v.norm();
}