#ifndef __LFBox2dHelper_H__
#define __LFBox2dHelper_H__
#include "LFUtile.h"
#include <list>

LF_SPACE_BEGIN

/**
 *	拾取点击的物体，扩展
 *	不需要扩展就 whichBodyContainThePoint(p,mWorld,NULL,ignorme);
 */
extern b2Body* whichBodyContainThePoint(b2Vec2 p,b2World* mWorld,float32 (*getExtensionValue)(b2Body* currentBody),bool (* ignoreMe)(b2Body* currentBody));

/**
 *	获取当前点击的物体部分,需要传入点击的位置p,当前世界mWorld,
 *	函数 getExtensionValue	用于返回当前需要扩展边界的值，都是box2d的单位，NULL则不扩展
 *	函数 ignoreMe			用于判断当前的body是否要忽略，是的话，就直接下一个，NULL 就每一个都需要计算
 */
extern b2Fixture* whichFixtureContainThePoint(b2Vec2 p,b2World* mWorld, float32 (*getExtensionValue)(b2Body* currentBody),bool (* ignoreMe)(b2Body* currentBody));

/**
 *	判断点是否在某个Body里面
 */
extern bool isPointInTheBody(b2Vec2 p,b2Body* body, float32 (*getExtensionValue)(b2Body* currentBody));
/**
 *	得到body的每一个Fixture的中心
 */
extern std::list<b2Vec2> getBodyFixtureCeneters(b2Body* body);


extern bool isAnyFixtureInLine(b2World* world,b2Vec2 startPoint, b2Vec2 endPoint,bool (* ignoreMe)(b2Body* currentBody),float testStep = 0.5);

//测试test中是否有fixture在线上
extern bool isAnyFixtureInLine(b2Vec2 startPoint,b2Vec2 endPoint,b2Vec2* point,std::vector<b2Fixture*> test, float testStep = 0.5);
//射线：angel=0，则是从左直接到右边,返回相交点
extern bool isAnyFixtureInLine_NearstPos(b2Vec2 startPoint,float angle,float32 length,b2Vec2* point,std::vector<b2Fixture*> test, float testStep = 0.5);

//
LF_SPACE_END

#endif

