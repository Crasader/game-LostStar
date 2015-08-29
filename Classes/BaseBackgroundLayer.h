//
//  BaseBackgroundLayer.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#ifndef __RedStarFall__BaseBackgroundLayer__
#define __RedStarFall__BaseBackgroundLayer__

#include "cocos2d.h"
#include <string>
#include <list>
#include "ResManager.h"
#include "BaseContentLayer.h"
#include "SpriteHelper.h"
#include "IBorder.h"

using namespace cocos2d;
using namespace std;

class BaseDynamicObj;

#define TIME_DYNAMIC_ACTION 5
//一个基础的背景图层
class BaseBackgroundLayer : public cocos2d::CCLayer 
{
public:
    BaseBackgroundLayer(int seasonId, int seactionId);
    ~BaseBackgroundLayer();
    //添加动态物体到管理器里面，没有加入到场景中
    void addDynamicObj(BaseDynamicObj* obj);
    void setBaseContentLayer(BaseContentLayer* baseContentLayer);
    //移除
    void removeDynamicObj(BaseDynamicObj* obj);
    
    //添加一个元素到layer，也到动态物体的链表里,会自动帮我们释放,并把obj返回来，以便链式编程
    BaseDynamicObj* easyAddObjToLayer(BaseDynamicObj* obj,CCPoint pos,bool release = true);
    
    //爆炸发生了就会回调
    virtual void bombed(int currentBombPower,int maxPower);
    
    //得到一个随机的桌子,参数设置这次是否也需要随机获得
    //本来是第一次随机得到一个桌子，但是如果第二次调用时randomAgain为true，那么就又会随机
    CCNode* getRandomTable(bool randomAgain);
    
    list<CCNode*> getTableList(b2World* world);
    CCNode* getNode(list<CCNode*> list,int index);
    
    //给一个触发动画播放的物体
    BaseDynamicObj* addTriggerDynamicObj(BaseDynamicObj* obj);
    void removeTriggerDynamicObj(BaseDynamicObj* obj);

    CCNode* getRedStar();

	CCSprite* getBackGroundPic(){return mBg;};
    
    void setTimeGapForDynamicAct(float newTime);
protected:
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    BaseContentLayer* baseContentLayer;
    
    //开始播放动态物体的动画
    virtual void startDynamicObjAction(float time);
    
    CCSize mScreenSize;
	CCSprite* mBg;
private:
    list<BaseDynamicObj*> dynamicObjList;
    //触发播放动画的动态物体，这里是dynamicObjList的子集
    vector<BaseDynamicObj*> dynamictTriggerVec;
    CCNode* table;
    float timeGapForDynamicAct;//动态物体开始动画的时间间隔，也就是没有操作的时间间隔
};

//场景中的动态物体
class BaseDynamicObj : public CCSprite,IBorder
{
public:
    BaseDynamicObj();
    ~BaseDynamicObj();
    virtual void backgroudClicked(CCPoint glPoint);
    
    void setBaseBackgroundLayer(BaseBackgroundLayer* layer);
    
    //和addDynamicObj对应
    void removeFromLayerDynamicObjList();
    void removeFromTriggerDynamicVec();
    
    //开始动画,如果不需要动画，就返回false，那么系统就会自动调用下一个。
    virtual bool startAction();
    
    //判断点是否在我里面，以后可以扩展成IBorder
    bool pointInMe(CCPoint glPoint);
    
    //获得我的边界，默认就是boundingBox
    virtual std::list<CCRect> getBounds();

protected:
    
    //可以使用CCSpawn来实现所有动画一起执行

    //根据format生成动画 ,例如"shark_%d.png",2那么就是从shark_2.png开始,一共totalNum多个
    CCFiniteTimeAction* easyCreateAction(const char * format,int startNum,int totalNum,
                                         float delay,bool repeat = false);
    
    CCFiniteTimeAction* easyCreateAction(const char * format,int startNum,int totalNum,
                                         float delay,CCObject* target,SEL_CallFuncO callBack);
    
    //会自己帮我们retain，然后在这个类不要的时候又release
    CCFiniteTimeAction* easyCreateAction_Retain(const char * format,int startNum,int totalNum,
                                                float delay,bool repeat = false);
    
    //会自己帮我们retain，然后在这个类不要的时候又release
    CCFiniteTimeAction* easyCreateAction_Retain(const char * format,int startNum,int totalNum,
                                                float delay,CCObject* target,SEL_CallFuncO callBack);
    //是否是处于动画阶段
    bool isAction;
    ResManager* resMgr;
    BaseBackgroundLayer* baseBgLayer;
private:
    list<CCAction* > releasePool;
};
#endif /* defined(__RedStarFall__BaseBackgroundLayer__) */
