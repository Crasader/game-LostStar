//
//  BackgroudLayerS6.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-19.
//
//

#include "BackgroundLayer_Castle.h"

BackgroundLayer_Castle::BackgroundLayer_Castle(int seasonId, int seactionId):BaseBackgroundLayer(seasonId,seactionId)
{
    BaseDynamicObj* obj =  easyAddObjToLayer(new Battle(), ccp(75,173));
    addTriggerDynamicObj(obj);
    
    obj = easyAddObjToLayer(new Ghost(), ccp(664,149));
    addTriggerDynamicObj(obj);
//    easyAddObjToLayer(new Imp(), ccp(76,35));
    obj = easyAddObjToLayer(new Mushroom(), ccp(948,25));
    addTriggerDynamicObj(obj);
//    easyAddObjToLayer(new Saliva(), ccp(204,24));
//    easyAddObjToLayer(new Skull(), ccp(615,25));
}

void BackgroundLayer_Castle::ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    BaseBackgroundLayer::ccTouchesEnded(touches, event);
    
}

void BackgroundLayer_Castle::bombed(int currentBombPower, int maxPower)
{
    BaseBackgroundLayer::bombed(currentBombPower, maxPower);
}



//===========================================
//   下面是战斗的动画程序
//===========================================

//season6_batttle_appear_26.png  season6_battle_fight_3.png season6_battle_death_9.png
Battle::Battle()
{
    
    
    appearAct = easyCreateAction_Retain("batttle_appear_%d.png", 1, 26, 0.1f,
                                        this, SEL_CallFuncO(&Battle::appearEnd));
    
    
    fightAct = easyCreateAction_Retain("battle_fight_%d.png", 1, 3, 0.1f,true);
    
    
    deathAct = easyCreateAction_Retain("battle_death_%d.png", 1, 9, 0.1f,
                                       this, SEL_CallFuncO(&Battle::deathEnd));
    
    
//    initWithFile(resMgr->getSharedFilePath("batttle_appear_1.png").c_str());
    
//    schedule(schedule_selector(Battle::doAction),Battle_TIME_ACTION);

}
bool Battle::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Battle::doAction(float time)
{
//    unschedule(schedule_selector(Battle::doAction));
    setVisible(true);
    isAction = true;
    isDisappearing = false;
    
    runAction(appearAct);
}

void Battle::appearEnd(cocos2d::CCObject *p)
{
    Battle* b = (Battle*)p;
    
    b->stopAllActions();
    
    b->runAction(b->fightAct);
}



void Battle::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
//        unschedule(schedule_selector(Battle::doAction));
//        schedule(schedule_selector(Battle::doAction),Battle_TIME_ACTION);
    }else{
        if (!isDisappearing) {
            stopAllActions();
            runAction(deathAct);
            isDisappearing = true;
        }
    }
}

void Battle::deathEnd(cocos2d::CCObject *p)
{
    Battle* b = (Battle*)p;
    
    b->stopAllActions();
    
    b->isDisappearing = false;
    b->isAction = false;
    
    b->setVisible(false);
//    b->schedule(schedule_selector(Battle::doAction),Battle_TIME_ACTION);
}






//===========================================
//   下面是魔鬼的动画程序
//===========================================

//season6_ghost_appear_14.png  season6_ghost_flutter_6.png season6_ghost_disappear_6.png
Ghost::Ghost()
{
    
    
    appearAct = easyCreateAction_Retain("ghost_appear_%d.png", 1, 14, 0.1f,
                                        this, SEL_CallFuncO(&Ghost::appearEnd));
    
    
    flutterAct = easyCreateAction_Retain("ghost_flutter_%d.png", 1, 6, 0.1f,true);
    
    
    disappearAct = easyCreateAction_Retain("ghost_disappear_%d.png", 1, 6, 0.1f,
                                       this, SEL_CallFuncO(&Ghost::disappearEnd));
    
    
    //    initWithFile(resMgr->getSharedFilePath("spider_down_1.png").c_str());
    
//    schedule(schedule_selector(Ghost::doAction),Battle_TIME_ACTION);
    
}
bool Ghost::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Ghost::doAction(float time)
{
//    unschedule(schedule_selector(Ghost::doAction));
    setVisible(true);
    isAction = true;
    isDisappearing = false;
    
    runAction(appearAct);
}

void Ghost::appearEnd(cocos2d::CCObject *p)
{
    Ghost* b = (Ghost*)p;
    
    b->stopAllActions();
    
    b->runAction(b->flutterAct);
}



void Ghost::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
//        unschedule(schedule_selector(Ghost::doAction));
//        schedule(schedule_selector(Ghost::doAction),Battle_TIME_ACTION);
    }else{
        if (!isDisappearing) {
            stopAllActions();
            runAction(disappearAct);
            isDisappearing = true;
        }
    }
}

void Ghost::disappearEnd(cocos2d::CCObject *p)
{
    Ghost* g = (Ghost*)p;
    
    g->stopAllActions();
    
    g->isDisappearing = false;
    g->isAction = false;
    
    g->setVisible(false);
//    g->schedule(schedule_selector(Ghost::doAction),Battle_TIME_ACTION);
}





//===========================================
//   下面是 小怪 的动画程序 season6_imp_blink_4.png
//===========================================

Imp::Imp()
{
    CCAction* frameAction = easyCreateAction_Retain("imp_blink_%d.png", 1, 4, 0.1f,true);
    
    runAction(frameAction);
}



