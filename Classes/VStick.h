//
//  VStick.h
//
//  Created by patrick on 14/10/2010.
//

#ifndef VSTICK_H_
#define VSTICK_H_

#include "VPoint.h"

class VStick {
	VPoint* pointA;
	VPoint* pointB;
	float hypotenuse;
public:
    void initWith(VPoint* argA, VPoint* argB);
    void contract();
    VPoint* getPointA();
    VPoint* getPointB();
};

#endif
