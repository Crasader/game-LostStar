//
//  BackgroudLayerS2.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#include "BackgroundLayer_Desert.h"

BackgroundLayer_Desert::BackgroundLayer_Desert(int seasonId, int seactionId):BaseBackgroundLayer(seasonId,seactionId)
{
    //添加一只乌鸦
    {
        Crow* crow = new Crow();
        addDynamicObj(crow);
        addTriggerDynamicObj(crow);
        crow->setPosition(ccp(132,158));
        
        addChild(crow);
        crow->release();
    }
    
    //添加一只老鼠
    {
        Mouse* mouse = new Mouse();
        addDynamicObj(mouse);
        addTriggerDynamicObj(mouse);
        mouse->setPosition(ccp(313,43));
        
        addChild(mouse);
        mouse->release();
    }
    
    //添加一个骷髅
    {
        Skeleton* skeleton = new Skeleton();
        addDynamicObj(skeleton);
        addTriggerDynamicObj(skeleton);
        skeleton->setPosition(ccp(893,61));
        
        addChild(skeleton);
        skeleton->release();
    }
    
    
    
}



//===================================
//   乌鸦的实现
//===================================
Crow::Crow()
{
    
    ResManager* resMgr = ResManager::getManager();
    
    initWithFile(resMgr->getSharedFilePath("corw_sleep_1.png").c_str());
    runingAway = false;
    
    int crowSleepFileNum = 37;
    string* crowSleepFiles = new string[crowSleepFileNum];
    for (int i=0; i<crowSleepFileNum; i++) {
        char temp[56];
        sprintf(temp, "corw_sleep_%d.png",i+1);//corw_sleep_1.png
        crowSleepFiles[i] = resMgr->getSharedFilePath(temp);
    }
    
    sleepAction = (CCFiniteTimeAction*)SpriteHelper::createAction(crowSleepFiles,//图片
                                                                                  crowSleepFileNum,//数量
                                                                                  0.15f,//帧间隔
                                                                                  true);//永远播放
    sleepAction->retain();
//    //播放默认动画
//    runAction(sleepAct);
    delete [] crowSleepFiles;
    
    int crowRunAwayFileNum = 13;
    string* crowRunAwayFiles = new string[crowRunAwayFileNum];
    for (int i=0; i<crowRunAwayFileNum; i++) {
        char temp[56];
        sprintf(temp, "crow_runwaay_%d.png",i+1);//crow_runwaay_1.png
        crowRunAwayFiles[i] = resMgr->getSharedFilePath(temp);
    }    
    
    
    CCFiniteTimeAction* runAwayAct = (CCFiniteTimeAction*)SpriteHelper::createAction(crowRunAwayFiles,//图片
                                                                                   crowRunAwayFileNum,//数量
                                                                                   0.1f,//帧间隔
                                                                                   false);//永远播放

    
    runAwayAction = SpriteHelper::createAction(runAwayAct,
                                           SEL_CallFuncO(&Crow::actionEnd),
                                           this);
    runAwayAction->retain();
    
    delete [] crowRunAwayFiles;
    
//    schedule( schedule_selector(Crow::doAction),Crow_TIME_ACTION );
}

Crow::~Crow()
{
    CC_SAFE_RELEASE(runAwayAction);
    CC_SAFE_RELEASE(sleepAction);
    unscheduleAllSelectors();
}
bool Crow::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Crow::doAction(float time)
{
//    unschedule( schedule_selector(Crow::doAction));
    isAction = true;
    stopAllActions();
    if (sleepAction) {
        runAction(sleepAction);
    }
}

void Crow::actionEnd(cocos2d::CCObject* crow)
{
    Crow* _crow = (Crow*)crow;
    _crow->removeFromParentAndCleanup(true);
}

