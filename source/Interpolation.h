//
//  LinearInterpolation.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/11/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FluidFire__LinearInterpolation__
#define __FluidFire__LinearInterpolation__

#include <iostream>
template<class T>
class LinearInterpolation {
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
    
    inline T operator()(double t,T f1,T f2)const {
        return f1+(f2-f1)*t;
    }
};

template<class T>
class CatmullRomInterpolation {
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
    
    inline T operator()(double t,T f1,T f2, T f3,T f4) const{
        T t2 = t*t; T t3 = t2*t;
        return f1*(-0.5*t+t2-0.5*t3)+f2*(1-5./2.*t2+3./2.*t3)+f3*(0.5*t+2*t2-3./2.*t3)+f4*(-0.5*t2+0.5*t3);
    }
};
#endif /* defined(__FluidFire__LinearInterpolation__) */
