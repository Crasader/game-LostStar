//
//  VPoint.h
//
//  Created by patrick on 14/10/2010.
//

#ifndef VPOINT_H_
#define VPOINT_H_

class VPoint
{
public:
    float x;
    float y;
	float oldx;
    float oldy;

    
    
    
    void setPos(float argX, float argY);
    void update();
    void applyGravity(float dt);
};

#endif