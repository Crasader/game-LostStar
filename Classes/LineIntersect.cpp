//
//  LineIntersect.mm
//  HungryBear
//
//  Created by Bruce Yang on 12-3-12.
//  Copyright (c) 2012年 EricGameStudio. All rights reserved.
//

#include "LineIntersect.h"


/*
// 计算交叉乘积 (P1-P0)x(P2-P0)
double xmult(b2Vec2 p1, b2Vec2 p2, b2Vec2 p0) {
    return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
}


// 判点是否在线段上,包括端点
bool dot_online_in(b2Vec2 p, b2Vec2 l1, b2Vec2 l2) {
    return intersect_zero(xmult(p, l1, l2) && (l1.x-p.x)*(l2.x-p.x) < b2_epsilon && (l1.y-p.y)*(l2.y-p.y) < b2_epsilon;
}

// 判两点在线段同侧,点在线段上返回0
+(int) same_side:(b2Vec2)p1 p2:(b2Vec2)p2 l1:(b2Vec2)l1 l2:(b2Vec2)l2 {
    return [self xmult:l1 p2:p1 p3:l2] * [self xmult:l1 p2:p2 p3:l2] > b2_epsilon;
}

// 判两直线平行
+(int) parallel:(b2Vec2)u1 u2:(b2Vec2)u2 v1:(b2Vec2)v1 v2:(b2Vec2)v2 {
    return zero((u1.x-u2.x)*(v1.y-v2.y)-(v1.x-v2.x)*(u1.y-u2.y));
}

// 判三点共线
+(int) dots_inline:(b2Vec2)p1 p2:(b2Vec2)p2 p3:(b2Vec2)p3 {
    return zero([self xmult:p1 p2:p2 p3:p3]);
}

// 判两线段相交,包括端点和部分重合
+(int) intersect_in:(b2Vec2)u1 u2:(b2Vec2)u2 v1:(b2Vec2)v1 v2:(b2Vec2)v2 {
    if (![self dots_inline:u1 p2:u2 p3:v1] || ![self dots_inline:u1 p2:u2 p3:v2]) {
        return ![self same_side:u1 p2:u2 l1:v1 l2:v2] && ![self same_side:v1 p2:v2 l1:u1 l2:u2];
    } else {
        return [self dot_online_in:u1 l1:v1 l2:v2] || 
                [self dot_online_in:u2 l1:v1 l2:v2] || 
                [self dot_online_in:v1 l1:u1 l2:u2] || 
                [self dot_online_in:v2 l1:u1 l2:u2];
    } 
}


// 计算两线段交点,请判线段是否相交(同时还是要判断是否平行!)
+(b2Vec2) intersection:(b2Vec2)u1 u2:(b2Vec2)u2 v1:(b2Vec2)v1 v2:(b2Vec2)v2 {
    b2Vec2 ret=u1;
    double t=((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
    /((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
    ret.x+=(u2.x-u1.x)*t;
    ret.y+=(u2.y-u1.y)*t;
    return ret;
}
*/

//#pragma mark-
//#pragma mark 适用于 CGPoint 的版本~

// 计算交叉乘积 (P1-P0)x(P2-P0)
double intersect_xmult2(CCPoint p1, CCPoint p2, CCPoint p0) {
    return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
}

// 判点是否在线段上,包括端点
bool dot_online_in2(CCPoint p, CCPoint l1, CCPoint l2) {
    return intersect_zero(intersect_xmult2(p, l1, l2)) && (l1.x-p.x)*(l2.x-p.x) < b2_epsilon && (l1.y-p.y)*(l2.y-p.y) < b2_epsilon;
}

// 判两点在线段同侧,点在线段上返回0
bool same_side2(CCPoint p1, CCPoint p2, CCPoint l1, CCPoint l2) {
    return intersect_xmult2(l1, p1, l2) * intersect_xmult2(l1, p2, l2) > b2_epsilon;
}

// 判两直线平行
bool parallel2(CCPoint u1, CCPoint u2, CCPoint v1, CCPoint v2) {
    return intersect_zero((u1.x-u2.x)*(v1.y-v2.y)-(v1.x-v2.x)*(u1.y-u2.y));
}

// 判三点共线
bool dots_inline2(CCPoint p1, CCPoint p2, CCPoint p3) {
    return intersect_zero(intersect_xmult2(p1, p2, p3));
}

bool intersect_in2(CCPoint u1, CCPoint u2, CCPoint v1, CCPoint v2) {
    if (!dots_inline2(u1,u2,v1) || !dots_inline2(u1,u2,v2)) {
        return !same_side2(u1,u2,v1,v2) && !same_side2(v1,v2,u1,u2);
    } else {
        return dot_online_in2(u1,v1,v2) ||
        dot_online_in2(u2,v1,v2) ||
        dot_online_in2(v1,u1,u2) ||
        dot_online_in2(v2,u1,u2);
    } 
}


// 计算两线段交点,请判线段是否相交(同时还是要判断是否平行!)
CCPoint intersection2(CCPoint u1, CCPoint u2, CCPoint v1, CCPoint v2) {
    CCPoint ret=u1;
    double t=((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
    /((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
    ret.x+=(u2.x-u1.x)*t;
    ret.y+=(u2.y-u1.y)*t;
    return ret;
}

/*
#pragma mark-
#pragma mark 验证上述几个方法的移植是否存在什么问题~

+(void) validateIntersect:(b2Vec2)u1 u2:(b2Vec2)u2 v1:(b2Vec2)v1 v2:(b2Vec2)v2 {
    b2Vec2 answer;
    if ([self parallel:u1 u2:u2 v1:v1 v2:v2] || ![self intersect_in:u1 u2:u2 v1:v1 v2:v2]){
        printf("无交点!\n");
    } else {
        answer = [self intersection:u1 u2:u2 v1:v1 v2:v2];
        printf("交点为:(%lf,%lf)\n", answer.x, answer.y);
    }
}

+(void) validateAlgorithm {
    [LineIntersect validateIntersect:b2Vec2(0, 1) u2:b2Vec2(1, 0) v1:b2Vec2(0, 0) v2:b2Vec2(1, 1)];
    [LineIntersect validateIntersect:b2Vec2(0, 10) u2:b2Vec2(10, 0) v1:b2Vec2(0, 0) v2:b2Vec2(10, 10)];
    [LineIntersect validateIntersect:b2Vec2(-2, 0) u2:b2Vec2(2, 0) v1:b2Vec2(-1, 3) v2:b2Vec2(-1, -1)];
    [LineIntersect validateIntersect:b2Vec2(-2, 0) u2:b2Vec2(2, 0) v1:b2Vec2(-1, 3) v2:b2Vec2(1, -2)];
}
*/