//===========================================
//   下面是 小怪 的动画程序 season6_mushroom_move_24-ipadhd.png  season6_mushroom_blast_13-ipadhd.png
//===========================================
Mushroom::Mushroom()
{
    move = easyCreateAction_Retain("mushroom_move_%d.png", 1, 24, 0.1f,
                                   this, SEL_CallFuncO(&Mushroom::moveEnd));
    
    blast = easyCreateAction_Retain("mushroom_blast_%d.png", 1, 13, 0.1f,
                                    this, SEL_CallFuncO(&Mushroom::blastEnd));
    
    
    initWithFile(resMgr->getSharedFilePath("mushroom_move_1.png").c_str());
//    schedule(schedule_selector(Mushroom::doAction),Mushroom_TIME_ACTION);
}
bool Mushroom::startAction()
{
    if (!isAction) {
        doAction(0.0f);
    }
    return true;
}
void Mushroom::doAction(float time)
{
//    unschedule(schedule_selector(Mushroom::doAction));
    isAction = true;
    isDisappearing = false;
    runAction(move);
}

void Mushroom::moveEnd(cocos2d::CCObject *m)
{
    Mushroom* mushroom = (Mushroom*)m;
    
    mushroom->stopAllActions();
}
void Mushroom::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
//        unschedule(schedule_selector(Mushroom::doAction));
//        schedule(schedule_selector(Mushroom::doAction),Mushroom_TIME_ACTION);
    }else{
        if (!isDisappearing) {
            stopAllActions();
            runAction(blast);
            isDisappearing = true;
        }
    }
}

void Mushroom::blastEnd(cocos2d::CCObject *m)
{
    Mushroom* mushroom = (Mushroom*)m;
    
//    mushroom->isAction = false;
//    mushroom->isDisappearing = false;
    
    mushroom->stopAllActions();
    
//    mushroom->schedule(schedule_selector(Mushroom::doAction),Mushroom_TIME_ACTION);
}





//===========================================
//   下面是 口水怪 的动画程序 season6_saliva_open_19.png season6_saliva_close_11.png
//===========================================
Saliva::Saliva()
{
    openAct = easyCreateAction_Retain("saliva_open_%d.png", 1, 19, 0.1f,
                                   this, SEL_CallFuncO(&Saliva::openEnd));
    
    closeAct = easyCreateAction_Retain("saliva_close_%d.png", 1, 11, 0.1f,
                                    this, SEL_CallFuncO(&Saliva::closeEnd));
    
    
    initWithFile(resMgr->getSharedFilePath("saliva_open_1.png").c_str());
    schedule(schedule_selector(Saliva::doAction),Saliva_TIME_ACTION);
}

void Saliva::doAction(float time)
{
    unschedule(schedule_selector(Saliva::doAction));
    isAction = true;
    isCloseing = false;
    runAction(openAct);
}

void Saliva::openEnd(cocos2d::CCObject *m)
{
    Saliva* saliva = (Saliva*)m;
    
    saliva->stopAllActions();
}
void Saliva::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
        unschedule(schedule_selector(Saliva::doAction));
        schedule(schedule_selector(Saliva::doAction),Saliva_TIME_ACTION);
    }else{
        if (!isCloseing) {
            stopAllActions();
            runAction(closeAct);
            isCloseing = true;
        }
    }
}

void Saliva::closeEnd(cocos2d::CCObject *s)
{
    Saliva* saliva = (Saliva*)s;
    
    saliva->isAction = false;
    saliva->isCloseing = false;
    
    saliva->stopAllActions();
    
    saliva->schedule(schedule_selector(Saliva::doAction),Saliva_TIME_ACTION);
}




//===========================================
//   下面是 口水怪 的动画程序 season6_skull_blow_46.png  season6_skull_ignite_3.png season6_skull_blast_11.png
//===========================================

Skull::Skull()
{
    blowAct = easyCreateAction_Retain("skull_blow_%d.png", 1, 46, 0.1f,
                                      this, SEL_CallFuncO(&Skull::blowEnd));
    
    igniteAct = easyCreateAction_Retain("skull_ignite_%d.png", 1, 3, 0.1f,true);
    
    
    blastAct = easyCreateAction_Retain("skull_blast_%d.png", 1, 33, 0.1f,
                                       this, SEL_CallFuncO(&Skull::blastEnd));
    
    
    schedule(schedule_selector(Skull::doAction),Skull_TIME_ACTION);
    
}

void Skull::doAction(float time)
{
    unschedule(schedule_selector(Skull::doAction));
    
    isAction = true;
    isBlasting = false;
    setVisible(true);
    runAction(blowAct);
    
}
void Skull::blowEnd(CCObject* m)
{
    Skull* skull = (Skull*)m;
    
    skull->stopAllActions();
    
    skull->runAction(skull->igniteAct);
    
}
void Skull::backgroudClicked(CCPoint glPoint)
{
    if (!isAction) {
        unschedule(schedule_selector(Skull::doAction));
        schedule(schedule_selector(Skull::doAction),Skull_TIME_ACTION);
    }else{
        if (!isBlasting) {
            stopAllActions();
            runAction(blastAct);
            isBlasting = true;
        }
    }
}
void Skull::blastEnd(CCObject* m)
{
    Skull* skull = (Skull*)m;
    
    skull->stopAllActions();
    skull->setVisible(false);
    skull->schedule(schedule_selector(Skull::doAction),Skull_TIME_ACTION);
}





//===========================================
//   下面是 吸血鬼 的动画程序
//   season6_vampire_fly_3.png  season6_vampire_turn_7.png
//   season6_vampire_stare_6.png season6_vampire_runaway_13.png
//===========================================

//Vampire::Vampire()
//{
//    fly = easyCreateAction_Retain(const char *format, int startNum, int totalNum, float delay)
//    turn;
//    stare;
//    runaway;
//}
//
//void Vampire::doAction(float time)
//{
//    
//}











