#ifndef __SectionGuide_H__
#define __SectionGuide_H__

#include "cocos2d.h"
#include "CommonDefine.h"
#include "BaseContentLayer.h"

#define GUIDE_GAP 3.0f

using namespace cocos2d;

//一个每一关的指导的类
class SectionGuide : public CCNode
{
public:
	//自动帮我们引导
	~SectionGuide(); 
	SectionGuide(BaseContentLayer* baseContentLayer,int seasonId,int sectionId);

	//回调：baseContenLayer发生真正点击后回调
	void baseContenLayerClikced(CCPoint glClickPoint);

	//回调：baseContenLayer发生有效点击后回调
	//点击可点或不可点都回调
	void baseContenLayerEffectiveClicked(CCPoint glClickPoint,CCNode* clikedItem);
	//隐藏等待提示
	void hideWatingPrompt();

	//显示开始的教学，没有的话，什么也不会做
	void showInitGuide();
	//是否处于Guide中
	bool guide(){return isGuide;};

//	//程序外部不要设置
//	void setGuideState(bool isGuide){this->isGuide = isGuide;};

	CCProgressTimer* redStartPrompt;
	CCProgressTimer* redStartToTableArrows;
	CCSprite* boxClickPrompt;
private:
	//创建红星的提示
	CCProgressTimer* createRedStartPrompt();
	//创建红星到桌面的路径箭头
	CCProgressTimer* createRedStartToTableArrows();
	//创建提示 点击炸弹的图片
	CCSprite* createBombClickPrompt();
	//创建提示 点击Box的图片
	CCSprite* createBoxClickPrompt();
	//创建 等待图标的提示图片
	CCSprite* createWatingPrompt();
	//创建一个不可以消除的提示，是隐藏的。到时要显示时就直接调用showCanNotDestoryPrompt
	CCSprite* createCanNotDestoryPrompt();
    CCSprite* createShine();
    CCAction* createClickAct_retain();

	//时钟摇晃的动画
	//CCAction* timmerShake;

	//显示不可点击的动画，最后隐藏自己
	void showForbidenPrompt(CCPoint pos);
	//显示等待的提示
	void showWatingPrompt(CCPoint timmerPos);

	//显示炸弹的提示，如果有提示就返回true
	bool showBombClickPrompt(CCPoint pos);
	//显示第一章的第一关的教学,如果有教学就返回true
	bool showFirstSectionGuide(CCPoint redStarPos,CCPoint clickBoxPos,CCPoint arrowsPos);


    //开始显示
    void timeToShowFinger(float time);
    
    //指导完毕，Hide，然后继续
    void hideGuide(float time);
    
	//红星的指示圈更新完毕
	void redStartPromptFinishedShow(CCObject* guide);
	void arrowsPromptFinishedShow(CCObject* guide);
	void boxClickPromptFinishedShow(CCObject* guide);
	void bombClickPromptFinishedShow(CCObject* guide);

	void hideInitPrompt();

	void hide(CCObject* obj);

	BaseContentLayer* mBaseContentLayer;

	bool isGuide;
	//用户是否在教学后进行了有效点击
	bool isUserDoEffectiveClickAffterGuide;
	
    CCPoint center;
    CCSize winSize;
	CCSprite* bombClickPrompt;
	CCSprite* watingPrompt;
	CCSprite* forbidenPrompt;
    CCSprite* shine;
    
    CCAction* clickAction;
    
	//创建一个Action，action完毕之后回调 void XXXXXX(CCObject* obj)的方法，obj就是param
	CCAction* createAction(CCFiniteTimeAction* action,SEL_CallFuncO finishCallBack, CCObject* param);
	
	//再次显示Guide
	void showInitGuideAgain(float time);
};
#endif