void Crow::backgroudClicked(cocos2d::CCPoint glPoint)
{
    //如果不是在动画期间，那么每次都是更新等待时间
    if (!isAction) {
//        unschedule( schedule_selector(Crow::doAction));
//        schedule( schedule_selector(Crow::doAction),Crow_TIME_ACTION );
    }else{
        if (!runingAway) {
            //播放逃跑动画
            stopAllActions();
            runAction(runAwayAction);
            runingAway = true;
        }
    }
}

//===================================
//   老鼠的实现
//===================================

Mouse::Mouse()
{
    ResManager* resMgr = ResManager::getManager();
    
    runingAway = false;
    isAction = false;
    int mouseAppearFileNum = 40;
    string* mouseAppearFiles = new string[mouseAppearFileNum];
    for (int i=0; i<mouseAppearFileNum; i++) {
        char temp[56];
        sprintf(temp, "mouse_appear_%d.png",i+1);//mouse_appear_1.png
        mouseAppearFiles[i] = resMgr->getSharedFilePath(temp);
    }
    
    CCFiniteTimeAction* appearAct = (CCFiniteTimeAction*)SpriteHelper::createAction(mouseAppearFiles,//图片
                                                                  mouseAppearFileNum,//数量
                                                                  0.1f,//帧间隔
                                                                  false);//永远?
    delete [] mouseAppearFiles;
    
    int shakeFileNum = 2;
    string shakeFiles[] = {resMgr->getSharedFilePath("mouse_shake_1.png"),resMgr->getSharedFilePath("mouse_shake_2.png")};
    
    CCFiniteTimeAction* shakeAct = (CCFiniteTimeAction*)SpriteHelper::createAction(shakeFiles,//图片
                                                                                      shakeFileNum,//数量
                                                                                      0.15f,//帧间隔
                                                                                      true);//永远?

    mouseApearAndShakeAct = CCSequence::create(appearAct,shakeAct,NULL);
    
    mouseApearAndShakeAct->retain();
    
    
    int mouseRunAwayFileNum = 12;
    string* mouseRunAwayFiles = new string[mouseRunAwayFileNum];
    for (int i=0; i<mouseRunAwayFileNum; i++) {
        char temp[56];
        sprintf(temp, "mouse_runaway_%d.png",i+1);//mouse_runaway_1.png
        mouseRunAwayFiles[i] = resMgr->getSharedFilePath(temp);
    }
    
    CCFiniteTimeAction* runAwayAct = (CCFiniteTimeAction*)SpriteHelper::createAction(mouseRunAwayFiles,//图片
                                                mouseRunAwayFileNum,//数量
                                                0.1f,//帧间隔
                                                false);//永远?
    mouseRunAwayAct = SpriteHelper::createAction(runAwayAct,
                                                 SEL_CallFuncO(&Mouse::actionEnd),
                                                 this);
    
    mouseRunAwayAct->retain();
    
    
//    schedule(schedule_selector(Mouse::doAction),Mouse_TIME_ACTION);
}

Mouse::~Mouse()
{
    CC_SAFE_RELEASE(mouseRunAwayAct);
    CC_SAFE_RELEASE(mouseApearAndShakeAct);
}
bool Mouse::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Mouse::doAction(float time)
{
//    unschedule(schedule_selector(Mouse::doAction));
    isAction = true;
    setVisible(true);
    if (mouseApearAndShakeAct != NULL) {
        stopAllActions();
        runAction(mouseApearAndShakeAct);
    }
}
void Mouse::actionEnd(CCObject* m)
{
    Mouse* mouse = (Mouse*)m;
    mouse->setVisible(false);
    mouse->runingAway = false;
    mouse->isAction = false;
    //重新等待时间又开始运动
//    mouse->schedule(schedule_selector(Mouse::doAction),Mouse_TIME_ACTION);
}
void Mouse::backgroudClicked(CCPoint glPoint)
{
    if (!isAction) {//还没开始运动，那么就重新延时
//        unschedule(schedule_selector(Mouse::doAction));
//        schedule(schedule_selector(Mouse::doAction),Mouse_TIME_ACTION);
    }else{
        if (!runingAway) {
            stopAllActions();
            
            runAction(mouseRunAwayAct);
            
            runingAway = true;
        }
    }
    
}


