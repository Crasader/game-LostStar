//
//  ShadowLayer.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-10-29.
//
//

#include "ShadowLayer.h"

ShadowLayer* ShadowLayer::layer;

ShadowLayer::ShadowLayer()
{
    resMgr = ResManager::getManager();
    layer = this;
}

ShadowLayer* ShadowLayer::sharedShadowLayer()
{
    return layer;
}

//注释这个函数内容，和startToUpdateShadows函数类容就可以屏蔽阴影
void ShadowLayer::createShadow(CustomeSprite *entity, const char *shadowFileName /* = NULL*/)
{
	//不需要阴影了，直接return
	return;
    CustomeSprite* shadow = new CustomeSprite();
    if (shadowFileName!=NULL)
    {
        shadow->initWithFile(resMgr->getSharedFilePath(shadowFileName).c_str());
    }else
    {
//        const char* file = resMgr->getShadowPath(entity->getFileName()).c_str();
//        shadow->initWithFile(file);
        string f = resMgr->getShadowPath(entity->getFileName());
        const char* file = f.c_str();
        shadow->initWithFile(file);
    }
    
    
    entity->setMyShadow(shadow);
    
    addChild(shadow);
    
    shadow->setOpacity(Shadow_Opacity);
    
    shadow->release();
}
//注释这个函数内容，和createShadow函数类容就可以屏蔽阴影
void ShadowLayer::startToUpdateShadows(b2World *theWorld)
{
	//不需要阴影了，直接return
	return;
    this->world = theWorld;
    schedule(schedule_selector(ShadowLayer::tick));
}

void ShadowLayer::tick(float time)
{
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			CustomeSprite* entity = (CustomeSprite*)b->GetUserData();
            if (entity->getMyShadow()!=NULL) {
                
                entity->getMyShadow()->setPosition( ccp(entity->getPositionX() + Shadow_X_Offset ,
                                                        entity->getPositionY() + Shadow_Y_Offset));
                
                entity->getMyShadow()->setRotation(entity->getRotation());
                
                entity->getMyShadow()->setScale(entity->getScale());
            }
        }
    }
}











