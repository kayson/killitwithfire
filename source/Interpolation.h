//
//  LinearInterpolation.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/11/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FluidFire__LinearInterpolation__
#define __FluidFire__LinearInterpolation__

#include "GridField.h"
template<class T> class LinearInterpolation;
template<class T> class CatmullRomInterpolation;
template<class T>
class Interpolation {
public:
    virtual ~Interpolation(){};
    virtual Interpolation* clone() const = 0;
    virtual T interpolate(const GridField<T> &g,int i,int j,int k,double x,double y,double z)const = 0;
    static Interpolation* defaultInterpolation(){ return new LinearInterpolation<T>(); };
};

template<class T>
class LinearInterpolation : public Interpolation<T>{
public:
    /*
     Input:
     t = ett värde mellan 0 och 1
     f(0) = f1;
     f(1) = f2;
     
     Exempel:
     t = 1.3, =  f(0) = 1, f(1) = 4
     => f(t) = f(1.3) =
     */
    virtual Interpolation<T>* clone() const{ return new LinearInterpolation<T>();}
    
    virtual ~LinearInterpolation(){}
    
    T operator()(double t,T f1,T f2)const {
        return (T)(f1+(f2-f1)*t);
    }
        
    virtual T interpolate(const GridField<T> &g,int i,int j,int k,double x,double y,double z)const {
        
        T t1 = (*this)(x,g.valueAtIndex(i,j,k),g.valueAtIndex(i+1,j,k));
        T t2 = (*this)(x,g.valueAtIndex(i,j+1,k),g.valueAtIndex(i+1,j+1,k));
        T t3 = (*this)(x,g.valueAtIndex(i,j,k+1),g.valueAtIndex(i+1,j,k+1));
        T t4 = (*this)(x,g.valueAtIndex(i,j+1,k+1),g.valueAtIndex(i+1,j+1,k+1));
        
        T t5 = (*this)(y,t1,t2);
        T t6 = (*this)(y,t3,t4);
        
        T t7 = (*this)(z,t5,t6);
        return t7;
    }
};

template<class T>
class CatmullRomInterpolation : public Interpolation<T> {
private:

public:
    //Sid 37 bridson
    
    /*
     Input: n
     t ett värde mellan 0 och 1
     f(-1) = f1;
     f(0) = f2;
     f(1) = f3;
     f(2) = f4;
     
     Exempel:
     t = 1.3, =  f(-1) = 1, f(0) = 2, f(1) = 6, f(2) = -5;
     => f(t) = f(1.3) = 3.222
    */
    
    virtual Interpolation<T>* clone() const{ return new LinearInterpolation<T>();}
    
    virtual ~CatmullRomInterpolation(){}

    inline T operator()(double t,T f1,T f2, T f3,T f4) const{
        T t2 = t*t; T t3 = t2*t;
        return f1*(-0.5*t+t2-0.5*t3)+f2*(1.0-5.0/2.0*t2+3.0/2.0*t3)+f3*(0.5*t+2.0*t2-3.0/2.0*t3)+f4*(-0.5*t2+0.5*t3);
    }
    
    virtual inline T interpolate(const GridField<T> &g,int i,int j,int k,double x,double y,double z)const {
    
        T t[4];
        for (int index = 0; index < 4; index++) {
            T t1 = (*this)(x,g.valueAtIndex(i-1,j-1,k+index-1),g.valueAtIndex(i,j-1,k+index-1),g.valueAtIndex(i+1,j-1,k+index-1),g.valueAtIndex(i+2,j-1,k+index-1));
            T t2 = (*this)(x,g.valueAtIndex(i-1,j,k+index-1),g.valueAtIndex(i,j,k+index-1),g.valueAtIndex(i+1,j,k+index-1),g.valueAtIndex(i+2,j,k+index-1));
            T t3 = (*this)(x,g.valueAtIndex(i-1,j+1,k+index-1),g.valueAtIndex(i,j+1,k+index-1),g.valueAtIndex(i+1,j+1,k+index-1),g.valueAtIndex(i+2,j+1,k+index-1));
            T t4 = (*this)(x,g.valueAtIndex(i-1,j+2,k+index-1),g.valueAtIndex(i,j+2,k+index-1),g.valueAtIndex(i+1,j+2,k+index-1),g.valueAtIndex(i+2,j+2,k+index-1));
            
            t[index] = (*this)(y, t1, t2, t3, t4);
        }
   
        return (*this)(z,t[0],t[1],t[2],t[3]);
    }
};
#endif /* defined(__FluidFire__LinearInterpolation__) */
