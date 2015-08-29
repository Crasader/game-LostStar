//
//  ShadowLayer.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-10-29.
//
//

#ifndef __RedStarFall__ShadowLayer__
#define __RedStarFall__ShadowLayer__

#include <iostream>
#include "cocos2d.h"
#include <string>
#include "ResManager.h"
#include "CustomeSprite.h"

using namespace cocos2d;

#define Shadow_X_Offset -7 //影子的x方向上的偏移
#define Shadow_Y_Offset -7 //影子的y方向上的偏移
#define Shadow_Opacity  255 //不透明度

//阴影图层
class ShadowLayer : public cocos2d::CCLayer
{
private:
    static ShadowLayer* layer;
    ResManager* resMgr;
    b2World* world;
    
    void tick(float time);
public:
    ShadowLayer();
    
    static ShadowLayer* sharedShadowLayer();
    
    void startToUpdateShadows(b2World* theWorld);
    
    //shadowFileName为空就使用默认的，在原基础上加一个_shadow后缀
    void createShadow(CustomeSprite* entity,const char* shadowFileName = NULL);
};

#endif /* defined(__RedStarFall__ShadowLayer__) */
