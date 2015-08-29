#ifndef __BaseContentWrapLayer_H__
#define __BaseContentWrapLayer_H__

#include "cocos2d.h"
#include "CrossPlatformSizeManager.h"
#include "BaseContentLayer.h"

using namespace cocos2d;


class VelocityCalculate;
class ScrollController;

#define MSG_FIXED_SCALE_END		"MSG_FIXED_SCALE_END"	// 顶点缩放动画完毕

//
//	一个BaseContenLayer的外部包裹类，就是一个CCScrollView图层;
//
class BaseContentWrapLayer: public CCLayer
{
public:
	////创建一个ScrollView图层;
	//static BaseContentWrapLayer* createLayer(CCSize viewProtSize,CCNode* container);
	//使用默认的容器用于缩放的,使用内部的有2个尺寸的容器，会根据放大缩小自己切换。
	//默认是缩小;
	static BaseContentWrapLayer* createLayer(BaseScense* scene);
	
	virtual ~BaseContentWrapLayer();

	void setBaseContentLayer(BaseContentLayer* layer,int zOrder = 0);
	BaseContentLayer* getBaseContentLayer();

	bool haveInitScaled(){return mCurrentInitCenterScale > 1.0f;};
	// 是否有双击点放大
	bool haveFixedScaled(){return mCurrentFixScale > 1.0f;};
    bool isFixedScaling(){return mFixedScaling;}

	// 用于双击，把双几点放大
	void setWrapperScaleWithFixPoint(float scale,CCPoint glPoint,bool animation = true);
	// 如果animation_time <=0 那么就没有动作
	void setWrapperScaleWithFixPoint(float scale,CCPoint glPoint,float animation_time = 0.5f);

	// 用于初始化时的指定放大
	void setWrapperInitScaleWithCenterPoint(float scale,CCPoint newNativeCenterPosIn1024_768,bool annimation = true);

	void setScrollControlerr(ScrollController* pScrollController);

	void setWapper(CCLayer* pWrapper);
	CCLayer* getWrapper(){return mWrapper;};

	CCPoint	getInitPos(){return mCurrentInitPos;};
	float getCurrentInitCenterScale(){return mCurrentInitCenterScale;};

	void createVCalculate();

private:
	BaseContentWrapLayer(BaseScense* scene);

	void scalingEnd(CCNode* selfNode);

	void getBoarderPos(float& minX,float& minY,float& maxX,float& maxY);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet* pTouches, cocos2d::CCEvent* event);

	//被包裹封装的类
	BaseContentLayer* wrapedLayer;

	// 真正的用来hold各个需要包裹的layer的对象
	// 那么BaseContentWrapLayer就是hold了wrapperForInit的layer了
	// 也就是套了2层！
	CCLayer* mWrapper;

	float mCurrentInitCenterScale;
	CCPoint mCurrentInitCenterScalePoint;
	CCPoint mCurrentInitPos;
	float mCurrentFixScale;
	BaseScense* mBaseScene;

	bool mFixedScaling; // 是否在缩放

	CCPoint mLastTouchPos;

	VelocityCalculate* mVCalculate;
	ScrollController* mScrollControler;

};

#endif