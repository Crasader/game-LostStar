//
//  VStick.m
//
//  Created by patrick on 14/10/2010.
//

#include "VStick.h"
#include "cocos2d.h"


void VStick::initWith(VPoint* argA, VPoint* argB) {
    pointA = argA;
    pointB = argB;
    hypotenuse = ccpDistance(ccp(pointA->x,pointA->y),ccp(pointB->x,pointB->y));
}

void VStick::contract() {
	float dx = pointB->x - pointA->x;
	float dy = pointB->y - pointA->y;
	float h = ccpDistance(ccp(pointA->x,pointA->y),ccp(pointB->x,pointB->y));
	float diff = hypotenuse - h;
	float offx = (diff * dx / h) * 0.7;
	float offy = (diff * dy / h) * 0.7;
	pointA->x-=offx;
	pointA->y-=offy;
	pointB->x+=offx;
	pointB->y+=offy;
}

VPoint* VStick::getPointA() {
	return pointA;
}
VPoint* VStick::getPointB() {
	return pointB;
}

