#pragma once
#include "cocos2d.h"
#include <stack>
#include <iostream>
#include <queue>
#include "Myobject.h"
#include "Uimsg.h"
#include "cocos-ext.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
class Starseneui:public CCLayer
{
public:
	Starseneui(void);
	~Starseneui(void);
	virtual bool init();  
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static CCScene* scene(int tag=Starmain_tag);
	// implement the "static node()" method manually
	CREATE_FUNC(Starseneui);
public:
	//返回上一级
	void Fahuiparentlayer(Myobject* object);
	//重定向
	void ChongdingX(Myobject* object);
    //去除当前页面
	void removecurrentLayer();
	void switchStateChanged(CCObject* obj);
	void byTagloadlayer(int tag,Myobject* object);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	void registerWithTouchDispatcher();

	int getTouchID(){return mTouchID;};
	bool getIsMoveActionOver(){return m_bMoveActionOver;};
public:

	void Goxiaogk(int tag);

	void removeOtherLayers(CCNode* pNode);
	
	static void changeBg(int seasonId, CCScale9Sprite* pSprite);
	static bool checkIsCanTouched(CCLayer* pLayer);

	void fadeInOrOut(CCNode* pParent, float time, bool bOut);

	void GoBiggk(int tag);
	void updateInitData(float dt);
private:
	//页面调转数据
	stack<int> layerStack;

	bool m_bMoveActionOver;

	static const int UI_LAYER_NUM = 3;
	static const Layertag UI_LAYERS[UI_LAYER_NUM];
	CCPoint mLayerPos[UI_LAYER_NUM];
	CCPoint mLayerPosNext[UI_LAYER_NUM];
	CCPoint mStartTouchPoint;
    int mTouchID;
	bool mLayerTouchMoving;
	bool mLayerTouchMovingVertical;
	bool mLayerActionMoving;
	int mAngleDir;
	int mCurLayerIndex;
	double mLastTouchTime;
	bool mIsInVerticalLayer;
	void actionMoveLayer();
	void setLayerNextAction(bool isUp);
	void resetLayerLocation();
	void focusLayer(Layertag tag);
	void doHorizentalTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void doHorizentalTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void doHorizentalTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void mainLayerMoveActionOver(CCNode* pNode);
	void moveActionOver(CCNode* pNode);
};


/*
stack 的基本操作有：
入栈，如例：s.push(x);
出栈，如例：s.pop();注意，出栈操作只是删除栈顶元素，并不返回该元素。
访问栈顶，如例：s.top()
判断栈空，如例：s.empty()，当栈空时，返回true。
访问栈中的元素个数，如例：s.size()。
*/