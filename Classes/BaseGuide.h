
#ifndef __GUIDE_H__
#define __GUIDE_H__

#include "cocos2d.h"
#include "CommonDefine.h"
#include "GameInforEditer.h"
#include "MainFrameLayer.h"
#include <string>

using namespace cocos2d;
//
//一个图片切换的教学类，完成了图片的载入，只需要重新设置切换的动作，图片的数量等就可以了，
//默认当图片切换完毕就进入主界面，所以，如果要扩展，那么请重写guideEnd方法;
//
class BaseGuide : public CCLayer
{
public:
	//创建一个Logo的场景
	static CCScene* createGuideScene(std::string baseFileName,int guidePicNum);
protected:
	//指导动画的前缀文件名,比如Guide,那么实际找的图片就是Guide_1.png之类的
	std::string guidePicBaseName;

	//最基础的名字，最后的形式是guidePicBaseName_1.png
	BaseGuide(std::string guidePicBaseName,int picNum);
	virtual ~BaseGuide();

	//创建guide页面切换的Action
	virtual CCFiniteTimeAction* createSwitchAction();
	
	//如果引导结束有不一样的操作，那么一定要重写！
	virtual void guideEnd();

	//每完成一个切换都调用，默认就是显示下一张
	virtual void switchGuideFinished();
	
private:
	
	//每次create都会调用
	void initGuideLayer(std::string baseFileName,int guidePicNum);

	virtual void  ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent); 

	CCArray* guideSpriteArray;

	CCSize winSize;

	int currentGuideIndex;

	CCAction* goToNestAction;

	bool switching;

	float switchTime;
	
	int picNum;
};

#endif