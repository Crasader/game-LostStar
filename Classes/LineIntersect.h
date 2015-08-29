//
//  LineIntersect.h
//  HungryBear
//
//  Created by Bruce Yang on 12-3-12.
//  Copyright (c) 2012年 EricGameStudio. All rights reserved.
//

#ifndef LINEINTERSECT_H_
#define LINEINTERSECT_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;

#define intersect_zero(x) (((x)>0?(x):-(x))<b2_epsilon)

/*
#pragma mark-
#pragma mark 适用于 b2Vec2 的版本~


// 判两线段相交,包括端点和部分重合
bool intersect_in(b2Vec2 u1, b2Vec2 u2, b2Vec2 v1,  b2Vec2 v2);


// 计算两线段交点,请判线段是否相交(同时还是要判断是否平行!)
b2Vec2 intersection(b2Vec2 u1, b2Vec2 u2, b2Vec2 v1, b2Vec2 v2);
*/
//#pragma mark-
//#pragma mark 适用于 CGPoint 的版本~


bool intersect_in2(CCPoint u1, CCPoint u2, CCPoint v1, CCPoint v2);


// 计算两线段交点,请判线段是否相交(同时还是要判断是否平行!)
CCPoint intersection2(CCPoint u1, CCPoint u2, CCPoint v1, CCPoint v2);
/*
#pragma mark-
#pragma mark 验证上述几个方法的移植是否存在什么问题~

void validateAlgorithm();
*/
#endif