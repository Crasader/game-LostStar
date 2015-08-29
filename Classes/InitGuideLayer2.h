#ifndef __InitGuideLayer2_H__
#define __InitGuideLayer2_H__
#include "LFLayer.h"
#include <vector>
#include <string>
#include "Box2d/Box2D.h"
#include "VRope.h"
#include "ResManager.h"
#include "BaseMoveUI.h"
#include "MainThingEffect.h"

class InitGuideLayer2 : public LFLayer,public BaseMoveUI
{
private:
	CCSprite* guide_pic_1 ;
	CCSprite* guide_pic_2 ;
	CCSprite* guide_pic_2_cover ;
	CCSprite* guide_pic_3 ;
// 	CCSprite* guide_pic_4 ;
// 	CCSprite* guide_pic_5 ;

	CCMenu* menu ;

	CCSprite* createSprite(const char* file,CCNode* parent,CCPoint pos, int zOrder = 0);

	void nextClicked(CCObject* obj);

	void end(float t);

	void reTry(CCObject* obj);

	void goToGame();
public:
	InitGuideLayer2();
private:
	vector<MainThingEffect*>		m_sMainThingList;
private:
	inline void moveThing()
	{
		int pos[8][2] = {{160,460},{222,218},{1207,313},{458,188},{680,48},{815,265},{998,172},{1241,500}};
		for (int i = 1; i < 9; i ++)
		{
			CCString *str = CCString::createWithFormat(ResManager::getManager()->getSharedFilePath(g_mainlayerPath+"main_xiaoguo_piaofu_%d.png").c_str(),i);
			CCSprite *oneSpr = CCSprite::create(str->getCString());
			MainThingEffect *one = MainThingEffect::create();
			one->initWithTexture(oneSpr->getTexture());
			//one->setAnchorPoint(CCPointZero);
			one->setPosition(ccp(pos[i - 1][0],pos[i - 1][1]));
			addChild(one,-1);
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