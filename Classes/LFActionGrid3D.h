#ifndef __LFActionGrid3D_H__
#define __LFActionGrid3D_H__

#include "cocos2d.h"
using namespace cocos2d;

class LFTwirl : public CCGrid3DAction
{
public:
	LFTwirl();
	~LFTwirl();
    
	///** returns the grid 重写，用于返回自己的Grid*/
	//virtual CCGridBase* getGrid(void);
    virtual void startWithTarget(CCNode *pTarget);
    
	const CCPoint& getPosition(){return m_position;};
	void preCreatGrid(CCNode* actionTarget);				// 提前计算，可以在构造之后就调用
	void preCalculateDistance();							// 这个方法一定要在正在做动作的时候才调用

	void setPosition(const CCPoint pos){m_position = pos;};
//    bool initWithPosition(float radius,float mAngleFactor,float mScaleFactor,const ccGridSize& gridSize, float duration);
//Richard
	bool initWithPosition(float radius,float mAngleFactor,float mScaleFactor,const CCSize& gridSize, float duration);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void update(float time);
//    static LFTwirl* create(float radius,float angleFactor,float scaleFactor,const ccGridSize& gridSize, float duration);
//Richard
	static LFTwirl* create(float radius,float angleFactor,float scaleFactor,const CCSize& gridSize, float duration);
private:
	CCPoint m_position;
	float mAngle;
	float mScale;
	float mRadius;

	float mAngleFactor;
	float mScaleFactor;
    
    int flag;
    float *gridDistanceArray;
    float similarDistance(CCPoint p1,CCPoint p2);
};
#endif