//===================================
//   骷髅的实现
//===================================

Skeleton::Skeleton()
{
    ResManager* resMgr = ResManager::getManager();
    
    initWithFile(resMgr->getSharedFilePath("skeleton_smoke_1.png").c_str());
    
    restoring = false;
    isAction = false;
    int smokeFileNum = 19;
    string*smokeFiles = new string[smokeFileNum];
    for (int i=0; i<smokeFileNum; i++) {
        char temp[56];
        sprintf(temp, "skeleton_smoke_%d.png",i+1);//skeleton_smoke_1.png
        smokeFiles[i] = resMgr->getSharedFilePath(temp);
    }
    
    CCFiniteTimeAction* smokeAct = (CCFiniteTimeAction*)SpriteHelper::createAction(smokeFiles,//图片
                                                                                    smokeFileNum,//数量
                                                                                    0.1f,//帧间隔
                                                                                    false);//永远?
    delete [] smokeFiles;
    
    int blinkFileNum = 2;
    string blinkFiles[] = {resMgr->getSharedFilePath("skeleton_blink_1.png"),resMgr->getSharedFilePath("skeleton_blink_2.png")};
    
    CCFiniteTimeAction* blinkAct = (CCFiniteTimeAction*)SpriteHelper::createAction(blinkFiles,//图片
                                                                                   blinkFileNum,//数量
                                                                                   0.15f,//帧间隔
                                                                                   true);//永远?
    
    smokeAndBlinkAct = CCSequence::create(smokeAct,blinkAct,NULL);
    
    smokeAndBlinkAct->retain();

    
    int restoreFileNum = 9;
    string* restoreFiles = new string[restoreFileNum];
    for (int i=0; i<restoreFileNum; i++) {
        char temp[56];
        sprintf(temp, "skeleton_restore_%d.png",i+1);//skeleton_restore_1.png
        restoreFiles[i] = resMgr->getSharedFilePath(temp);
    }
    
    CCFiniteTimeAction* runAwayAct = (CCFiniteTimeAction*)SpriteHelper::createAction(restoreFiles,//图片
                                                                                     restoreFileNum,//数量
                                                                                     0.12f,//帧间隔
                                                                                     false);//永远?
    restoreAct = SpriteHelper::createAction(runAwayAct,
                                                 SEL_CallFuncO(&Skeleton::actionEnd),
                                                 this);
    
    restoreAct->retain();
    
    
//    schedule(schedule_selector(Skeleton::doAction),Skeleton_TIME_ACTION);

    

}
Skeleton::~Skeleton()
{
    CC_SAFE_RELEASE(smokeAndBlinkAct);
    CC_SAFE_RELEASE(restoreAct);
}
bool Skeleton::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Skeleton::doAction(float time)
{
//    unschedule(schedule_selector(Skeleton::doAction));
    isAction = true;
    if (smokeAndBlinkAct != NULL) {
        stopAllActions();
        runAction(smokeAndBlinkAct);
    }
}

void Skeleton::actionEnd(cocos2d::CCObject *s)
{
    Skeleton* skeleton = (Skeleton*)s;
    skeleton->isAction = false;
    skeleton->restoring = false;
    
//    skeleton->schedule(schedule_selector(Skeleton::doAction),Skeleton_TIME_ACTION);
}

void Skeleton::backgroudClicked(CCPoint glPoint)
{
    if (!isAction) {//还没开始运动，那么就重新延时
//        unschedule(schedule_selector(Skeleton::doAction));
//        schedule(schedule_selector(Skeleton::doAction),Skeleton_TIME_ACTION);
    }else{
        if (!restoring) {
            stopAllActions();
            
            runAction(restoreAct);
            
            restoring = true;
        }
    }
}
    
