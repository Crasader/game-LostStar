#include "BaseLightEffectNode.h"
#include "ResManager.h"


BaseLightEffectNode::BaseLightEffectNode()
	:CCSprite(),
	m_nLightCount(0),
	m_fLightScale(1.0f)
{
}
BaseLightEffectNode::~BaseLightEffectNode()
{
	m_sLightSprList.clear();
}
bool BaseLightEffectNode::init()
{
	if (!CCSprite::init())
	{
		return false;
	}
	return true;
}
void BaseLightEffectNode::onEnter()
{
	CCSprite::onEnter();

	for (int i = 1;i < 11 ; i ++)
	{
		CCString *str = CCString::createWithFormat((ResManager::getManager()->getSharedFilePath()+g_storelayerPath+"main_shangdian_xiaoguo_shanguang_%d.png").c_str(),i);
		CCSprite *lightSpr = NULL;
		if (i == 10)
		{
			lightSpr = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_xiaoguo_shanguang_yuanquan.png").c_str());
		}
		else
		{
			lightSpr = CCSprite::create(str->getCString());
		}
		lightSpr->setTag(i);
		lightSpr->setAnchorPoint(ccp(0.5,0.5));
		lightSpr->setPosition(ccp(103,214));
		lightSpr->setVisible(false);
		lightSpr->setScale(m_fLightScale);
		this->addChild(lightSpr);
		m_sLightSprList.insert(map<int,CCSprite*>::value_type(i,lightSpr));
	}
	this->schedule(SEL_SCHEDULE(&BaseLightEffectNode::showLightEffect),KLightSideTime);
}
/********************************闪光特效*************************************/
void BaseLightEffectNode::showLightEffect(float dt)
{
	if (m_nLightCount == 0)
	{
		this->unschedule(SEL_SCHEDULE(&BaseLightEffectNode::showLightEffect));
		map<int,CCSprite *>::iterator iter = m_sLightSprList.find(5);
		if (iter != m_sLightSprList.end())
		{
			iter->second->setVisible(false);
		}
	}
	else
	{
		map<int,CCSprite *>::iterator iter = m_sLightSprList.find(m_nLightCount);
		if (iter != m_sLightSprList.end())
		{
			iter->second->setVisible(false);
		}
	}
	m_nLightCount++;

	if (m_nLightCount == 10)
	{

		this->unschedule(SEL_SCHEDULE(&BaseLightEffectNode::showLightEffect));

		map<int,CCSprite *>::iterator iter = m_sLightSprList.find(m_nLightCount);
		if (iter != m_sLightSprList.end())
		{
			iter->second->setVisible(true);
			iter->second->setScale(1.0f);
			iter->second->runAction(CCSpawn::create(CCScaleTo::create(0.6f,1.3f),
				CCFadeOut::create(0.5f),
				CCCallFuncN::create(this,SEL_CallFuncN(&BaseLightEffectNode::showLightEnd)),
				//CCScaleTo::create(0,1.0f),
				//CCFadeIn::create(0),
				NULL));
		}
		m_nLightCount = 0;
	}
	else
	{
		map<int,CCSprite *>::iterator iter = m_sLightSprList.find(m_nLightCount);
		if (iter != m_sLightSprList.end())
		{
			iter->second->setVisible(true);
		}
		if (m_nLightCount == 1)
		{
			this->schedule(SEL_SCHEDULE(&BaseLightEffectNode::showLightEffect),KLightTime);
		}
	}
}
void BaseLightEffectNode::showLightEnd(CCNode *node)
{
	//node->setVisible(false);
	this->schedule(SEL_SCHEDULE(&BaseLightEffectNode::showLightEffect),KLightSideTime);
}