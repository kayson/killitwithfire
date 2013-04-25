//
//  JumpCondition.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 4/18/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__JumpCondition__
#define __FuidFire__JumpCondition__

#include <iostream>
#include "LevelSet.h"
#include "Vector3.h"
#include "firePresets.h"

class JumpCondition {
public:
     /*Finns endast för att thisDomain == otherDomain INTE fungera!*/
    static bool isSame(CellType thisDomain,CellType otherDomain){
        return ((thisDomain == FUEL && otherDomain == FUEL)
                || (thisDomain == BURNT && otherDomain == BURNT) );
    }

    
    static double mult(CellType thisDomain,CellType otherDomain){
        
        if (isSame(thisDomain, otherDomain)) {
            return 0.0;
        }else if (thisDomain == FUEL && otherDomain == BURNT){
            return 1.0;
        }else if (thisDomain == BURNT && otherDomain == FUEL){
            return -1.0;
        }else{
            assert(false);
            
        }
    }
    
    static double DVn(const LevelSet *l, VelocityDirection direction,
                      double x,double y,double z,
                      CellType thisDomain,CellType otherDomain){
        
        
        Vector3 normal = l->getNormal(x, y, z);
        normal *= -1;
        double norm = normal.norm();
        assert(norm < 1.01);
        double val;
        if (direction == UDIR) {
            val =  normal.x;
        }else if (direction == VDIR){
            val =  normal.y;
        }else if (direction == WDIR){
            val =  normal.z;
        }else{
            assert(false);
        }
        
        double mult = JumpCondition::mult(thisDomain, otherDomain);
        
        return val*mult*(FirePresets::rhof/FirePresets::rhob-1.0)*FirePresets::S;
    }
};
#endif /* defined(__FuidFire__JumpCondition__) */
