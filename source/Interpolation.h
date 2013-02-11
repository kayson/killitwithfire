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
    inline T operator()(T t,T x1,T x2)const {
        return x1+(x2-x1)*t;
    }
};

template<class T>
class CatmullRomInterpolation {
private:

public:
    //Sid 37 bridson
    inline T operator()(T t1,T x1,T x2, T x3,T x4) const{
        T t2 = t1*t1; T t3 = t2*t1;
        return x1*(-0.5*t1+t2-0.5*t3)+x2*(1-5./2.*t2+3./2.*t3)+x3*(0.5*t1+2*t2-3./2.*t3)+x4*(-0.5*t2+0.5*t3);
    }
};
#endif /* defined(__FluidFire__LinearInterpolation__) */
