//
//  BackgroudLayerS5.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-19.
//
//

#include "BackgroundLayer_Forest.h"

BackgroundLayer_Forest::BackgroundLayer_Forest(int seasonId, int seactionId):BaseBackgroundLayer(seasonId,seactionId)
{
    //创建飞絮
    {
        DandelionConfig config;
        config.startMinX = -10;
        config.startMaxX = 76;
        config.startMinY = 274;
        config.startMaxY = 683;
        config.endMinX = -10;
        config.endMaxX = 615;
        config.endMinY = 52;
        config.endMaxY = 123;
        config.num = 10;
        
        Dandelion::createDandelionsIntoLayer(config, this);

        config.startMinX = 852;
        config.startMaxX = 1044;
        config.startMinY = 189;
        config.startMaxY = 582;
        config.endMinX = 184;
        config.endMaxX = 866;
        config.endMinY = 52;
        config.endMaxY = 153;
        config.num = 7;
        
        Dandelion::createDandelionsIntoLayer(config, this);
    }
    
    //创建一只猴子
    {
        BaseDynamicObj* obj = easyAddObjToLayer(new Monkey(), ccp(28,175));
        addTriggerDynamicObj(obj);
    }
    
    //创建一棵树
    {
        BaseDynamicObj* obj = easyAddObjToLayer(new Plant(), ccp(853,79));
        addTriggerDynamicObj(obj);
    }
    
    //创建南瓜眼睛
    {
        easyAddObjToLayer(new PumpkinEyes(), ccp(188,55));
    }
    
    //创建一只蜗牛
    {
        easyAddObjToLayer(new Snail(), ccp(447,62));
    }
    
    //创建一个蜘蛛
    {
        BaseDynamicObj* obj = easyAddObjToLayer(new Spider(), ccp(18,262));
        addTriggerDynamicObj(obj);
    }
    schedule(schedule_selector(BackgroundLayer_Forest::createDandelion),CRETAE_Dandelion_TIME_ACTION);
}

void BackgroundLayer_Forest::ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    BaseBackgroundLayer::ccTouchesEnded(touches, event);
    
}

void BackgroundLayer_Forest::bombed(int currentBombPower, int maxPower)
{
    BaseBackgroundLayer::bombed(currentBombPower, maxPower);
}

void BackgroundLayer_Forest::createDandelion(float time)
{
    DandelionConfig config;
    config.startMinX = -10;
    config.startMaxX = 76;
    config.startMinY = 274;
    config.startMaxY = 683;
    config.endMinX = -10;
    config.endMaxX = 615;
    config.endMinY = 52;
    config.endMaxY = 123;
    config.num = 5 + CCRANDOM_0_1() * 5;
    
    Dandelion::createDandelionsIntoLayer(config, this);
    
    config.startMinX = 852;
    config.startMaxX = 1044;
    config.startMinY = 189;
    config.startMaxY = 582;
    config.endMinX = 184;
    config.endMaxX = 866;
    config.endMinY = 52;
    config.endMaxY = 153;
    config.num = 3 + CCRANDOM_0_1() * 5;
    
    Dandelion::createDandelionsIntoLayer(config, this);
}



//=======================================================
//  飞絮 //season5_dandelion_15-ipadhd.png
//=======================================================
Dandelion::Dandelion(float posx,float posy,float endPosX,float endPosY)
{
    int index = 15.0f*CCRANDOM_0_1()+1;
    if (index > 15) {
        index = 15;
    }
    
    char fileName[64];
    sprintf(fileName, "dandelion_%d.png",index);
    initWithFile(resMgr->getSharedFilePath(fileName).c_str());
    
    setRotation(30);//旋转角度
    setAnchorPoint(ccp(0.5, 3));//设置精灵锚点
    setPosition(ccp(posx, posy));//叶子1第一次初始位置
    setScale(0.5);//设置叶片大小
    
    time = 10 + 10*CCRANDOM_0_1();//叶子下落的时间
    roTime = 1.5 + CCRANDOM_0_1();//叶子单向摆动一次时间
    fAngle1 = -40 - (CCRANDOM_0_1()*30);//叶子逆时针摆动角度
    fAngle2 = 40 + (CCRANDOM_0_1()*30);//顺时针摆动角度
    
    runAction(createFallAction(time, endPosX, endPosY, roTime,
                               fAngle1,fAngle2, SEL_CallFuncN(&Dandelion::arrived)));
}

void Dandelion::createDandelionsIntoLayer(const DandelionConfig config, BaseBackgroundLayer *layer)
{
    //生成这么多的飞絮
    for (int i=0; i<config.num; i++) {
        
        float startX = config.startMinX + (config.startMaxX - config.startMinX)*CCRANDOM_0_1();
        float startY = config.startMinY + (config.startMaxY - config.startMinY)*CCRANDOM_0_1();
        
        float endX = config.endMinX + (config.endMaxX - config.endMinX)*CCRANDOM_0_1();;
        float endY = config.endMinY + (config.endMaxY - config.endMinY)*CCRANDOM_0_1();
        
        Dandelion* d = new Dandelion(startX,startY,endX,endY);
        
        layer->addDynamicObj(d );
        layer->addChild(d);
        d ->release();
    }
}

