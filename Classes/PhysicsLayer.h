#ifndef __PhysicsLayer_H__
#define __PhysicsLayer_H__

#include "LFLayer.h"
#include "Box2d/Box2D.h"
#include "VRope.h"
#include "MainThingEffect.h"
#include "BaseMoveUI.h"
using namespace cocos2d;
using namespace std;

class PhysicsLayer : public LFLayer,b2ContactListener,b2ContactFilter,public BaseMoveUI
{
private:
	b2World* mWorld;
	//rope
	CCSpriteBatchNode* mRopeSpriteBatchNode;
	std::vector<VRope*> mRopes;
	CCSize winSize;
	CCMenu* menu;
	CCLayer* mainFrameLayer;

	b2Body* createLeftBody(b2Vec2 p,float angle);	// 创建左边的 Star  木牌
	b2Body* createRightBody(b2Vec2 p,float angle);	// 创建右边的 Fall  木牌
	b2Body* createCenterBody(b2Vec2 p,float angle);	// 创建中间的 Start 木牌
	void createRope(b2Vec2 anchorPos,float length,b2Body* hungBody,bool showRope = true);
	void tick(float time);
	virtual void lfClick(CCPoint glPoint);
	void restarClicked(CCObject* obj);
	void settingClicked(CCObject* obj);

	// 碰撞回调函数
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
public:
	PhysicsLayer(CCLayer* mainFrameLayer);
	~PhysicsLayer();
private:
	vector<MainThingEffect*>		m_sMainThingList;
private:
	inline void moveThing()
	{
		int pos[8][2] = {{160,460},{222,218},{1207,313},{458,188},{680,48},{815,265},{998,172},{1241,500}};
		for (int i = 1; i < 9; i ++)
		{
			CCString *str = CCString::createWithFormat("main_xiaoguo_piaofu_%d.png",i);
			CCSprite *oneSpr = CCSprite::create(str->getCString());
			MainThingEffect *one = MainThingEffect::create();
			one->initWithTexture(oneSpr->getTexture());
			//one->setAnchorPoint(CCPointZero);
			one->setPosition(ccp(pos[i - 1][0],pos[i - 1][1]));
			addChild(one);
			one->setTag(i);
			one->setParent(this);
			m_sMainThingList.push_back(one);
		}
		for (vector<MainThingEffect*>::iterator iter = m_sMainThingList.begin();iter != m_sMainThingList.end();iter++)
		{
			(*iter)->moveEffect();
		}
	}
	inline void moveChangeEnd(MainThingEffect *node)
	{
		node->moveEffect();
	}
};



#endif