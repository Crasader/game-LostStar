//
//  BaseBackgroundLayer.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#include "BaseBackgroundLayer.h"
#include "GameLayerFactory.h"
#include "SpriteHelper.h"
#include "LevelManager.h"
#include "font.h"
#include "LFStrings.h"

BaseBackgroundLayer::BaseBackgroundLayer(int seasonId, int seactionId)
{
    init();
    baseContentLayer = NULL;
    table = NULL;
    setTouchEnabled(true);
    
    dynamicObjList.clear();
    
    mScreenSize = CCDirector::sharedDirector()->getWinSize();
    
    {
		string backBgPic = SpriteHelper::splicingNum_String("background_s%d.png",seasonId);
        mBg = CCSprite::create(backBgPic.c_str());
		mBg->setAnchorPoint(ccp(0.5f, 0.0f));
        mBg->setPosition(CCPointMake(mScreenSize.width/2, 0));
        addChild(mBg, 0);
    }
    timeGapForDynamicAct = TIME_DYNAMIC_ACTION;
    
    //开始计时播放动画
    schedule(schedule_selector(BaseBackgroundLayer::startDynamicObjAction),timeGapForDynamicAct);
}

BaseBackgroundLayer::~BaseBackgroundLayer()
{
    unscheduleAllSelectors();
}

void BaseBackgroundLayer::startDynamicObjAction(float time)
{
    if (baseContentLayer->getLayerFreezy()) {
        return;
    }
    int size = dynamictTriggerVec.size();
    if (size <= 0) {
        return;
    }
    int index = ((float)size) * CCRANDOM_0_1();

    dynamictTriggerVec[index]->startAction();

    //我们同一时间只播放一个动画，所以就取消任务
    unschedule(schedule_selector(BaseBackgroundLayer::startDynamicObjAction));
}

void BaseBackgroundLayer::setTimeGapForDynamicAct(float newTime)
{
    timeGapForDynamicAct = newTime;
}

void BaseBackgroundLayer::addDynamicObj(BaseDynamicObj *obj)
{
    dynamicObjList.insert(dynamicObjList.end(), obj);
    obj->setBaseBackgroundLayer(this);
}
BaseDynamicObj* BaseBackgroundLayer::addTriggerDynamicObj(BaseDynamicObj *obj)
{
    dynamictTriggerVec.insert(dynamictTriggerVec.end(), obj);
    return obj;
}
void BaseBackgroundLayer::removeTriggerDynamicObj(BaseDynamicObj *obj)
{
    vector<BaseDynamicObj*>::iterator it;
    for (it = dynamictTriggerVec.begin(); it!=dynamictTriggerVec.end(); it++) {
        if ((*it) == obj) {
            dynamictTriggerVec.erase(it);
            break;
        }
    }
}
BaseDynamicObj* BaseBackgroundLayer::easyAddObjToLayer(BaseDynamicObj *obj, cocos2d::CCPoint pos,bool release /* = true*/)
{
    addDynamicObj(obj);
    addChild(obj);
    obj->setPosition(pos);
    
    if (release) {
        obj->release();
    }
    
    return obj;
}

void BaseBackgroundLayer::removeDynamicObj(BaseDynamicObj *obj)
{
    list<BaseDynamicObj*>::iterator it;
    for (it = dynamicObjList.begin(); it!=dynamicObjList.end();) {
        
        if ((*it) == obj ) {
           it = dynamicObjList.erase(it);
        }else{
            it++;
        }
    }
}

void BaseBackgroundLayer::setBaseContentLayer(BaseContentLayer *baseContentLayer)
{
    this->baseContentLayer = baseContentLayer;
}

CCNode* BaseBackgroundLayer::getRandomTable(bool randomAgain)
{
    if (baseContentLayer == NULL) {
        return NULL;
    }
    
    b2World* world = baseContentLayer->getWorld();
    //第一次获得桌面
    if (table == NULL || randomAgain) {
        list<CCNode*> tables = getTableList(world);
        int index = tables.size()*CCRANDOM_0_1();
        
        table = getNode(tables, index);    
    }
        
    return table;

}

CCNode* BaseBackgroundLayer::getNode(list<cocos2d::CCNode *> list, int index)
{
    std::list<CCNode*>::iterator it;
    CCNode* rt = NULL;
    if (index <0 || index >= (int)list.size()) {
        CCLog("getNode--->error!---->index [= %d] outoff size [0,%d]",index,list.size());
        
        return NULL;
    }
    
    int i=0;
    for (it=list.begin(); it!=list.end(); ++it) {
        if (i == index) {
            rt = (CCNode*)(*it);
            
            return rt;
        }
        i++;
    }
    
    return NULL;
}