CCAction* Dandelion::createFallAction(float fallTime,float endX,float endY,float roTime,
                                      float fAngle1,float fAngle2,SEL_CallFuncN callBack)
{
    
    //叶子所运动到的位置
    CCMoveTo *moveTo = CCMoveTo::create(fallTime,ccp(endX,endY));
    CCCallFuncN *actDone = CCCallFuncN::create(this, callBack);
    CCFiniteTimeAction *putdown = CCSequence::create(moveTo, actDone, NULL);
    //叶子旋转动作
    CCRotateBy *rotaBy1 = CCRotateBy::create(roTime, fAngle1);
    CCRotateBy *rotaBy2 = CCRotateBy::create(roTime, fAngle2);
    
    //叶子翻转动作
    setVertexZ(60);//设置深度抬高60，避免出现使用CCOrbitCamera实现空间翻转时产生错位和遮挡等问题
    //CCDirector::sharedDirector()->setDepthTest(false);
    //关闭深度测试同样可以避免上述问题，不过，推荐使用深度设置setVertexZ来正确解决，因为有时你可能需要遮挡的效果，关闭深度测试后将造成遮挡效果的缺失
    CCOrbitCamera * orbit = CCOrbitCamera::create(8, 1, 0, 0, 360, 45, 0);
    //让树叶精灵始终执行三维翻转的动作
    CCRepeat *fz3d = CCRepeat::create(orbit, -1);//无限循环执行叶片翻转的动作
    //CCRepeatForever *fz3d = CCRepeatForever::actionWithAction(orbit);
    //由于下面使用CCSpawn同时执行动作，所以不可以使用无限次数类型的动作，而因使用有线次数循环CCRepeat将循环次数设置为-1
    
    //用CCEaseInOut包装落叶摆动的动作，让树叶的进入、出现更自然（淡入淡出效果）
    CCEaseInOut *ease1 = CCEaseInOut::create(rotaBy1, 3);
    CCEaseInOut *ease2 = CCEaseInOut::create(rotaBy2, 3);
    //摆动动作合成
    CCFiniteTimeAction *seq2 = CCSequence::create(ease1, ease2, NULL);//依次执行顺时针、逆时针摆动
    CCRepeat *baidong = CCRepeat::create(seq2, -1);//摆动合成
    
    //动作执行->同时执行所有动作
    return CCSpawn::create(putdown, baidong, fz3d, NULL);
}


void Dandelion::arrived(cocos2d::CCNode *dandelion)
{
    Dandelion* d = (Dandelion*)dandelion;
    d->removeFromParentAndCleanup(true);
    d->removeFromLayerDynamicObjList();
}


//=======================================================
//  猴子 //season5_monkey_appear_25.png season5_monkey_blink_6.png season5_monekey_runaway_7.png
//=======================================================

Monkey::Monkey()
{
    monkeyAppearAct = easyCreateAction_Retain("monkey_appear_%d.png", 1, 25, 0.1f,
                                              this, SEL_CallFuncO(&Monkey::monkeyAppearEnd));
    
    monkeyBlinkAct = easyCreateAction_Retain("monkey_blink_%d.png", 1, 6, 0.1f,true);
    
    monkeyRunAwayAct = easyCreateAction_Retain("monekey_runaway_%d.png", 1, 7, 0.1f,
                                               this, SEL_CallFuncO(&Monkey::monkeyRunEnd));
    
//    schedule(schedule_selector(Monkey::doAction),Monkey_TIME_ACTION);
}
bool Monkey::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Monkey::doAction(float time)
{
//    unschedule(schedule_selector(Monkey::doAction));
    
    isAction = true;
    isRuning = false;
    setVisible(true);
    
    runAction(monkeyAppearAct);
}

void Monkey::monkeyAppearEnd(cocos2d::CCObject *m)
{
    Monkey* monkey = (Monkey*)m;
    
    monkey->stopAllActions();
    
    monkey->runAction(monkey->monkeyBlinkAct);
}

void Monkey::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
//        unschedule(schedule_selector(Monkey::doAction));
//        schedule(schedule_selector(Monkey::doAction),Monkey_TIME_ACTION);
    }else{
        if (!isRuning) {
            
            stopAllActions();
            
            runAction(monkeyRunAwayAct);
            
            isRuning = true;
        }
    }
}

void Monkey::monkeyRunEnd(cocos2d::CCObject *m)
{
    Monkey* monkey = (Monkey*)m;
    
    monkey->isRuning = false;
    monkey->isAction = false;
    
    monkey->setVisible(false);
    
//    monkey->schedule(schedule_selector(Monkey::doAction),Monkey_TIME_ACTION);
}



