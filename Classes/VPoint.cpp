//
//  VPoint.m
//
//  Created by patrick on 14/10/2010.
//

#include "VPoint.h"
#include "cocos2d.h"

void VPoint::setPos(float argX, float argY){
	oldx = argX;
	oldy = argY;

    x = oldx;
    y = oldy;
}

void VPoint::update() {
	float tempx = x;
	float tempy = y;
	x += x - oldx;
	y += y - oldy;
	oldx = tempx;
	oldy = tempy;
}

void VPoint::applyGravity(float dt) {
	y -= 1.0f*dt; //gravity magic number
}