list<CCNode*> BaseBackgroundLayer::getTableList(b2World *world)
{
    list<CCNode*> tables;
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        CCNode* node = (CCNode*)b->GetUserData() ;
        if (node!= NULL &&
            (node->getTag() == Tag_Box_Table) )
//            (node->getTag() == Tag_Box_Table || node->getTag() == Tag_Box_Table_Ineffective) ) 
        {
           
            tables.insert(tables.end(),node);
            
        }
    }
    return tables;
}

void BaseBackgroundLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    //重新开始计时播放动画
    unschedule(schedule_selector(BaseBackgroundLayer::startDynamicObjAction));
    schedule(schedule_selector(BaseBackgroundLayer::startDynamicObjAction),timeGapForDynamicAct);
}

void BaseBackgroundLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}
void BaseBackgroundLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCTouch* touch = (CCTouch*)(*(touches->begin()));
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    list<BaseDynamicObj*>::iterator it;
    for (it=dynamicObjList.begin(); it!=dynamicObjList.end(); ++it) {
        BaseDynamicObj* obj = (*it);
        if (obj == NULL) {
            continue;
        }
        obj->backgroudClicked(location);
    }
}
void BaseBackgroundLayer::bombed(int currentBombPower, int maxPower)
{//这里什么都不处理
    
}

CCNode* BaseBackgroundLayer::getRedStar()
{
    return baseContentLayer->getCurrentRedStar();
}



//==============================
//  动态物体
//==============================
BaseDynamicObj::BaseDynamicObj()
{
    //一定要init
    init();
    isAction = false;
    resMgr = ResManager::getManager();
    baseBgLayer = NULL;
    releasePool.clear();
}
BaseDynamicObj::~BaseDynamicObj()
{
    unscheduleAllSelectors();

    
    list<CCAction*>::iterator it;
    for (it=releasePool.begin(); it!=releasePool.end(); ++it) {
        CC_SAFE_RELEASE((*it));
    }
    
    releasePool.clear();
}
void BaseDynamicObj::backgroudClicked(cocos2d::CCPoint glPoint)
{
    
}

bool BaseDynamicObj::startAction()
{
    return false;
}
void BaseDynamicObj::setBaseBackgroundLayer(BaseBackgroundLayer *layer)
{
    this->baseBgLayer = layer;
}

void BaseDynamicObj::removeFromLayerDynamicObjList()
{
    if (baseBgLayer != NULL) {
        baseBgLayer->removeDynamicObj(this);
        baseBgLayer->removeTriggerDynamicObj(this);
    }
}
void BaseDynamicObj::removeFromTriggerDynamicVec()
{
    if (baseBgLayer != NULL) {
        baseBgLayer->removeTriggerDynamicObj(this);
    }
}

list<CCRect> BaseDynamicObj::getBounds()
{
    std::list<CCRect> rects;
    CCRect r = boundingBox();
    rects.insert(rects.end(),r);
    return rects;
}

bool BaseDynamicObj::pointInMe(cocos2d::CCPoint glPoint)
{
    return SpriteHelper::pointInBorder(this, glPoint);
}


CCFiniteTimeAction* BaseDynamicObj::easyCreateAction_Retain(const char *format, int startNum, int totalNum, float delay,bool repeat /* = false*/)
{
    CCFiniteTimeAction* action = easyCreateAction(format, startNum, totalNum, delay,repeat);
    
    action->retain();
    releasePool.insert(releasePool.end(), action);
    
    return action;
}

CCFiniteTimeAction* BaseDynamicObj::easyCreateAction_Retain(const char *format, int startNum, int totalNum, float delay, cocos2d::CCObject *target, SEL_CallFuncO callBack)
{
    CCFiniteTimeAction* action = easyCreateAction(format, startNum, totalNum, delay, target, callBack);
    
    action->retain();
    releasePool.insert(releasePool.end(), action);
    
    return action;
}

CCFiniteTimeAction* BaseDynamicObj::easyCreateAction(const char * format,int startNum,int totalNum,float delay,bool repeat /* = false*/)
{
    int index = startNum;
    string* files = new string[totalNum];
    for (int i=0; i < totalNum; i++) {
        char name[128];
        sprintf(name, format,index);index++;
        files[i] = resMgr->getSharedFilePath(name);
    }
    
    
    CCFiniteTimeAction* action = (CCFiniteTimeAction*)SpriteHelper::createAction(files,
                                                               totalNum,
                                                               delay,repeat);

	delete [] files;
    
    return action;
}


CCFiniteTimeAction* BaseDynamicObj::easyCreateAction(const char *format, int startNum, int totalNum, float delay,CCObject* target,SEL_CallFuncO callBack)
{
    CCFiniteTimeAction* framAct = easyCreateAction(format, startNum, totalNum, delay,false);
    CCFiniteTimeAction* action =
            (CCFiniteTimeAction*)SpriteHelper::createAction(framAct,
                                                            callBack,
                                                            target);
    
    return action;
}