//=======================================================
//  植物 //season5_plant_grow_44.png season5_plant_devil_2.png season5_plant_disappear_8.png
//=======================================================

Plant::Plant()
{
    growUpAct = easyCreateAction_Retain("plant_grow_%d.png", 1, 44, 0.2f,
                                        this, SEL_CallFuncO(&Plant::growUpEnd));
    
    devilAct = easyCreateAction_Retain("plant_devil_%d.png", 1, 2, 0.1f,true);
    
    disappearAct = easyCreateAction_Retain("plant_disappear_%d.png", 1, 8, 0.1f,
                                           this, SEL_CallFuncO(&Plant::disappearEnd));
    
//    schedule(schedule_selector(Plant::doAction),Plant_TIME_ACTION);
}
bool Plant::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Plant::doAction(float time)
{
//    unschedule(schedule_selector(Plant::doAction));
    isAction = true;
    isDisappearing = false;
    setVisible(true);
    runAction(growUpAct);
}

void Plant::growUpEnd(CCObject *p)
{
    Plant* plant = (Plant*)p;
    
    plant->stopAllActions();
    
    plant->runAction(plant->devilAct);
}

void Plant::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
//        unschedule(schedule_selector(Plant::doAction));
//        schedule(schedule_selector(Plant::doAction),Plant_TIME_ACTION);
    }else{
        if (!isDisappearing) {
            
            stopAllActions();
            
            runAction(disappearAct);
            
            isDisappearing = true;
        }
    }
}

void Plant::disappearEnd(CCObject *p)
{
    Plant* plant = (Plant*)p;
    
//    plant->schedule(schedule_selector(Plant::doAction),Plant_TIME_ACTION);
    plant->isDisappearing = false;
    plant->isAction = false;
    plant->setVisible(false);
}


//=======================================================
//  南瓜眼睛 //season5_pumpkin_blink_6.png
//=======================================================
PumpkinEyes::PumpkinEyes()
{
    blink = easyCreateAction_Retain("pumpkin_blink_%d.png", 1, 8, 0.1f);
    
    initWithFile(resMgr->getSharedFilePath("pumpkin_blink_1.png").c_str());
    schedule(schedule_selector(PumpkinEyes::doAction),PumpkinEyes_TIME_ACTION);
}

void PumpkinEyes::doAction(float time)
{
    stopAllActions();
    runAction(blink);
}




//=======================================================
//  蜗牛  season5_snail_right_8.png season5_snail_left_8.png
//=======================================================

Snail::Snail()
{
    rightAct = easyCreateAction_Retain("snail_right_%d.png", 1, 8, 0.1f,
                                      this, SEL_CallFuncO(&Snail::actionEnd));
    leftAct = easyCreateAction_Retain("snail_left_%d.png", 1, 8, 0.1f,
                                       this, SEL_CallFuncO(&Snail::actionEnd));
    
    initWithFile(resMgr->getSharedFilePath("snail_right_1.png").c_str());
    setRotation(-20.0f);
}

void Snail::actionEnd(cocos2d::CCObject *snail)
{
    Snail* s = (Snail*)snail;
    
    s->isAction = false;
}

void Snail::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction  && pointInMe(glPoint)) {
        
        stopAllActions();
        isAction = true;
        if (CCRANDOM_0_1() > 0.5f) {
            
            runAction(rightAct);
        }else{
            runAction(leftAct);
        }
    }
}



//=======================================================
//  蜘蛛  season5_spider_down_15.png  season5_spider_up_14.png
//=======================================================

//
Spider::Spider()
{
    actionUp = easyCreateAction_Retain("spider_up_%d.png", 1, 14, 0.1f,
                                       this, SEL_CallFuncO(&Spider::actionUpEnd));
    
    actionDown = easyCreateAction_Retain("spider_down_%d.png", 1, 15, 0.1f,
                                         this, SEL_CallFuncO(&Spider::actionDownEnd));
    
    
    initWithFile(resMgr->getSharedFilePath("spider_down_1.png").c_str());
    
//    schedule(schedule_selector(Spider::doAction),Spider_TIME_ACTION);
}

bool Spider::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Spider::doAction(float time)
{
//    unschedule(schedule_selector(Spider::doAction));
    isAction = true;
    
    runAction(actionDown);
}
void Spider::actionUpEnd(cocos2d::CCObject *s)
{
    Spider* spider = (Spider*)s;
    
    spider->isUping = false;
    spider->isAction = false;
    
//    spider->schedule(schedule_selector(Spider::doAction),Spider_TIME_ACTION);
}
void Spider::actionDownEnd(cocos2d::CCObject *s)
{
    Spider* spider = (Spider*)s;
    
    CC_UNUSED_PARAM(spider);
}
void Spider::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
//        unschedule(schedule_selector(Spider::doAction));
//        schedule(schedule_selector(Spider::doAction),Spider_TIME_ACTION);
    }else{
        if (!isUping) {
            stopAllActions();
            runAction(actionUp);
            isUping = true;
        }
    }
}


